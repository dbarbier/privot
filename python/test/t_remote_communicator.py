#! /usr/bin/env python
# -*- coding: utf8 -*-

# this test need a working ssh server on localhost
# (accessible without password)

import openturns.remote_communicator
import os
import tempfile

hostname = "localhost"


def check_rc(rc):
    rc.debug = True

    tempdir = tempfile.gettempdir() + os.sep
    remote_dir = tempdir + "t_remote_communicator.dir"
    remote_file = remote_dir + os.sep + "t_remote_communicator.file"
    local_file = "t_remote_communicator.file"
    handle = open(local_file, "wb")
    hello_str = "coucou\nsalut\nouech?"
    handle.write(hello_str)
    handle.close()


    rc.connect(hostname)

    print '== mkdir'
    rc.mkdir(remote_dir)

    print '== send existing file'
    rc.send_files([local_file], remote_dir)

    print '== check file there'
    rc.launch("ls " + remote_file)
    rc.launch("ls " + remote_file, detached=True)

    print '== read all with handle'
    remote_file_handle = rc.open(remote_file)
    remote_file_content = remote_file_handle.read()
    remote_file_handle.close()
    print remote_file_content
    if remote_file_content != hello_str:
        raise Exception("Problem while reading remote_file_content with open() ("
                        + remote_file_content + ")")

    print '== cleanup'
    rc.launch("rm " + remote_file)
    if rc.launch("ls " + remote_file) != 2:
        raise Exception("the file (" + remote_file + ") has not been removed!")

    print '== write'
    handle = rc.open(remote_file, 'w')
    handle.write(hello_str)
    handle.close()

    print '== read step by step'
    remote_file_handle = rc.open(remote_file)
    remote_file_content = remote_file_handle.read(4)
    print remote_file_content
    remote_file_content = remote_file_handle.read(1)
    print remote_file_content
    remote_file_content = remote_file_handle.read(4000000)
    print remote_file_content
    remote_file_handle.close()

    print '== read by line'
    remote_file_handle = rc.open(remote_file)
    remote_file_content = remote_file_handle.readline()
    while remote_file_content != '':
        print remote_file_content
        remote_file_content = remote_file_handle.readline()
    remote_file_handle.close()

    print '== rmdir'
    rc.rmdir(remote_dir)

    rc.disconnect()


    os.remove(local_file)


check_rc(openturns.remote_communicator.RemoteCommunicatorSSH())

try:
    import paramiko
    check_rc(openturns.remote_communicator.RemoteCommunicatorParamiko())
except:
    # check it again to have the same output
    check_rc(openturns.remote_communicator.RemoteCommunicatorSSH())
