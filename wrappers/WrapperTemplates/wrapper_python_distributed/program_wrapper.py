#! /usr/bin/env python
# -*- coding: utf8 -*-

"""
define how to compute a point : 
make the glue to give to the actual program the input variables and take from 
the results the output variables
"""

import os
import coupling_tools

external_program = 'python program.py'

def _exec( X ): 
    """ 
    simple example of wrapper where compute is made in an external program
    """

    # write input point
    in_file = 'input.py'
    coupling_tools.replace('input_template', in_file, ['@E', '@F'], X)

    # work, work, work
    coupling_tools.execute(external_program + " " + in_file)

    Y = coupling_tools.get('output.py', tokens=['Z='])

    return Y


# super simple example of wrapper where compute will be made inside it:
# def _exec( X ): 
#     Y = X[0] * X[1]
#     return [Y]


