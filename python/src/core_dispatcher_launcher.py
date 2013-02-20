# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  core_dispatcher_launcher.py
# @brief launch the core dispatcher on a sample.
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

import core_dispatcher
import wrapper_data

import os
import sys


if len(sys.argv) == 2:
    host_in_file = sys.argv[1]
else:
    print( 'host_in_file args recquired! exit.' )
    exit(1)

# ensure the core dispatcher will find the in_file
moduledir = os.path.dirname( os.path.realpath( __file__ ) )
os.chdir(moduledir)

wd_host_in = wrapper_data.WrapperDataHostIn()
wd_host_in.set_filename(host_in_file)
wd_host_in.read()

wd_host_out = wrapper_data.WrapperDataHostOut(remote=True)
wd_host_out.set_dirname(wd_host_in.workdir)
wd_host_out.set_hostname(wd_host_in.hostname)

dispatcher = core_dispatcher.CoreDispatcher(wd_host_in, wd_host_out)
dispatcher.exec_sample()
