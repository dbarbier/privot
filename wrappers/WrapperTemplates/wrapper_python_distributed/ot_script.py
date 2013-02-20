#! /usr/bin/env python

import openturns as ot

import sys

#hosts = ["localhost", "node-3", "node-4"]
hosts = None
print( 'hosts used:  ' + str(hosts) )

# number of points to compute
sample_size = 4
if len(sys.argv) > 1:
    sample_size = int(sys.argv[1])

# uncomment following line to show wanted logs
#ot.Log.Show( ot.Log.ALL )
#
#ot.Log.Show( ot.Log.Flags() | ot.Log.WRAPPER )
#ot.Log.Show( ot.Log.Flags() | ot.Log.DBG )
# print compute progression :
ot.Log.Show( ot.Log.Flags() | ot.Log.INFO )

model = ot.DistributedPythonFunction(n_input=2, 
                                     n_output=1, 
                                     wrapper_file='program_wrapper.py', 
                                     hosts=hosts, 
                                     #scheduler="PBS",
                                     cleanup='ok', 
                                     files_to_send=['input_template', 
                                                    'program.py'])
# compute single point
#inP = NumericalPoint( (101,255) )
#print inP
#outP = model( inP )
#print outP

# create sample
in_sample = ot.NumericalSample(sample_size, 2)
# put deterministic values
F=2
for i in range(sample_size):
    in_sample[i,0] = i + 1
    in_sample[i,1] = F


print( '\nCompute' )
outP = model( in_sample )


print( '\nResults' )
if sample_size < 64:
    print( outP )


# check output values are ok
print('')
sum = 0
ok = 1
for i in range(sample_size):
    z = outP[i][0]
    valid_value = (i + 1) * F
    if z != valid_value:
        ok = 0
        print( 'point ' + str(i) + ' incorrect, got value: ' + str(z) + \
                ', valid value is ' + str(valid_value) )

if ok:
    print( 'Results are OK.' )
else:
    print( '! ERROR !' )


