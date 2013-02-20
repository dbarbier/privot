#! /usr/bin/env python
# -*- coding: utf8 -*-

import openturns as ot

import sys
import shutil
import os
import argparse
import threading
import tempfile

# multicore example:
# ./t_distributed_python_wrapper.py --sample-size 10 --test local
#
# multihost example (need working ssh server):
# ./t_distributed_python_wrapper.py --sample-size 100 --test remote --hosts host1 host2

# todo: add ctrc-c test

parser = argparse.ArgumentParser(description="test openturns "
                                 "distributed-python-wrapper",
                                 add_help=False)
parser.add_argument('--help', action='store_const', const=[],
                    help='show this help message and exit')

parser.add_argument('--debug', '-d', action='store_true',
                    help='print openturns debug\'log')
parser.add_argument('--hosts', '-h', nargs='+')
parser.add_argument('--test', '-t', nargs=1,
                    help='test_type (remote, local)')
parser.add_argument('--point', action='store_true',
                    help='test with one point rather than a sample')
parser.add_argument('--analytical', action='store_true',
                    help='test without separate workdir')
parser.add_argument('--cleanup', '-c',  nargs=1,
                    help='cleanup workdirs')
parser.add_argument('--error', '-e', action='store_true',
                    help='make error')
parser.add_argument('--tmpdir', '-w', nargs=1,
                    help='tmpdir')
parser.add_argument('--sample-size', '-s', nargs=1,
                    help='number of points to compute')
parser.add_argument('--work-time', '-p', nargs=1,
                    help='number of second of computing per point')
parser.add_argument('--nb-output', '-n', nargs=1,
                    help='number of output variable')

args = parser.parse_args()
#print "args: " + str(args)

# print help if asked
if args.help != None:
    printHelp(parser)

if args.debug:
    ot.Log.Show( ot.Log.Flags() | ot.Log.DBG )

hosts = None
if args.hosts != None:
    hosts = args.hosts

test_type = "local"
if args.test != None:
    test_type = args.test[0]

test_point = False
if args.point:
    test_point = True

test_analytical = False
if args.analytical:
    test_analytical = True

cleanup = "ok"
if args.cleanup != None:
    cleanup = args.cleanup[0]

make_error = args.error

tmpdir = ""
if args.tmpdir != None:
    tmpdir = args.tmpdir[0]

sample_size = 5
if args.sample_size != None:
    sample_size = int(args.sample_size[0])

work_time = 1
if args.work_time != None:
    work_time = float(args.work_time[0])

nb_output = 1
if args.nb_output != None:
    nb_output = int(args.nb_output[0])


print(  "test_type:" + test_type + ",  test_point:" + str(test_point) + \
        ",  test_analytical:" + str(test_analytical) + \
        ",  cleanup:" + cleanup + ",  make_error:" + \
        str(make_error) + ",  tmpdir:" + tmpdir +  ",  sample_size:" + \
        str(sample_size) + ",  work_time:" + str(work_time) + ",  hosts:" + \
        str(hosts) + ", nb_output:" + str(nb_output))



# uncomment following line to show wanted logs
#ot.Log.Show( ot.Log.ALL )
#
#ot.Log.Show( ot.Log.Flags() | ot.Log.WRAPPER )
#ot.Log.Show( ot.Log.Flags() | ot.Log.DBG )
# print compute progression :
ot.Log.Show( ot.Log.Flags() | ot.Log.INFO )

# set number of thread = number of job concurrently started
#ResourceMap.Set("parallel-threads", "6")
#print "Nb of thread of localhost: ", ot.ResourceMap.Get("parallel-threads")

script_dir = os.path.dirname( os.path.realpath( __file__ ) )
program_wrapper = script_dir + os.sep + "dummy_program_wrapper.py"
func_wrapper = script_dir + os.sep + "dummy_func_wrapper.py"
program = script_dir + os.sep + "dummy_program.py"

dist_func = ot.OpenTURNSDistributedPythonFunction(n_input=4,
                                                  n_output=nb_output,
                                                  wrapper_file=program_wrapper,
                                                  hosts=hosts,
                                                  cleanup=cleanup,
                                                  files_to_send=[program],
                                                  tmpdir=tmpdir)

if test_analytical:
    dist_func.set_separate_workdir(False)

if 'win' not in sys.platform:
    # change group pid in order to avoid wrapper_launcher destroying parent process
    # when interrupting
    os.setpgid(0, 0)

model = ot.NumericalMathFunction( dist_func )

# create sample
inS = ot.NumericalSample(sample_size, 4)
if not make_error:
    F=2
else:
    F=666
for i in range(sample_size):
    inS[i,0] = i + 1
    inS[i,1] = F
    inS[i,2] = work_time
    inS[i,3] = nb_output

print( 'Compute' )
if make_error:
    try:
        outS = model( inS )
    except:
        print( '====== An error raised, that\'s ok ======' )
    else:
        Exception("ERROR: no exception!")

else:
    if not test_point:
        outS = model( inS )
    else:
        outS = []
        outS.append( model( inS[0] ) )
        sample_size = 1

    print( 'Results' )
    if sample_size < 64 and nb_output <= 2:
        print( outS )

    # 01 comes from deterministic value, check output values are ok
    sum = 0
    ok = 1
    for i in range(sample_size):
        z = outS[i][0]
        valid_value = (i + 1) * F
        if z != valid_value:
            ok = 0
            print( 'point ' + str(i) + ' incorrect, got value: ' + str(z) + \
                    ', valid value is ' + str(valid_value) )

    if ok:
        print( 'Results are OK.' )
    else:
        print ('!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!' )
        exit(1)


# check existing or not workdir
check_workdir_beg = None
check_workdir_end = None

# guess workdir
workdir = dist_func.wd_hosts_in.remote_tmpdir
if not workdir:
    workdir = tempfile.gettempdir()
workdir += os.sep + dist_func.wd_hosts_in.workdir_basename

if cleanup == "no":
    check_workdir_beg = 0
    check_workdir_end = sample_size

elif cleanup == "ok" and sample_size > 2 and make_error:
    check_workdir_beg = 2
    check_workdir_end = 2

if check_workdir_beg != None:
    # todo: check that 0..n directory are there on remote nodes too?
    if test_type == "local":
        workdirs = range(check_workdir_beg, check_workdir_end)
        dirs = os.listdir(workdir)
        for i in workdirs:
            if str(i) not in dirs:
                err_msg = "The directory " + workdir + os.sep +\
                        str(i) + " was not found!"
                print (err_msg)
                raise Exception(err_msg)

    shutil.rmtree(workdir)
    print( 'Workdir found. Cleaned.' )
else:
    if os.path.exists(workdir):
        raise Exception('The workdir was not cleaned!')
    print( 'Workdir not found: ok.' )

# print an empty line in order to detect exception
print ('')
