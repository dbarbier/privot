#! /usr/bin/env python
# -*- coding: utf8 -*-
# simple program

import sys

# get input
inFile = sys.argv[1]
exec(compile(open(inFile).read(), inFile, 'exec'))

# compute
Z = F * E

# do sthg
i = 10*1000*1000
while i != 0:
    i = i - 1

# write output
outFile = 'output.py'
file = open( outFile, 'w' )
file.write( 'Z=' + str(Z) + '\n' )
file.close()


