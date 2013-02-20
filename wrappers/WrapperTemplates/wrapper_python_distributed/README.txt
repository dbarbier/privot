OpenTURNS distributed-python-wrapper.

Depandancies:
  python-openturns >= 0.14
  Python >= 2.6?
  SSH server on remote hosts (if remote hosts are used)

How to use it:
  On Linux, change the 'hosts' variable in ot_script.py file to enable the
  distributed mode.
  Launch it with ./start_it.sh or (start_it.bat on windows).

For further info on the DistributedPythonFunction and CouplingTools interface,
  see python/src/distributed_wrapper.py and python/src/coupling_tools.py files
  or read the OpenTURNS WrapperGuide documentation.

