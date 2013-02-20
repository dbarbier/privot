# -*- coding: utf-8 -*-
#                                               -*- Python -*-
#
# @file  remote_communicator.py
# @brief Permit to communicate to a remote host.
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


import os
import sys

# useful for RemoteCommunicatorSSH
import subprocess
import shlex
import StringIO
import string

class RemoteCommunicator():

    def __init__(self, wd_hosts_out=None):
        self.wd_hosts_out = wd_hosts_out

        self.debug = True
        self.os_sep = '/'

        self.host = None

        self.mkdir_exe = 'mkdir'
        self.rm_exe = 'rm'


    def log(self, msg):
        if self.debug == True:
            msg = 'host ' + self.host + ': ' + msg
            if self.wd_hosts_out != None:
                self.wd_hosts_out.add_debug(msg)
            else:
                sys.stderr.write('remote_communicator: ' + msg + '\n')


    def connect(self, host):
        self.host = host

    def disconnect(self):
        pass


    def mkdir(self, remote_dir):
        # todo: chmod rwx r-x --- rather than rwx r-x r-x
        self.launch(self.mkdir_exe + ' -p ' + remote_dir)

    def rmdir(self, remote_dir, ignore_error=True):
        """
        remove file or path recursively

        remote_dir: dir to remove
        ignore_error: ignore nonexistent files, never prompt : no exception
            is raised if the command failed
        """
        if ignore_error:
            self.launch(self.rm_exe + ' -rf ' + remote_dir + ' || true ')
        else:
            self.launch(self.rm_exe + ' -r ' + remote_dir)


    def send_file(self, local_file, remote_file):
        """ send file (or path recursively) to remote host """
        pass

    def send_files(self, local_files, remote_dir):
        """ send files (or path recursively) to remote host """
        for fic in local_files:
            remote_fic = remote_dir + self.os_sep + os.path.basename(fic)
            self.send_file(fic, remote_fic)


    def receive(self, remote_files, localDir):
        raise Exception('Not implemented!')


    def open(self, remote_file, mode='r'):
        """
        open file on the remote host
        return: handle to the remote file
        raise EOFError if file could not be read
        """
        raise Exception('Not implemented!')


    def launch(self, cmd, detached=False, quiet=False):
        """
        return an exception if ssh failed

        detached:
            if False, wait for completion, return the command exit status
            if True, do not wait for completion therefore the cmd status
                is always = 0
        quiet: if False and detached is False, print stdout and stderr if the
            cmd failed

        return: the command exist status
        """
        raise Exception('Not implemented!')



class RemoteCommunicatorSSH(RemoteCommunicator):
    """ implement RemoteCommunicator using ssh command """

    def __init__(self, wd_hosts_out=None, reuse_ssh=True):
        RemoteCommunicator.__init__(self, wd_hosts_out)

        self.ssh_exe = '/usr/bin/ssh'

        # reuse same ssh connection for better latency
        self.reuse_ssh = reuse_ssh
        if reuse_ssh:
            self.ssh_exe += ' -o ControlMaster=auto -o ControlPath=~/.ssh/otmaster-%r@%h:%p'

        self.cat_exe = 'cat'
        self.head_exe = 'head'
        self.tail_exe = 'tail'


    def connect(self, host):
        RemoteCommunicator.connect(self, host)

        if self.reuse_ssh:
            # next ssh con will use master conn. 25920000 = 300 days
            real_cmd = self.ssh_exe + ' ' + self.host + ' sleep 25920000 & '
            self.ssh_master = subprocess.Popen(shlex.split(real_cmd), shell=False)

    def disconnect(self):
        if self.reuse_ssh:
            # stop master conn
            self.ssh_master.terminate()


    def launch(self, cmd, detached=False, quiet=False):
        # todo: check that retunring cmd status is ok with other .py
        real_cmd = self.ssh_exe + ' ' + self.host + ' ' + cmd
        if detached:
            real_cmd += ' &'
        self.log('exec cmd: ' + cmd)

        stdout = subprocess.PIPE
        stderr = subprocess.PIPE

        #p = subprocess.Popen(real_cmd, shell=True,
        p = subprocess.Popen(shlex.split(real_cmd), shell=False,
                             stdout=stdout, stderr=stderr)

        # communicate must be launched before wait
        stdout_data, stderr_data = p.communicate()

        # get return code
        ret = p.wait()
        if ret == 255:
            raise Exception('SSH error, check hostname (command :' + real_cmd +
                            ' returned exit code ' + str(ret) + ')')

        elif ret != 0 and not detached and not quiet:
            self.log("stdout: " + stdout_data)
            self.log("stderr: " + stderr_data)

        return ret


    def send_file(self, local_file, remote_file):
        self.log("send file: " + local_file)
        cmd = self.cat_exe + ' ' + local_file + ' | ' + self.ssh_exe + ' ' + self.host \
                + " '" + self.cat_exe + ' > ' + remote_file + "'"
        #p = subprocess.Popen(shlex.split(cmd), shell=False)
        p = subprocess.Popen(cmd, shell=True)
        ret = p.wait()
        if ret != 0:
            if ret == 255:
                raise Exception('SSH error, check hostname (command :' + cmd +
                                ' returned exit code ' + str(ret) + ')')
            else:
                raise Exception('Command (' + cmd +
                                ') returned exit code ' + str(ret))


    def open(self, remote_file, mode='r'):
        self.log('open file: ' + remote_file)
        if mode == 'r' or mode == 'w':
            return HandleSSH(remote_file, mode, self)
        else:
            raise Exception('Wrong mode argument!')



class HandleSSH():

    def __init__(self, remote_file, mode, rc_ssh):
        self.remote_file = remote_file
        self.mode = mode
        self.rc_ssh = rc_ssh

        # pos in the file
        self.pos = 0

        # buffer
        # todo: free buf already read
        self.buf = ''

        # real pos of the file already read
        self.real_pos = 0

        if mode == 'r':
            self.check_file_exists()


    def check_file_exists(self):
        real_cmd = self.rc_ssh.ssh_exe + ' ' + self.rc_ssh.host
        real_cmd += ' ls ' + self.remote_file

        stdout = subprocess.PIPE
        stderr = subprocess.PIPE

        p = subprocess.Popen(shlex.split(real_cmd), stdout=stdout, stderr=stderr)
        p.communicate()
        ret = p.wait()
        if ret == 255:
            raise Exception('SSH error, check hostname (command :' + real_cmd +
                            ' returned exit code ' + str(ret) + ')')
        elif ret != 0:
            raise IOError('File ' + self.remote_file + ' not found.')


    def seek(self, pos):
        if self.mode == 'w' and pos != 0:
            raise Exception('seek is possible only to pos 0 in write mode!')
        self.pos = pos


    def tell(self):
        return self.pos


    def read(self, size=-1):
        if self.mode != 'r':
            raise Exception('handle open in wrong mode : ' + str(self.mode) + ')')

        #self.rc_ssh.log("read(" + str(size) + ") file: " + self.remote_file

        self.update_buf(size)

        end_pos = self.pos + size
        if size < 0 or len(self.buf) < end_pos:
            data = self.buf[self.pos:]
            self.pos = len(self.buf)
        else:
            # the buf contain enough data
            data = self.buf[self.pos:end_pos]
            self.pos = end_pos

        return data


    def readline(self):
        """ read a line (terminate with a \n) """

        #self.rc_ssh.log("readline file: " + self.remote_file)

        return self.internal_readline(False)


    def internal_readline(self, buf_updated):
        """ private """
        line_end_pos = string.find(self.buf, '\n', self.pos)
        if line_end_pos != -1:
            line_end_pos += 1 # add \n
            data = self.buf[self.pos:line_end_pos]
            self.pos = line_end_pos
        elif buf_updated == True:
            data = self.buf[self.pos:]
            self.pos = len(self.buf)
        else:
            self.update_buf()
            data = self.internal_readline(True)

        return data


    def update_buf(self, size=-1):
        """
        private
        size: min wanted read size
        """
        if size < 0 or (size > 0 and len(self.buf) < self.pos + size):
            # try to get everything
            self.buf += self.real_read()


    def real_read(self, size=-1):
        """
        private
        read data from a remote host
        raise EOFError if file could not be read
        not thread safe

        size: < 0: get the whole file content
        """

        self.rc_ssh.log("real_read file: " + self.remote_file)

        real_cmd = self.rc_ssh.ssh_exe + ' ' + self.rc_ssh.host
        # +1 needed by tail -c +X
        if self.real_pos == 0:
            real_cmd += " '" + self.rc_ssh.cat_exe + ' '
        else:
            real_cmd += " '" + self.rc_ssh.tail_exe + ' -c +' + str(self.real_pos + 1)
        real_cmd += ' ' + self.remote_file
        if size >= 0:
            real_cmd += ' | ' + self.rc_ssh.head_exe + ' -c ' + str(size)
        real_cmd += " '"

        stdout = subprocess.PIPE
        stderr = subprocess.PIPE

        #p = subprocess.Popen(real_cmd, shell=True,
        p = subprocess.Popen(shlex.split(real_cmd), shell=False,
                             stdout=stdout, stderr=stderr)

        # communicate must be launched before wait
        stdout_data, stderr_data = p.communicate()

        # get return code
        ret = p.wait()
        if ret == 255:
            raise Exception('SSH error, check hostname (command :' + real_cmd +
                            ' returned exit code ' + str(ret) + ')')
        elif ret != 0:
            raise Exception('Command (' + real_cmd +
                            ') returned exit code ' + str(ret))

        self.real_pos += len(stdout_data)

        return stdout_data


    def write(self, string):
        """
        write/append data to the remote file already open

        not thread safe
        """
        if self.mode != 'w':
            raise Exception('handle open in wrong mode : ' + str(self.mode) + ')')
        #self.rc_ssh.log("write file: " + self.remote_file)
        self.buf += string


    def flush(self):
        """
        flush data to the remote file already open in write mode

        not thread safe
        """
        if self.mode != 'w':
            raise Exception('handle open in wrong mode : ' + str(self.mode) + ')')
        self.rc_ssh.log("flush file: " + self.remote_file)

        if len(self.buf) == 0:
            return

        #real_cmd = 'echo ' + self.buf + ' | ' + self.rc_ssh.ssh_exe + ' ' + self.rc_ssh.host
        real_cmd = self.rc_ssh.ssh_exe + ' ' + self.rc_ssh.host
        if self.pos == 0:
            real_cmd += " '" + self.rc_ssh.cat_exe + ' > '
        else:
            real_cmd += " '" + self.rc_ssh.cat_exe + ' >> '
        real_cmd += self.remote_file + " '"

        stdin = subprocess.PIPE
        p = subprocess.Popen(shlex.split(real_cmd), shell=False, stdin=stdin)
        p.communicate(input=self.buf)

        # get return code
        ret = p.wait()
        if ret == 255:
            raise Exception('SSH error, check hostname (command :' + real_cmd +
                            ' returned exit code ' + str(ret) + ')')
        elif ret != 0:
            raise Exception('Command (' + real_cmd +
                            ') returned exit code ' + str(ret))

        self.pos += len(self.buf)
        self.buf = ''


    def close(self):
        if self.mode == 'w':
            self.flush()
        self.pos = 0
        self.real_pos = 0
        self.buf = ''



class RemoteCommunicatorParamiko(RemoteCommunicator):
    """ implement RemoteCommunicator using python-paramiko """

    def __init__(self, wd_hosts_out=None):
        RemoteCommunicator.__init__(self, wd_hosts_out)

        self.ssh = None
        self.sftp = None


    def connect(self, host):

        RemoteCommunicator.connect(self, host)

        import paramiko

        self.ssh = paramiko.SSHClient()
        #self.ssh.load_host_keys(os.path.expanduser(os.path.join("~", ".ssh",
        #                        "known_hosts")))
        self.ssh.load_system_host_keys()
        #self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh.set_missing_host_key_policy(paramiko.WarningPolicy())
        self.ssh.connect(host)
        self.sftp = self.ssh.open_sftp()


    def disconnect(self):
        self.sftp.close()
        self.ssh.close()


    def send_file(self, local_file, remote_file):
        self.log("send file: " + local_file + " to " + self.host + ":" +
                 remote_file)
        self.sftp.put(local_file, remote_file)

    def receive(self, remote_files, localDir):
        for fic in remote_files:
            local_fic = localDir + self.os_sep + os.path.basename(fic)
            self.log("receive file: " + local_fic + " from " + self.host +
                     ":" + fic)
            self.sftp.get(fic, local_fic)


    def open(self, remote_file, mode='r'):
        self.log("open file: " + remote_file)
        handle = self.sftp.open(remote_file, mode)
        return handle


    def launch(self, cmd, detached=False, quiet=False):
        self.log("exec cmd: " + cmd + " on " + self.host)

        ret = 0
        real_cmd = cmd
        if detached == True:
            real_cmd += ' &'

        if detached == False:
            chan = self.ssh.get_transport().open_session()
            chan.exec_command(real_cmd)
            ret = chan.recv_exit_status()

            #if ret != 0 and not detached and not quiet:
            #    self.log("stdout: " + stdout_data)
            #    self.log("stderr: " + stderr_data)
        else:
            self.ssh.exec_command(real_cmd)

        return ret
