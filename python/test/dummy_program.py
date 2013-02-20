#! /usr/bin/env python
# -*- coding: utf8 -*-

import time
import sys

inFile = sys.argv[1]

# result
exec(compile(open(inFile).read(), inFile, 'exec'))

# dummy work
start_time = time.time()
while time.time() - start_time < T:
    k = 1000*100
    while k > 0:
        k = k - 1
#time.sleep(T)

Z = F * E

# F = 666 make the wrapper fail points 2
if F == 666:
    if E == 3:
        exit(1)

print( Z )
