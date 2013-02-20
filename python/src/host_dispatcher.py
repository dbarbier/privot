# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  host_dispatcher.py
# @brief launch a python wrapper script onto local, remote hosts or a scheduler
#
# Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
# @author souchaud
# @date   2012-08-28
#

"""
launch a python wrapper script onto local or remote hosts or a scheduler.
"""

import core_dispatcher
import wrapper_data

import time
import os
import sys
import traceback
import shutil
import socket
import math
import tempfile

# todo: permit to have several compute type.

script_name = 'host_dispatcher.py'
core_dispatcher_launcher = "core_dispatcher_launcher.py"


class HostDispatcher():
    """
    dispatch every wrapper_file computation to several hosts
    """

    def __init__(self, wd_hosts_in, wd_hosts_out):

        self.wd_hosts_in = wd_hosts_in
        self.wd_hosts_out = wd_hosts_out

        # separator of the remote system
        self.remote_os_sep = "/"
        # python path of the remote system
        self.remote_python_exe = sys.executable


        # get the directory of this module
        self.moduledir = os.path.dirname( os.path.realpath( __file__ ) )

        # stop everything when set to true
        self.stop = False


    def get_scheduler_hosts(self):
        """
        get hosts reserved and launch compute on them
        """
        if self.wd_hosts_in.scheduler == "PBS":
            nodefile = os.getenv("PBS_NODEFILE")
            if nodefile == None:
                raise Exception("ERROR: environment variable PBS_NODEFILE not "
                                "found! \nThis script must be launched from "
                                "the first reserved node!")

            handle = open(nodefile, 'r')
            reserved_hosts = []
            for host in handle.readlines():
                h = host.strip()
                if h not in reserved_hosts:
                    reserved_hosts.append(h)
            handle.close()
            if len(reserved_hosts) == 0:
                raise Exception("ERROR: no hosts found!")
        else:
            raise Exception("ERROR: scheduler " + self.wd_hosts_in.scheduler +
                            " not implemented!")

        self.wd_hosts_in.hosts = reserved_hosts


    def exec_sample(self):
        # try... useful?
        try:
            if self.wd_hosts_in.scheduler:
                self.get_scheduler_hosts()

            if len(self.wd_hosts_in.hosts) > 0:
                return self.exec_on_hosts()
            else:
                return self.exec_on_localhost()

        except:

            ex_info = traceback.format_exc()
            raise Exception("openturns_wrapper: unexpected error ! (" +
                            ex_info + ")")


    ### internals func ###

#    def add_files_to_send(self, local_files):
#        for f in local_files:
#            self.add_file_to_send(f);
#
#
#    def add_file_to_send(self, local_file, remote_dir="", recursive=True,
#                         readonly=False, shared_file_system=False):
#        self.files_to_send[local_file] = {'remote_dir' : remote_dir,
#            'recursive' : recursive,
#            'readonly' : readonly,
#            'shared_file_system' : shared_file_system}
#
#
#    def log(self, hostname="", msg="", log_type="info"):
#
#        if hostname != "":
#            msg = (hostname + ":").ljust(13) + msg
#
#        if self.otlog != None:
#            if log_type == "warn":
#                self.otlog.Warn(msg)
#            elif log_type == "debug":
#                self.otlog.Debug(msg)
#            else:
#                self.otlog.Info(msg)
#        else:
#            print "[" + log_type + "] " + msg
#

    def exec_on_localhost(self):

        self.create_workdir()

        coredispatcher = core_dispatcher.CoreDispatcher(self.wd_hosts_in,
                                                        self.wd_hosts_out)
        coredispatcher.exec_sample()

        # go back to a harmless dir before deleting workdir (useful especially
        # for windows)
        os.chdir(self.wd_hosts_in.tmpdir)

        if self.wd_hosts_in.cleanup == "all" or \
           (not coredispatcher.errors_appear and self.wd_hosts_in.cleanup == "ok"):
            try:
                shutil.rmtree(self.wd_hosts_in.workdir)
            except:
                ex_info = traceback.format_exc()
                self.wd_hosts_out.add_error("unable to remove dir " +
                                            self.wd_hosts_in.workdir + "\n(" +
                                            ex_info.strip() + ")", "warn")


    def create_workdir(self):
        """ create host workdir """
        if not self.wd_hosts_in.tmpdir:
            self.wd_hosts_in.tmpdir = tempfile.gettempdir()
        # prepare base workdir
        workdir = self.wd_hosts_in.tmpdir + os.sep
        workdir += self.wd_hosts_in.workdir_basename
        # fixme: workdir not present explictly in WrapperDataHostIn
        self.wd_hosts_in.workdir = workdir

        os.makedirs(workdir)
        core_dispatcher.symlink(self.wd_hosts_in.wrapper_file,
                                workdir + os.sep + core_dispatcher.user_wrapper + ".py")
        core_dispatcher.symlink(self.moduledir + os.sep +
                                wrapper_data.script_name,
                                workdir + os.sep +
                                wrapper_data.script_name)
        core_dispatcher.symlink(self.moduledir + os.sep +
                                core_dispatcher.coupling_tools_script_name,
                                workdir + os.sep +
                                core_dispatcher.coupling_tools_script_name)

        if self.wd_hosts_in.separate_workdir:
            core_dispatcher.symlink(self.moduledir + os.sep +
                                    core_dispatcher.wrapper_launcher_script_name,
                                    workdir + os.sep +
                                    core_dispatcher.wrapper_launcher_script_name)
        for f in self.wd_hosts_in.files_to_send:
            shutil.copy(f, workdir)


    def exec_on_hosts(self):
        import remote_communicator

        # shortcut
        sample_size = len(self.wd_hosts_in.sample)
        hosts = self.wd_hosts_in.hosts
        hosts_out = self.wd_hosts_out
        hosts_out.sample = []

        self.wd_hosts_out.add_debug('compute using remote hosts: ' + str(hosts))

        # we compute the chunk size that each thread will serve
        chunk     = sample_size / len(hosts)
        remainder = sample_size % len(hosts)
        begin = 0;

        # same workdir name for every hosts
        if not self.wd_hosts_in.remote_tmpdir:
            self.wd_hosts_in.remote_tmpdir = tempfile.gettempdir()
        hosts_workdir = self.wd_hosts_in.remote_tmpdir + os.sep
        hosts_workdir += self.wd_hosts_in.workdir_basename
        self.hosts_workdir = hosts_workdir

        # todo: cleanup local workdir
        # create workdir localy even if not used in order to know if ther is nfs
        #os.makedirs(local_workdir)

        files_to_send = [os.path.basename(f) for f in self.wd_hosts_in.files_to_send]
        module_dir = self.moduledir + os.sep
        files_to_send.append(module_dir + core_dispatcher.script_name)
        files_to_send.append(module_dir + wrapper_data.script_name)
        files_to_send.append(module_dir + core_dispatcher.script_name)
        files_to_send.append(module_dir + core_dispatcher.wrapper_launcher_script_name)
        files_to_send.append(module_dir + core_dispatcher.coupling_tools_script_name)
        files_to_send.append(module_dir + core_dispatcher_launcher)

        # an error appears
        errors_appear = False

        # contain handle to hosts connection
        self.hosts_channel = {}
        # host sample id boundary
        hosts_ids = {}
        # contain whether the host has shared filesystem
        #hosts_nfs = {}
        #cur_nfs_share = 0
        host_num = 0
        for host in hosts:
            # stop command received
            if self.stop:
                hosts_out.add_debug('stopped while launching compute')
                errors_appear = True
                break

            end = begin + chunk
            if remainder > 0:
                end = end + 1
                remainder = remainder - 1
            hosts_ids[host] = [begin, end]

            if end - begin <= 0:
                # more hosts than points
                break

            # todo, write locally if nfs everywhere
            channel = remote_communicator.RemoteCommunicatorSSH(wd_hosts_out=hosts_out)
            try:
                channel.connect(host)
            except:
                ex_info = traceback.format_exc()
                hosts_out.add_warn(host, 'error during connection '
                                   'initialization. Message: ' + str(ex_info))
                raise Exception('error during connection initialization. '
                                'Message: ' + str(ex_info))

            # guess NFS
            #try:
            #    channel.launch('ls ' + self.hosts_workdir)
            #except: # workdir was not found
            #    hosts_nfs[host] = False
            #else: # workdir was found
            #    hosts_nfs[host] = True
            #    self.log(host, "workdir " + self.hosts_workdir + " already exists. Assume tmpdir is on shared filesystem.")

            # create remote workdir
            #if not hosts_nfs[host]:
            channel.mkdir(hosts_workdir)

            # create input file
            wd_host_in = wrapper_data.WrapperDataHostIn()
            wd_host_in.copy(self.wd_hosts_in)
            wd_host_in.sample = self.wd_hosts_in.sample[begin:end]
            wd_host_in.hostname = host
            wd_host_in.first_id = begin
            wd_host_in.workdir = hosts_workdir
            wd_host_in.set_dirname(hosts_workdir)

            #if nfs_everywhere:
            #    handle = open(wd_host_in.get_fullname(), 'wb')
            #    handle.write(in_sample_pkl)
            #    handle.close()
            #else:
            wd_host_in.handle = channel.open(wd_host_in.get_fullname(), 'w')
            wd_host_in.write()

            # send python files
            # todo: warning if overwriting files?
            #if host_num == 0 and nfs_everywhere:
            #    for f in self.files_to_send.keys():
            #        shutil.copy(f, self.hosts_workdir)
            #    shutil.copy(self.wrapper_file, self.hosts_workdir + os.sep + self.user_wrapper + ".py")
            #elif host_num == 0 or not nfs_on_hosts:
            channel.send_files(files_to_send, hosts_workdir)
            channel.send_file(self.wd_hosts_in.wrapper_file, hosts_workdir + os.sep +
                              core_dispatcher.user_wrapper + ".py")


            # launch the core dispatcher and do not wait it
            err_file = hosts_workdir + os.sep + 'core_dispatcher_launcher.err'
            channel.launch(self.remote_python_exe + " " + hosts_workdir +
                           os.sep + core_dispatcher_launcher + " " +
                           wd_host_in.get_fullname() + " > " + err_file + " 2>&1",
                           detached=True)

            if self.wd_hosts_in.extended_check:
                # check if detached processus succeed to start. Avoid this
                # check if possible cause it slow down the launch processus.
                retry = 5
                err_msg = ""
                while retry != 0:
                    try:
                        # fixme: ugly: wait a little, otherwise, sometimes the
                        # failed process has not the time to fill the err file
                        time.sleep(1)
                        handle = channel.open(err_file)
                        err_msg = handle.read()
                        handle.close()
                    except (IOError, EOFError):
                        hosts_out.add_debug(err_file + " not found for host " +
                                            host + ", try again later.")
                        retry -= 1
                    else:
                        if err_msg != "":
                            hosts_out.add_warn(host, "The core_dispatcher_launcher "
                                                "failed ! Error msg: " + err_msg)
                            raise Exception("The core_dispatcher_launcher "
                                            "failed on host " + host +
                                            "! Error msg: " + err_msg)
                        break
                if retry == 0:
                    hosts_out.add_warn(host, err_file + " not found!")


            self.hosts_channel[host] = channel

            host_num += 1
            begin = end


        sleep_time_max = 120
        sleep_time_mult = 1.5

        # todo: parallel fetch: try to get results of each hosts anytimes
        for host in hosts:
            # stop command received
            if self.stop:
                hosts_out.add_warn(host, 'Stopped while start retrieving results')
                errors_appear = True
                break

            if host not in self.hosts_channel:
                # more hosts than points
                break

            channel = self.hosts_channel[host]

            # init object that will parse data from the remote host
            wd_host_out = wrapper_data.WrapperDataHostOut()
            wd_host_out.set_hostname(host)
            wd_host_out.set_dirname(hosts_workdir)

            # init to 2 seconds
            # todo: init 10s
            sleep_time = 1
            # todo: host_out_errors ugly hierarchy
            self.host_out_errors = []

            # try to get results
            self.output_file_lines_read = 0
            host_compute = True
            while host_compute:
                # stop command received
                if self.stop:
                    hosts_out.add_warn(host, 'Stopped compute while waiting '
                                       'results')
                    errors_appear = True
                    break

                try:
                    # little optimization: try nfs
                    wd_host_out.handle = open(wd_host_out.get_fullname(), 'rb')
                    hosts_out.add_debug("file " + wd_host_out.get_fullname() +
                                        " found on localhost.")
                except:
                    # 37m :time python t_ot_wrapper.py -t remote -c ok -w
                    # /cluster/tmp -s 100 -p 0.01 -d -n 1000000
                    # (compute using hosts: ['localhost', 'node-4', 'node-3'])
                    # no nfs
                    try:
                        wd_host_out.handle = channel.open(wd_host_out.get_fullname())
                    except:
                        hosts_out.add_debug("file " + wd_host_out.get_fullname() +
                                            " still not found")
                if wd_host_out.handle != None:
                    # file found, read it
                    new_data = wd_host_out.read()

                    # parse data
                    if new_data:
                        log = wd_host_out.get_next_log()
                        while log:
                            flag = log[0]
                            timestamp = log[1]
                            data = log[2]
                            if flag != hosts_out.flag_sample:
                                hosts_out.add_log(flag, data, timestamp)
                                if flag == hosts_out.flag_error:
                                    errors_appear = True
                            log = wd_host_out.get_next_log()
                        if wd_host_out.sample:
                            hosts_out.sample += wd_host_out.sample
                            host_compute = False

                if host_compute:
                    # todo, less verbose, give number of point calculated
                    hosts_out.add_debug("compute on host " + host +
                                        " not finished, check again in " +
                                        str(sleep_time) + "s")
                    time.sleep(sleep_time)
                    # compute next sleep time
                    sleep_time = int(math.ceil(sleep_time * sleep_time_mult))
                    if sleep_time > sleep_time_max:
                        sleep_time = sleep_time_max


        # cleanup when everything has been computed
        for host in hosts:
            if host not in self.hosts_channel:
                # more hosts than points
                break
            channel = self.hosts_channel[host]

            # cleanup
            #todo: improve while dealing with nfs: if self.cleanup == "yes" or
            # (self.cleanup == "ok" and host not in hosts_with_errors):
            if self.wd_hosts_in.cleanup == "all" or \
               (self.wd_hosts_in.cleanup == "ok" and not errors_appear):
                channel.rmdir(hosts_workdir)

            channel.disconnect()

        # reset stopper
        self.stop = False



    def stop_now(self):
        """ stop and cleanup compute quickly """
        self.stop = True

        for host in self.wd_hosts_in.hosts:
            if host not in self.hosts_channel:
                # more hosts than points
                break
            channel = self.hosts_channel[host]
            channel.launch("pkill -f '" + self.remote_python_exe + "." +
                           self.hosts_workdir + os.sep +
                           core_dispatcher.script_name + "'")
            self.wd_hosts_out.add_debug("child " + host + " stopped")


#    def is_localhost(self, hostname):
#        local = False
#        try:
#            local = socket.getfqdn(hostname) == socket.getfqdn() or \
#                socket.gethostbyname(hostname) == "127.0.0.1" or \
#                socket.gethostname() == hostname
#        except:
#            local = False
#        return local
#
