#! /usr/bin/env python
# -*- coding: utf8 -*-

"""
define how to compute a point :
simple example of wrapper:
compute results only with python
"""
import time

def _exec( X ):
    """ define how to compute a point """
    #work_factor = 10
    #k = 1000*1000*work_factor
    #while k != 0:
    #    k = k - 1

    return [ X[0] * X[1] ]
