# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  core_dispatcher.py
# @brief launch wrapper_file on several cores.
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
launch wrapper_file on several cores.
"""


import wrapper_data

import threading
import time
import socket
import os
import re
import sys
import subprocess
import traceback
import shutil
import pickle
import stat

script_name = 'core_dispatcher.py'
wrapper_launcher_script_name = "wrapper_launcher.py"
coupling_tools_script_name = "coupling_tools.py"

# name of the user wrapper when renamed
user_wrapper = "user_wrapper"


class CoreDispatcher():
    """
    dispatch every wrapper_file computation to local cores
    """

    def __init__(self, wd_host_in, wd_host_out):
        """
        wd_host_in: input data (in sample...)
        wd_host_out: ouput data (out sample...)
        """

        self.wd_host_in = wd_host_in
        self.wd_host_out = wd_host_out

        if wd_host_in.n_cores == 0:
            wd_host_in.n_cores = get_number_of_core()

        self.errors_appear = False

        # shortcut
        self.sample_size = len(wd_host_in.sample)

        self.next_point = 0
        self.mutex_next_point = threading.Lock()


    def exec_sample(self):
        """ exec the sample on localhost """

        # initialize the out sample in order to set outpoint in random order
        self.wd_host_out.sample = [None] * self.sample_size

        self.next_point = 0

        os.chdir(self.wd_host_in.workdir)
        sys.path.append(self.wd_host_in.workdir)

        # create and launch threads
        threads = []
        nb_thread = min(self.sample_size, max(1, self.wd_host_in.n_cores))
        if not self.wd_host_in.separate_workdir:
            nb_thread = 1
        self.wd_host_out.add_debug('compute on ' + socket.gethostname() + ' in'
                                   ' workdir ' + self.wd_host_in.workdir +
                                   ', using ' + str(nb_thread) + ' threads.')

        for i in range(nb_thread):
            thread = ExecInThread(self, i)
            thread.start()
            threads.append(thread)

        # wait they finish
        for thread in threads:
            thread.join()

        if self.wd_host_out.remote:
            self.wd_host_out.write_sample()


    def get_next_point(self):
        """ Private method. Get the next point to compute """

        self.mutex_next_point.acquire()

        point_idx = self.next_point
        self.next_point = self.next_point + 1

        self.mutex_next_point.release()

        return point_idx



class ExecInThread(threading.Thread):
    """ a thread that launch _exec on a point """

    def __init__(self, core_dispatcher, thread_id):

        threading.Thread.__init__(self)
        self.thread_id = thread_id
        self.dispatcher = core_dispatcher

        # shortcuts
        self.wd_host_in = core_dispatcher.wd_host_in
        self.wd_host_out = core_dispatcher.wd_host_out
        self.separate_workdir = self.wd_host_in.separate_workdir

        self.python_exe = sys.executable
        self.workdir = None


    def run_separate(self):
        """ run each wrapper in separate workdir """

        while True:
            # get next point num (cur_id = point's indice of the received sample)
            cur_id = self.dispatcher.get_next_point()
            if cur_id >= self.dispatcher.sample_size:
                #write_result(CoreDispatcher.result_debug, None, "thread num " +
                #             str(self.thread_id) + " stop")
                break

            # _cur_global_id = point's indice of the global sample
            cur_global_id = str(self.wd_host_in.first_id + cur_id)

            start_time = time.time()
            self.wd_host_out.add_debug('thread num ' +  str(self.thread_id) +
                                       ' start computing point ' +
                                       cur_global_id)

            in_point = self.wd_host_in.sample[cur_id]

            # prepare input
            self.create_workdir(cur_global_id)
            wd_core_in = wrapper_data.WrapperDataCoreIn()
            wd_core_in.set_dirname(self.workdir)
            wd_core_in.point = in_point
            wd_core_in.write()


            # launch and get the results
            try:
                self.insulate_command(cur_id)
            except:
                # todo: stop children processes on error?
                ex_info = traceback.format_exc()
                compute_time = str(time.time() - start_time)
                self.wd_host_out.add_error(cur_global_id,
                                           'ERROR when computing point ' +
                                           cur_global_id + ' in ' +
                                           compute_time + ' s \n(' +
                                           ex_info.strip() + ')')
                self.dispatcher.errors_appear = True
            else:
                compute_time = str(time.time() - start_time)
                self.wd_host_out.add_point(cur_global_id,
                                           'finished computing point ' +
                                           cur_global_id + ' in ' +
                                           compute_time + ' s')

                # todo: separate function with faster retry
                if self.wd_host_in.cleanup != 'no':
                    try:
                        shutil.rmtree(self.workdir)
                    except:
                        # NFS cause rmtree raise exception when doing os.rmtree:
                        # "OSError: [Errno 39] Directory not empty:" ?!
                        # give it another chance :
                        time.sleep(5)
                        try:
                            shutil.rmtree(self.workdir)
                        except:
                            ex_info = traceback.format_exc()
                            err_msg = 'Warning: unable to remove dir ' + \
                                    self.workdir + ' \n(' + ex_info.strip() + ')'
                            self.wd_host_out.add_debug(err_msg)


    def run_common(self):
        """ run every wrapper in same workdir """

        # _cur_global_id = point's indice of the global sample
        cur_global_id = str(self.wd_host_in.first_id)
        self.wd_host_out.add_debug('start computing sample')

        # user_wrapper string global variable is modified in place and becomes a module
        exec(compile('import ' + user_wrapper, '<string>', 'exec'), globals())
        
        start_time = time.time()
        self.wd_host_out.sample = map(user_wrapper._exec, self.wd_host_in.sample)

        compute_time = str(time.time() - start_time)
        self.wd_host_out.add_debug('finished computing sample in ' +
                                   compute_time + ' s')



    def run(self):

        if self.separate_workdir:
            self.run_separate()
        else:
            self.run_common()


    def create_workdir(self, cur_global_id):
        """ create point's workdir """

        # create point's workdir
        global_workdir = self.wd_host_in.workdir + os.sep
        self.workdir = global_workdir + cur_global_id
        os.makedirs(self.workdir)
        symlink(global_workdir + user_wrapper + ".py",
                self.workdir + os.sep + user_wrapper + ".py")
        symlink(global_workdir + wrapper_data.script_name,
                self.workdir + os.sep + wrapper_data.script_name)
        symlink(global_workdir + wrapper_launcher_script_name,
                self.workdir + os.sep + wrapper_launcher_script_name)
        symlink(global_workdir + coupling_tools_script_name,
                self.workdir + os.sep + coupling_tools_script_name)

        for user_file in self.wd_host_in.files_to_send:
            shutil.copy(os.path.basename(user_file), self.workdir)


    def insulate_command(self, cur_id):
        """
        Execute the wrapper_launcher in a different processus in order to give
        to the wrapper its own current dir (threads share the same curdir).
        """
        cmd = self.python_exe + " " + self.workdir + os.sep + \
                wrapper_launcher_script_name
        if 'win' not in sys.platform:
            cmd += " " + str(os.getpid())
        self.wd_host_out.add_debug('thread num ' + str(self.thread_id) +
                                   ' start cmd ' + cmd)

        p = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, cwd=self.workdir)
        stdout, stderr = p.communicate()

        # get out point
        # todo: better errmsg if outfile do not exists
        wd_core_out = wrapper_data.WrapperDataCoreOut()
        wd_core_out.set_dirname(self.workdir)
        wd_core_out.read()
        self.wd_host_out.sample[cur_id] = wd_core_out.point

        if p.returncode != 0:
            errmsg = 'command "' + cmd + '" failed with status ' + \
                    str(p.returncode) + ':\n'
            errmsg += '(' + wd_core_out.err_msg + ')\n'
            if stdout.strip() != "":
                errmsg += '(stdout: ' + str(stdout) + ')\n'
            if stderr.strip() != "":
                errmsg += '(stderr: ' + str(stderr) + ')'
            raise Exception(errmsg)



def symlink(src, target):
    """ symlink file if possible """
    if 'win' in sys.platform:
        shutil.copy(src, target)
        os.chmod(target, stat.S_IRWXU)
    else:
        os.symlink(src, target)


def get_number_of_core():
    """
    Number of virtual or physical CPUs on this system, i.e.
    user/real as output by time(1) when called with an optimally scaling
    userspace-only program.
    Source code get on stackoverflow posted by phihag.
    """

    # Python 2.6+
    try:
        import multiprocessing
        return multiprocessing.cpu_count()
    except (ImportError,NotImplementedError):
        pass

    # POSIX
    try:
        res = int(os.sysconf('SC_NPROCESSORS_ONLN'))

        if res > 0:
            return res
    except (AttributeError,ValueError):
        pass

    # Windows
    try:
        res = int(os.environ['NUMBER_OF_PROCESSORS'])

        if res > 0:
            return res
    except (KeyError, ValueError):
        pass

    # jython
    try:
        from java.lang import Runtime
        runtime = Runtime.getRuntime()
        res = runtime.availableProcessors()
        if res > 0:
            return res
    except ImportError:
        pass

    # BSD
    try:
        sysctl = subprocess.Popen(['sysctl', '-n', 'hw.ncpu'],
                                      stdout=subprocess.PIPE)
        scStdout = sysctl.communicate()[0]
        res = int(scStdout)

        if res > 0:
            return res
    except (OSError, ValueError):
        pass

    # Linux
    try:
        res = open('/proc/cpuinfo').read().count('processor\t:')

        if res > 0:
            return res
    except IOError:
        pass

    # Solaris
    try:
        pseudoDevices = os.listdir('/devices/pseudo/')
        expr = re.compile('^cpuid@[0-9]+$')

        res = 0
        for pd in pseudoDevices:
            if expr.match(pd) != None:
                res += 1

        if res > 0:
            return res
    except OSError:
        pass

    # Other UNIXes (heuristic)
    try:
        try:
            dmesg = open('/var/run/dmesg.boot').read()
        except IOError:
            dmesgProcess = subprocess.Popen(['dmesg'], stdout=subprocess.PIPE)
            dmesg = dmesgProcess.communicate()[0]

        res = 0
        while '\ncpu' + str(res) + ':' in dmesg:
            res += 1

        if res > 0:
            return res
    except OSError:
        pass

    raise Exception('Can not determine number of CPUs on this system')
