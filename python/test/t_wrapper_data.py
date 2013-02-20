#! /usr/bin/env python
# -*- coding: utf-8 -*-

from openturns import wrapper_data
import os


print '==  test WrapperDataCoreIn'
# write
wd = wrapper_data.WrapperDataCoreIn()
wd.point = [1, 2]
wd.write()
# read
wd_r = wrapper_data.WrapperDataCoreIn()
wd_r.read()
print 'read ' + str(wd_r.point)
# get data
print wrapper_data.get_data(wd.get_fullname())
# clean
os.remove(wd.get_fullname())


print '== test WrapperDataCoreOut'
# write
wd = wrapper_data.WrapperDataCoreOut()
wd.point = [2, 3]
wd.err_msg = 'an error'
wd.write()
# read
wd_r = wrapper_data.WrapperDataCoreOut()
wd_r.read()
print 'read ' + str(wd_r.point)
print 'read ' + str(wd_r.err_msg)
# get data
print wrapper_data.get_data(wd_r.get_fullname())
# clean
os.remove(wd.get_fullname())


print '== test WrapperDataHostIn'
# write
wd = wrapper_data.WrapperDataHostIn()
wd.hostname = "tata"
wd.sample = [[2, 3, 4], [5]]
wd.first_id = 2
wd.workdir_basename = 'temp132'
wd.workdir = '/temp/tmp56'
wd.tmpdir = '/temp'
wd.remote_tmpdir = '/temp'
wd.n_cores = 2
wd.files_to_send = ['us.py']
wd.separate_workdir = False
wd.cleanup = 'all'
wd.write()
# read
wd_r = wrapper_data.WrapperDataHostIn()
wd_r.hostname = wd.hostname
wd_r.read()
print wd_r.hostname
print wd_r.sample
print wd_r.first_id
print wd_r.workdir_basename
print wd_r.workdir
print wd_r.tmpdir
print wd_r.remote_tmpdir
print wd_r.n_cores
print wd_r.files_to_send
print wd_r.separate_workdir
print wd_r.cleanup
# get data
print wrapper_data.get_data(wd.get_fullname())
# clean
os.remove(wd.get_fullname())


print '== test WrapperDataHostOut'
# write
wd = wrapper_data.WrapperDataHostOut(remote=True)
wd.hostname = "tata"
wd.sample = [[4, 3, 2], [8]]
wd.add_debug("start point")
wd.add_error(2, "error on point")
wd.add_point(3, 45)
wd.add_warn("toto", "warning")
wd.write_sample()
# read
wd_r = wrapper_data.WrapperDataHostOut()
wd_r.hostname = wd.hostname
wd_r.read()
print wd_r.sample
print 'log'
log = wd_r.get_next_log()
while log:
    flag = log[0]
    data = log[2]
    print '  ' + flag + ':' + str(data)
    log = wd_r.get_next_log()
# get data
print wrapper_data.get_data(wd.get_fullname())
# clean
os.remove(wd.get_fullname())


#print '== test WrapperDataHostsIn'
#print '== test WrapperDataHostsOut'


exit(0)
