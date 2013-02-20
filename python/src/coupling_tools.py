# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  coupling_tools.py
# @brief Gives functions that help coupling against external code,
#   .i.e: manipulate template file.
#
# Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
# @author souchaud
# @date   2012-08-28
#

import re
import shutil
import os
import shlex
import subprocess
import sys


debug = False

def check_param(obj, obj_type):
    """
    private func
    assert obj as type obj_type
    """

    # int casts into long ; long is deprecated
    if (obj_type == int) and (sys.version_info[0] < 3):
        obj_type = long

    try:
        obj_type(obj)
    except:
        raise AssertionError("error: parameter (" + \
                             str(type(obj)) + ": " + str(obj) + \
                             ") must be of " + str(obj_type) + "!")


def replace(infile, outfile, tokens, values):
    """
    replace tokens on a file

    parameters:
    infile:  template file that will be parsed
    outfile: file that will received the template parsed. If equal to "None" or
             to "infile", the result file will be moved to infile.
    tokens:  a list of regex that will be replaced
    values:  list of values (can be string, float, ...) that will replace
             the tokens. The list must have the same size as tokens.

    exception:
        AssertionError: parameters badly set
        EOFError: a token has not been found

    example:
        # template.in = 'E=@E_VAR F=-PF-')
        replace(infile="template.in",
                outfile="prgm_data.in",
                tokens=["@E_VAR", ".PF."],
                values=[1.4, "5"])
        # => prgm_data.in = 'E=1.4 F=5')
    """

    if len(tokens) != len(values):
        raise AssertionError("Error: tokens size is != values size!")
    check_param(tokens, list)
    check_param(values, list)

    inplace = False
    if outfile == None or infile == outfile:
        inplace = True
        outfile = infile + ".temporary_outfile"

    infile_handle = open(infile, 'rb')
    outfile_handle= open(outfile, 'wb')

    regex_tokens = []
    found_tokens = []
    for token in tokens:
        regex_tokens.append(re.compile(token))
        found_tokens.append(False)

    for line in infile_handle:
        line = line.decode()
        i = 0
        for regex_token in regex_tokens:
            found = regex_token.search(line)
            while found != None:
                found_tokens[i] = True
                line = line.replace(found.group(0), str(values[i]))
                found = regex_token.search(line)
            i += 1

        outfile_handle.write(line.encode())

    infile_handle.close()
    outfile_handle.close()

    for token, found_token in zip(tokens, found_tokens):
        if found_token == False:
            EOFError("Error: token '" + token + "' not found!")

    if inplace:
        os.remove(infile)
        shutil.move(outfile, infile)



def execute(args, workdir=None, is_shell=False, shell_exe=None, hide_win=True,
            check_exit_code=True, get_stdout=False, get_stderr=False):
    """
    Launch an external processus in a clean way.

    args: a string representing the command.
        e.g.: "ls -l /home"
    workdir: set the current directory of the executed command
    is_shell: if set to True, the command is started in a shell (bash).
        default: False.
    shell_exe: path to the shell. e.g. /bin/zsh.
        default: None -> /bin/bash.
    check_exit_code: if set to True: raise a RuntimeError exception if return
        code of process != 0
    get_stdout: whether standard output of the command is returned
    get_stderr: whether standard error of the command is returned

    return:
        the exit code of the command
        the stdout data if get_stdout parameter is set
        the stderr data if get_stderr parameter is set

    example:
        ret, stdout = execute('/bin/ls /bin/kill', get_stdout=True).
        # => ret = 0, stdout = '/bin/kill'

    exception: RuntimeError
    """

    # split args if not in a shell before passing it to os.execvp()
    if not is_shell:
        process_args = shlex.split(args)
    else:
        process_args = args

    # override startupinfo to hide windows console
    startupinfo = None
    #if hide_win and 'win' in sys.platform:
    #    startupinfo = subprocess.STARTUPINFO()
    #    # fixme: do not work
    #    #startupinfo.dwFlags = subprocess.STARTF_USESHOWWINDOW
    #    #startupinfo.wShowWindows = subprocess.SW_HIDE

    stdout = None
    stderr = None
    if get_stdout:
        stdout = subprocess.PIPE
    if get_stderr:
        stderr = subprocess.PIPE
    p = subprocess.Popen(process_args, shell=is_shell, cwd=workdir,
                         executable=shell_exe, stdout=stdout, stderr=stderr,
                         startupinfo=startupinfo)
    if get_stdout or get_stderr:
        stdout_data, stderr_data = p.communicate()
    ret = p.wait()

    # check return code
    if check_exit_code and ret != 0:
        raise RuntimeError('Command (' + args + ') returned exit code ' +
                           str(ret))

    if get_stdout and get_stderr:
        return ret, stdout_data, stderr_data
    elif get_stdout:
        return ret, stdout_data
    elif get_stderr:
        return ret, stderr_data
    return ret



def get_regex(filename, patterns):
    """
    get results from a file using regex

    filename: the file to parse
    patterns: a list of patterns that will permit to get the values.
        \R and \I can be used to match float and integer.
        \s can be used to match any whitespace character (= [ \t\n\r\f\v])
        ! \S can be used to match any non-whitespace character !
        The value to be searched must be surrounded by '(' and ')' (see example).

    return: a list of values corresponding to each pattern.
        If nothing has been found, the corresponding value is set to None.

    exception:
        AssertionError: parameters badly set
        EOFError: no value found

    example:
        # simple (results.out = '@E=-9.5E8')
        Y = get_regex(filename='results.out',
                      patterns=['@E=(\R)'])
        # Y = [-9.5E8]
    """

    if not isinstance(patterns, list) or len(patterns) == 0:
        raise AssertionError("error: patterns parameter badly set!")

    results = [None] * len(patterns)

    re_patterns = []
    for pattern in patterns:
        # OT-like shortcuts
        pattern = pattern.replace('\R',
                                 '[+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?')
        pattern = pattern.replace('\I',
                                 '[+-]? *\d+')

        re_patterns.append(re.compile(pattern))


    file_handle = open(filename, 'rb')

    for line in file_handle:
        line = line.decode()
        i = 0
        for re_pattern in re_patterns:
            match = re_pattern.search(line)
            if match:
                results[i] = float(match.group(1))
            i += 1

    file_handle.close()

    for result, pattern in zip(results, patterns):
        if result == None:
            raise EOFError('error: no pattern (' + pattern + ') found!')

    return results



def get_real_from_line(line):
    """
    private func

    Try to get a real value from the beginning of a text line.

    return: The float value if a real (or an integer) is found. Raise an
      exception if nothing found.
    """
    result = None
    #print "token=" + str(token) + ', skip_token=' + str(skip_token) + ','\
    #        'skip_line=' + str(skip_line) + ', skip_col=' + str(skip_col)

    # \S*: spaces are allowed at the beginning of the real
    real_regex = '\s*[+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?'
    #real_regex = '[+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?'
    re_real = re.compile(real_regex)

    # get the value
    match = re_real.match(line)
    if match:
        result = float(line[match.start():match.end()])

    if result == None:
        raise EOFError('error: real not found at the beginning of this line: '
                       '(' + line + ')!')

    return result



def read_line(handle, seek=0):
    """
    private func

    get next line.
    seek: if seek < 0: stop reading seek before the end of the file
    """
    line = handle.readline().decode()

    if seek < 0 and line != '':
        if handle.tell() > -seek:
            if debug: sys.stderr.write( 'line before cut ->' + line + '<-\n' )

            # cut the end of the line
            line = line[:len(line) - (handle.tell()+seek)]
            # after cut, if the line is empty, add a \n to behave like
            # readline() python function
            if line == '':
                line = '\n'
            # any further read will return ''
            handle.seek(0, os.SEEK_END)

            if debug: sys.stderr.write( 'line after cut ->' + line + '<-\n' )

    if debug: sys.stderr.write( 'read_line: ->' + line + '<-\n' )

    return line



def get_line_col(filename, skip_line=0, skip_col=0, seek=0):
    """
    skip_line: number of lines skipped
        If skip_line < 0: count lines backward from the end of the file.
        Be careful: a last empty line is taken into account too!
        Default: 0: no line skipped
    skip_col: number of columns skipped from the beginning or end of the line.
        If skip_col < 0: count col backward from the end of the line.
        Default: 0: no column skipped
    seek: if > 0, consider the file starts at pos seek.
        if < 0, consider the file ends at pos -seek (and NOT (end-(-seek))!).
        Default: 0: consider the whole file.
    """
    check_param(filename, str)
    check_param(skip_line, int)
    check_param(skip_col, int)
    check_param(seek, int)

    handle = open(filename, 'rb')
    if seek > 0:
        handle.seek(seek)

    if debug: sys.stderr.write( 'get_line_col(skip_line=' + str(skip_line) +\
       ', skip_col=' + str(skip_col) + ', seek=' + str(seek) + ')\n' )

    # skip line backward
    if skip_line < 0:
        # cache position of each beginning of line
        # last elt of the list is the last inserted
        lines_cache = []
        # determine number of elt to cache
        lines_cache_size = -skip_line

        # build lines cache
        previous_pos = handle.tell()
        line = read_line(handle, seek)
        while line != '':
            # append to cache
            lines_cache.append(previous_pos)
            if len(lines_cache) > lines_cache_size:
                # todo: list not optimized to del before the end
                del lines_cache[0]
            previous_pos = handle.tell()

            line = read_line(handle, seek)
            if debug: sys.stderr.write( 'lines_cache: ' + str(lines_cache)+'\n' )

        if len(lines_cache) < lines_cache_size:
            err_msg = 'error: the file has less than ' + str(lines_cache_size) +\
                      ' lines!'
            handle.close()
            raise EOFError(err_msg)
        else:
            handle.seek(lines_cache[0])
            line_found = read_line(handle, seek)
            if debug: sys.stderr.write( 'line found: ->' + line_found + '<-\n' )
    # skip line forward
    else:
        while skip_line >= 0:
            line = read_line(handle, seek)
            if skip_line > 0 and line == '':
                handle.close()
                raise EOFError('error: the file has less lines than "skip_line"!')
            skip_line -= 1
        line_found = line

    handle.close()

    # get the good col
    if skip_col != 0:
        try:
            line_found = line_found.split()[skip_col]
        except:
            raise EOFError('error: value not found on this line: (' +
                           line_found + ')!')

    # get the value
    result = get_real_from_line(line_found)

    return result



def get_value(filename, token=None, skip_token=0, skip_line=0, skip_col=0):
    """
    get one result from a file using token and/or various offsets

    This function is optimized to be rather fast and takes low memory on human
    readable file.

    filename: a file that will be parsed
    token: a regex that will be searched. The value right after the token is
        returned.
        Default: None (no token searched)
    skip_token: the number of tokens that will be skipped before getting the
        value. If set to != 0, the corresponding token parameter must not be
        equal to None.
        If skip_tokens < 0: count tokens backward from the end of the file.
        Default: 0: no token skipped
    skip_line: number of lines skipped from the token found.
        If corresponding token equal None, skip from the beginning of the file.
        If corresponding token != None, skip from the token.
        If skip_line < 0: count lines backward from the token or from the end
        of the file. Be careful: a last empty line is taken into account too.
        Default: 0: no line skipped
    skip_col: number of columns skipped from the token found.
        If corresponding token = None, skip words from the beginning of the line.
        If corresponding token != None, skip words from the token.
        If skip_col < 0: count col backward from the end of the line or from
        the token.
        Default: 0: no column skipped

    return: a real value.

    exception:
        AssertionError: parameters badly set
        EOFError: no value found

    examples:
        # simple token (results.out = ' @Y1=2.6, Y2=  -6.6E56')
        Y = CouplingTools.get_value(filename='results.out',
                                    token='@Y1=',
                                    )
        # Y = 2.6

        # using token ans skip_tokens (results.out =
                                       '@Y1=5.9 @Y1=2.6 # temperature 2')
        Y = CouplingTools.get_value(filename='results.out',
                                    token='@Y1=',
                                    skip_token=1,
                                    )
        # Y = 2.6

        # using column & line (results.out = ' 2.6 3.5 6.8  9.0
                                               3.6 7.5 6.9  9.8  ')
        Y = CouplingTools.get_value(filename='results.out',
                                    skip_line=1,
                                    skip_col=-2,
                                    )
        # Y = 6.9

        # mix it (results.out = ' 2.6  Y1=3.5 Y1=6.8 9.0
                                 temperatures:
                                 88.8 99.9 # ')
        Y = CouplingTools.get_value(filename='results.out',
                                    token='Y1=',
                                    skip_token=1,
                                    skip_line=0,
                                    skip_col=1,
                                    )
        # Y = 9.0
    """

    if debug: sys.stderr.write( "get_value(token=" + str(token) + \
       ', skip_token=' + str(skip_token) + \
       ', skip_line=' + str(skip_line) + \
       ', skip_col=' + str(skip_col) + ')\n' )

    # check parameters
    check_param(filename, str)
    if token != None:
        check_param(token, str)
    elif skip_token != 0:
        raise AssertionError("error: skip_token parameter needs token "
                             "parameter to be set!")
    check_param(skip_token, int)
    check_param(skip_line, int)
    check_param(skip_col, int)

    result = None
    if not token:
        result = get_line_col(filename, skip_line, skip_col)
    else:
        handle = open(filename, 'rb')

        re_token = re.compile(token)

        # store previous begin of line pos
        line_pos = handle.tell()

        # store token pos
        if skip_token >= 0:
            token_pos = None
        else:
            token_pos_cache = []

        line = handle.readline().decode()
        while line != '':
            for token_match in re_token.finditer(line):
                if skip_token > 0:
                    # token found but it's not the good one
                    skip_token -= 1
                elif skip_token == 0:
                    # token found
                    token_pos = line_pos + token_match.start()
                    if debug:
                        sys.stderr.write( 'skip_token == 0, line: ' + line+'\n' )
                    break
                else:
                    # token wanted in revert order: we first cache them all
                    token_pos_cache.append(line_pos + token_match.start())

            if skip_token >= 0 and token_pos != None:
                # token found
                break

            line_pos = handle.tell()
            line = handle.readline().decode()

        # get the token from the cache
        if skip_token < 0 and len(token_pos_cache) >= -skip_token:
            token_pos = token_pos_cache[skip_token]

        if token_pos == None:
            handle.close()
            raise EOFError('error: no token (' + token + ') was found!')

        if skip_line == 0 and skip_col == 0:
            # get the real right after the token
            handle.seek(token_pos + len(token))
            line = handle.readline().decode()
            if debug: sys.stderr.write( 'first token, line_found: ' + line+'\n' )
            result = get_real_from_line(line)
            handle.close() # fixme: no multiple close?
        else:
            # get the real by skipping from the token
            if skip_line < 0 or (skip_line == 0 and skip_col < 0):
                # search before the token
                seek_pos = -token_pos
                skip_line -= 1 # skip line from the token
            else:
                # search after the token
                seek_pos = token_pos + len(token)
            handle.close()
            result = get_line_col(filename, skip_line, skip_col, seek_pos)

    return result



def get(filename, tokens=None, skip_tokens=None, skip_lines=None, skip_cols=None):
    """
    get results from a file using token and/or various offsets

    filename: a file that will be parsed

    Each following parameters must be a list. The first element of each list
    will be used to get the first value, the 2nd ...
    If the parameter is set to None, the list is set to the default value of
    get_value(), i.e skip_lines=None => [0, 0, ...].

    tokens:
    skip_tokens:
    skip_lines:
    skip_cols:
        see get_value() function

    return: a list of real values.

    exception:
        AssertionError: parameters badly set
        EOFError: no value found

    examples:
        # simple token (results.out = ' @Y1=2.6, Y2=  -6.6E56')
        Y = CouplingTools.get(filename='results.out',
                              tokens=['@Y1=', 'Y2='],
                              )
        # Y = [2.6, -6.6E56]

        # mix (results.out = ' 2.6  Y1=3.5 Y1=6.8 9.0
                                 temperatures:
                                 88.8 99.9 # ')
        Y = CouplingTools.get(filename='results.out',
                              tokens=['Y1=', None],
                              skip_tokens=[1, 0],
                              skip_lines=[0, 2],
                              skip_cols=[1, 1],
                              )
        # Y = [9.0, 99.9]
    """


    # test parameters and determine the number of value to return
    nb_values = None
    if tokens != None:
        nb_values = len(tokens)

    err_msg = "error: skip_tokens parameter needs tokens parameter to be set!"
    if skip_tokens != None:
        if nb_values == None:
            raise AssertionError(err_msg)
        elif nb_values != len(skip_tokens):
            raise AssertionError(err_msg)

    if skip_lines != None:
        if nb_values == None:
            nb_values = len(skip_lines)
        elif nb_values != len(skip_lines):
            raise AssertionError(err_msg)

    if skip_cols != None:
        if nb_values == None:
            nb_values = len(skip_cols)
        elif nb_values != len(skip_cols):
            raise AssertionError(err_msg)

    if nb_values == None:
        raise AssertionError("error: no parameters have been set!")

    # init properly every list
    if tokens == None:
        tokens = [None] * nb_values
    if skip_tokens == None:
        skip_tokens = [0] * nb_values
    if skip_lines == None:
        skip_lines = [0] * nb_values
    if skip_cols == None:
        skip_cols = [0] * nb_values

    results = [None] * nb_values

    # possible optimization: get values in one pass
    for n in range(nb_values):
        results[n] = get_value(filename,
                               tokens[n],
                               skip_tokens[n],
                               skip_lines[n],
                               skip_cols[n])

    return results
