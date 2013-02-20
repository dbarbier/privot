#! /usr/bin/env python
# -*- coding: utf8 -*-

"""
define how to compute a point :
make the glue to give to the actual program the input variables and take from
the results the output variables
"""

import coupling_tools


external_program = 'python dummy_program.py'

def _exec( X ):
    """ define how to compute a point """

    in_file = 'input_example.py'
    file = open(in_file, 'w')
    file.write('E=' + str(X[0]) + '\n')
    file.write('F=' + str(X[1]) + '\n')
    file.write('T=' + str(X[2]) + '\n')
    file.close()

    # work, work, work
    rc, cmd_stdout, cmd_stderr = \
            coupling_tools.execute(external_program + ' ' + in_file,
                                   get_stdout=True,
                                   get_stderr=True)


    # get out value from external_program stdout
    try:
        Y = float(cmd_stdout)
    except:
        import traceback
        ex_info = traceback.format_exc()
        raise Exception('unable to get the result (' + ex_info + ')!"'
                        ' (\ncmd stdout: ' + cmd_stdout + '\ncmd stderr: ' +
                        cmd_stderr + '\n)')

    nb_output = int(X[3])
    if X[3] > 1:
        out_sample = [Y] * nb_output
    else:
        out_sample = [Y]

    return out_sample
