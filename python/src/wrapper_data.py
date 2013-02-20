# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file wrapper_data.py
# @brief define / read / write distributed python wrapper data (mostly pickle
#   files)
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


import pickle # don't use cpickle cause it don't work with file > 1Go
import os
import threading
import time

script_name = "wrapper_data.py"


def get_data(filename):
    """
    get the point or sample from a data file

    filename: the file containing data
    return: None if nothing found, a point or a sample otherwise

    i.e. :
    a_point = get_data('/tmp/wrapper/0/core_out.pkl')
    a_sample = get_data('host_in_node05.pkl')
    """

    ret = None
    header_found = False

    wds = [WrapperDataCoreOut(), WrapperDataCoreIn(), WrapperDataHostIn(),
           WrapperDataHostOut()]
    for wd in wds:
        wd.handle = open(filename, 'rb')
        head_id = WrapperData.load(wd)
        if head_id != wd.head_id:
            wd.handle.close()
        else:
            header_found = True
            wd.handle.seek(0)
            ret = wd.get_data()
            if not ret:
                print( 'No data has been found in this ' + \
                        str(wd.__class__.__name__) + ' file' )
            break
    if not header_found:
        print( 'This file is not a WrapperData file!' )

    return ret



class WrapperData():
    """
    data that will travel between hosts
    """

    file_suffix = '.pkl'

    ## data type
    # file type (core, host, hosts)
    flag_head = 'H'

    def __init__(self):
        self.hostname = None
        self.filename = None
        self.dirname = None

        self.handle = None


    def set_filename(self, filename):
        self.filename = filename

    def get_filename(self):
        if self.filename == None:
            # create default filename
            self.filename = self.head_id
            if self.hostname:
                self.filename += '_' + self.hostname
            self.filename += self.file_suffix
        return self.filename

    def set_dirname(self, dirname):
        self.dirname = dirname

    def get_dirname(self):
        return self.dirname

    def get_fullname(self):
        if self.dirname:
            fullname = self.dirname + os.sep + self.get_filename()
        else:
            fullname = self.get_filename()
        return fullname


    def open_file(self, mode='rb'):
        if self.handle == None:
            self.handle = open(self.get_fullname(), mode)

    def close_file(self):
        self.handle.close()


    def dump(self):
        """ serialize data """
        pickle.dump([self.flag_head, self.head_id], self.handle)

    def load(self):
        """
        unserialize data
        return: head_id, 'None' if no header found
        """
        data = pickle.load(self.handle)
        flag_head = data[0]
        if flag_head == self.flag_head:
            head_id = data[1]
        else:
            head_id = None
        return head_id


#    def write(self, string):
#        """ write string to the file """
#        self.handle = open(self.get_fullname(), 'wb')
#        self.handle.write(string)
#        self.handle.close()
#
#    def read(self):
#        """ read a file and return its content """
#        self.handle = open(self.get_fullname(), 'rb')
#        string = self.handle.read()
#        self.handle.close()
#        return string
#


class WrapperDataCoreIn(WrapperData):
    """
    used to give the point to one core (or thread)
    """

    head_id = "core_in"

    def __init__(self):
        WrapperData.__init__(self)
        self.point = None
        #self.set_dirname(dirname)

    def write(self):
        self.open_file('wb')
        WrapperData.dump(self)
        pickle.dump(self.point, self.handle)
        self.close_file()

    def read(self):
        self.open_file()
        WrapperData.load(self)
        self.point = pickle.load(self.handle)
        self.close_file()


    def get_data(self):
        """ get only data """
        self.read()
        if self.point:
            print( 'A ' + self.head_id + ' point has been found.' )
        return self.point



class WrapperDataCoreOut(WrapperDataCoreIn):
    """
    used to get the point computed by one core (or thread)
    """

    head_id = "core_out"

    def __init__(self):
        WrapperDataCoreIn.__init__(self)
        self.err_msg = ''

    def write(self):
        self.open_file('wb')
        WrapperData.dump(self)
        pickle.dump(self.point, self.handle)
        pickle.dump(self.err_msg, self.handle)
        self.close_file()

    def read(self):
        self.open_file()
        WrapperData.load(self)
        self.point = pickle.load(self.handle)
        self.err_msg = pickle.load(self.handle)
        self.close_file()



class WrapperDataHostIn(WrapperData):
    """
    used to give sample for one host
    """

    head_id = "host_in"

    def __init__(self, hostname=None):
        WrapperData.__init__(self)
        self.hostname = hostname
        self.sample = None
        # first_id: is used to get the num order of the point of the whole
        # todo: begin, end instead of first_id
        self.first_id = 0
        #
        self.workdir_basename = None
        # fixme: wordkir vs workdir_basename not clear
        self.workdir = None
        self.tmpdir = None
        self.remote_tmpdir = None
        self.n_cores = 0
        self.files_to_send = []
        self.separate_workdir = True
        self.cleanup = 'ok'

    def copy(self, wd_host_in):
        """ copy the object """
        # not copied
        #self.hostname
        #self.sample
        #self.first_id
        self.workdir_basename = wd_host_in.workdir_basename
        self.workdir = wd_host_in.workdir
        self.tmpdir = wd_host_in.tmpdir
        self.remote_tmpdir = wd_host_in.remote_tmpdir
        self.n_cores = wd_host_in.n_cores
        self.files_to_send = wd_host_in.files_to_send
        self.separate_workdir = wd_host_in.separate_workdir
        self.cleanup = wd_host_in.cleanup


    def write(self):
        """ Store the object to a file. """
        self.open_file('wb')

        WrapperData.dump(self)
        pickle.dump(self.sample, self.handle)
        pickle.dump(self.first_id, self.handle)
        pickle.dump(self.hostname, self.handle)
        pickle.dump(self.workdir_basename, self.handle)
        pickle.dump(self.workdir, self.handle)
        pickle.dump(self.remote_tmpdir, self.handle)
        pickle.dump(self.n_cores, self.handle)
        pickle.dump(self.separate_workdir, self.handle)
        pickle.dump(self.cleanup, self.handle)
        pickle.dump(self.files_to_send, self.handle)

        self.close_file()


    def read(self):
        """ Fill the object from a file. """
        self.open_file()

        WrapperData.load(self)
        self.sample = pickle.load(self.handle)
        self.first_id = pickle.load(self.handle)
        self.hostname = pickle.load(self.handle)
        self.workdir_basename = pickle.load(self.handle)
        self.workdir = pickle.load(self.handle)
        self.remote_tmpdir = pickle.load(self.handle)
        self.n_cores = pickle.load(self.handle)
        self.separate_workdir = pickle.load(self.handle)
        self.cleanup = pickle.load(self.handle)
        self.files_to_send = pickle.load(self.handle)

        self.close_file()


    def get_data(self):
        self.read()
        if self.sample:
            print( 'An input sample has been found.' )
        return self.sample



class WrapperDataHostOut(WrapperData):
    """
    used to get the sample compute by one host
    """

    head_id = "host_out"

    # sample content
    flag_sample = "S"
    # ensure whole previous line has been read
    flag_end    = "END"
    # send finished point msg
    flag_point = "P"
    # send error
    flag_error  = "ERR"
    # send error
    flag_warn  = "W"
    # send msg
    flag_debug  = "D"


    def set_hostname(self, hostname):
        self.hostname = hostname

    def __init__(self, remote=False):
        WrapperData.__init__(self)
        # the out sample
        self.sample = None
        self.sample_timestamp = None

        # list of log msg  [flag, timestamp, data]
        self.logs = []
        # store last log already read
        self.next_log = 0
        # protect logs list
        self.mutex = threading.Lock()

        # whether parameters are passed by file
        self.remote = remote

        # useful for read tail
        self.tail_pos = 0

        self.dump_head = True


    def add_log(self, flag, data, timestamp=time.time()):
        """ thread safe """
        self.mutex.acquire()
        log = [flag, timestamp, data]
        self.logs.append(log)
        if self.remote:
            self.write(log)
        self.mutex.release()


    # todo: disable sending debug log, if disabled by OT
    def add_debug(self, msg):
        self.add_log(self.flag_debug, msg)

    def add_point(self, point_id, compute_time):
        self.add_log(self.flag_point, [point_id, compute_time])

    def add_warn(self, hostname, msg):
        self.add_log(self.flag_warn, msg)

    def add_error(self, point_id, msg):
        self.add_log(self.flag_error, [point_id, msg])


    def write_sample(self):
        """
        thread safe
        """
        self.mutex.acquire()
        self.write([self.flag_sample, time.time(), [self.sample, self.flag_end]])
        self.mutex.release()


    def get_next_log(self):
        """
        thread safe
        get unread log
        return: a log list: [flag, timestamp, data]
                None, if no anymore log to be read
        """
        self.mutex.acquire()
        if self.next_log < len(self.logs):
            log = self.logs[self.next_log]
            self.next_log += 1
        else:
            log = None
        self.mutex.release()
        return log


    def write(self, row):
        """ private """
        self.open_file('wb')

        # must be mutex protected
        if self.dump_head:
            WrapperData.dump(self)
            self.dump_head = False

        pickle.dump(row, self.handle)

        flag = row[0]
        if flag == self.flag_sample:
            # once the sample is given, no output is needed anymore
            self.close_file()
        else:
            self.handle.flush()


    def read(self):
        """
        Try to fill the object from a file.
        If the file has already been read, only the not read part is read.

        return True if new data have been found in the file
        """
        new_data = False

        try:
            self.open_file()

            # do not read previous already read data
            self.handle.seek(self.tail_pos)

            while True:
                row = pickle.load(self.handle)

                flag = row[0]
                if flag == self.flag_sample:
                    timestamp = row[1]
                    data = row[2]
                    if data[1] == self.flag_end:
                        self.sample = data[0]
                        self.sample_timestamp = timestamp
                    else:
                        raise Exception('End flag not found! The sample is'
                                        'perhaps not complete!')

                elif flag == self.flag_error or \
                        flag == self.flag_point or \
                        flag == self.flag_debug:
                    self.logs.append(row)

                # no exception: store read pos
                self.tail_pos = self.handle.tell()

                if flag != self.flag_head:
                    new_data = True

            self.close_file()

        except EOFError as e:
            pass

        return new_data


    def get_data(self):
        self.read()
        if self.sample:
            print( 'An Output sample has been found.' )
        return self.sample



class WrapperDataHostsIn(WrapperDataHostIn):
    """
    used to give sample for a frontal host
    """

    head_id = "hosts_in"

    def __init__(self, hostname=None):

        WrapperDataHostIn.__init__(self, hostname)
        self.scheduler = None
        self.hosts = []
        self.extended_check = False
        self.wrapper_file = None



class WrapperDataHostsOut(WrapperDataHostOut):
    """
    used to get sample from a frontal host
    """

    head_id = "hosts_out"

    def __init__(self, remote=False):

        WrapperDataHostOut.__init__(self, remote=False)
