#                                               -*- Python -*-
#
#  __init__.py
#
#  Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
#  This library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
#  @author dutka
#  @date   2009-01-28 14:45:54 +0100 (mer, 28 jan 2009)
#

"""
    Open TURNS --- The uncertainty engineering software
    ===================================================

    Documentation is available online at http://www.openturns.org.

    Contents
    --------
      Open TURNS imports all the functions from the OT namespace.

    Available subpackages
    ---------------------
      common                                --- Utility classes
      wrapper                               --- Wrapping utilities
      typ                                   --- Internal data types
      statistics                            --- Statistical classes
      graph                                 --- Graphical output
      geom                                  --- Geometrical primitives
      func                                  --- Function primitives
      diff                                  --- Differential algorithms
      optim                                 --- Optimization routines
      solver                                --- Solvers
      algo                                  --- Approximation algorithms
      experiment                            --- Non probabilistic designs of experiments
      base                                  --- Deterministic meta-package

      classification                        --- Classification algorithms
      model_copula                          --- Copulas
      randomvector                          --- Random vectors
      dist                                  --- Probabilistic distributions
      model_process                         --- Stochastic processes
      weightedexperiment                    --- Probabilistic designs of experiments
      orthogonalbasis                       --- Polynomial primitives
      metamodel                             --- Meta-modelling
      transformation                        --- Iso-probabilistic and process transformations
      analytical                            --- Analytical uncertainty propagation algorithms
      simulation                            --- Simulation uncertainty propagation algorithms
      stattests                             --- Statistical tests
      bayesian                              --- Bayesian updating
      uncertainty                           --- Probabilistic meta-package

    Utility tools
    -------------

      __version__                           --- Open TURNS version string

    Environment variables
    ---------------------

      OPENTURNS_HOME                        --- Custom installation path
      OPENTURNS_MODULE_PATH                 --- Submodule path
      OPENTURNS_CONFIG_PATH                 --- Path to configuration file
      OPENTURNS_WRAPPER_PATH                --- Path to custom wrappers
      OPENTURNS_LOG_SEVERITY                --- Log severity flag, for example "ALL"

"""
import os
import sys
import platform
import warnings
if platform.system() == "Windows":
    # the following line will be modified by the windows installer
    OT_PYTHON_PATH = ""
    if OT_PYTHON_PATH != "":
        # set Path to OT module
        os.environ['PATH'] = OT_PYTHON_PATH + ';' + os.environ['PATH']


# check if interactive mode
if not hasattr(sys, 'ps1'):
    try:
        # ipython does not define ps1
        __IPYTHON__
    except:
        # Reset the default Crtl-C behavior
        import signal
        try:
            signal.signal(signal.SIGINT, signal.SIG_DFL)
        except ValueError:
            warnings.warn( 'Failed to setup thread-interrupt handler. This is usually not critical.' )

from .common import *
from .wrapper import *
from .typ import *
from .graph import *
from .geom import *
from .func import *
from .statistics import *
from .diff import *
from .optim import *
from .solver import *
from .algo import *
from .experiment import *
from .base import *

from .model_copula import *
from .randomvector import *
from .dist import *
from .model_process import *
from .weightedexperiment import *
from .classification import *
from .orthogonalbasis import *
from .metamodel import *
from .transformation import *
from .analytical import *
from .simulation import *
from .stattests import *
from .bayesian import *
from .uncertainty import *

from .distributed_wrapper import *
from .coupling_tools import *

# TODO: remove deprecated Show function
def Show(graph):
    warnings.warn( 'Show function is deprecated, use the viewer module instead.' )
    from .viewer import View
    view = View(graph).show()

# define the version
__version__ = PlatformInfo.GetVersion()

# This code sets the sys.path (through site module) to point to the module standard dirs
dirs = os.getenv("OPENTURNS_MODULE_PATH", None)
if dirs:
    import string
    if(platform.system() != "Windows"):
        path_separator = ":"
    else:
        path_separator = ";"
    dirlist = string.split(dirs, path_separator)
else:
    dirlist = list()
dirlist.append( "%s/openturns/lib/openturns/module" % os.getenv("HOME") )
dirlist.append( Path.GetModuleDirectory() )
dirlist.append( Path.GetInstallationDirectory() )

import site
otpaths = set()
for d in dirlist:
    if(platform.system() != "Windows"):
        python_suffix = "/lib/python%d.%d/site-packages" % ( sys.version_info[0], sys.version_info[1] )
    else:
        python_suffix = "/lib/python%d%d/site-packages" % ( sys.version_info[0], sys.version_info[1] )
    site.addsitedir( d + python_suffix, otpaths )
#print "known_paths =", otpaths

def PRINT(obj):
    return repr(obj)
