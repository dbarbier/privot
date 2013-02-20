#! /usr/bin/env python
# -*- coding: utf8 -*-

# test that the python template is fine

import sys
import os


if len(sys.argv) == 1:
    # windows native
    template_dir = "WrapperTemplates/wrapper_python_distributed"
else:
    # linux cross compile
    template_dir = sys.argv[1] + \
            "/../../wrappers/WrapperTemplates/wrapper_python_distributed"
if 'win' in sys.platform:
    os.environ['PATH'] += ";" + os.getcwd()
os.chdir(template_dir)
start_script = sys.executable + " ot_script.py"
os.system(start_script)
