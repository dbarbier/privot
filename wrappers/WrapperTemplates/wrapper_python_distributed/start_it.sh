#!/bin/bash

# set PYTHONPATH to OpenTURNS if needed
PYOT=$(echo ../../../build/install/lib/python*/site-packages)
export PYTHONPATH=$PYOT:$PYTHONPATH

# start the script
python ot_script.py
