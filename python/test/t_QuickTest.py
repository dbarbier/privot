#! /usr/bin/env python

# check that python can find the OpenTURNS package
print '1: openturns module      ',
try:
    import os
    os.environ['OPENTURNS_PYTHON_SILENT'] = "1"
    import openturns as ot
    print 'OK'
except:
    print 'no'

# check that python can find the ViewImage facility
# If it fails, check that matplotlib package is installed
print '2: Viewer                ',
try:
    import openturns.viewer
    print 'OK'
except:
    print 'no'

# check that OpenTURNS can run R
# It should produce two files named testDraw.png and testDraw.eps
print '3: R                     ',
try:
    g = ot.Normal().drawPDF()
    g.draw("testDraw")
    print 'OK'
except:
    print 'no'

# check that rot package is installed
print '4: R.rot                 ',
try:
    lm = ot.LinearModelFactory().build(ot.Normal(2).getSample(10), ot.Normal().getSample(10))
    print 'OK'
except:
    print 'no'

# check XML support
print '5: XML support           ',
try:
    storageManager = ot.XMLStorageManager('myFile.xml')
    print 'OK'
except:
    print 'no'

# check that the sample wrappers are accessible
print '6: wrappers              ',
try:
    f = ot.NumericalMathFunction("poutre")
    print 'OK'
except:
    print 'no'
