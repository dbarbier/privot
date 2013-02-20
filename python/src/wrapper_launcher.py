# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  wrapper_launcher.py
# @brief Launch the wrapper on a point and write the result in a result file.
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

import traceback
import sys
import os
import time
import threading

# ensure the global workdir is searched for wrapper_data
sys.path.append(os.getcwd() + os.sep + '..')

import wrapper_data


class ParentIsAlive(threading.Thread):
    """ stop current process (and it's children) if parent dies """
    def __init__(self, parent_pid, condition):
        threading.Thread.__init__(self)
        self.parent_pid = parent_pid
        self.condition = condition
        self.again = True

    def run(self):
        self.condition.acquire()
        while self.again:
            if os.getppid() != self.parent_pid:
                # kill children's too (do not work on windows)
                os.system('kill -- -' + str(os.getpgid(0)))
            self.condition.wait(1)
        self.condition.release()

parent_checker = None
parent_pid = None
if len(sys.argv) == 2:
    parent_pid = int(sys.argv[1])
    condition = threading.Condition()

# ensure the current workdir is searched for user_wrapper
sys.path.append(os.getcwd())

wd_core_out = wrapper_data.WrapperDataCoreOut()

try:

    import user_wrapper

    wd_core_in = wrapper_data.WrapperDataCoreIn()
    wd_core_in.read()

    # parent_pid == 0: do not launch it on localhost cause it will kill the
    # initial python script!
    if parent_pid:
        parent_checker = ParentIsAlive(parent_pid, condition)
        parent_checker.start()

    wd_core_out.point = user_wrapper._exec(wd_core_in.point)


except:
    wd_core_out.err_msg = "ERROR during wrapper execution:\n" + traceback.format_exc()

if parent_checker != None:
    parent_checker.again = False
    condition.acquire()
    condition.notify()
    condition.release()

wd_core_out.write()

if wd_core_out.err_msg != '':
    print( wd_core_out.err_msg )
    exit(1)
