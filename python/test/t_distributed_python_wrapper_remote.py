#! /usr/bin/env python
# -*- coding: utf8 -*-

import sys
import os

if len(sys.argv) == 1:
    # start test manually
    test_path = os.getcwd()
else:
    # started from CTest
    test_path = sys.argv[1]
os.chdir(test_path)
script_path = test_path + os.sep + "t_distributed_python_wrapper_std.py"
os.system("python " + script_path + " " + test_path + " remote")
