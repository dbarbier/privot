// SWIG file base_module.i
// @author schueller
// @date   2012-02-17 19:18:57 +0100 (Fri, 17 Feb 2012)

%module(package="openturns", docstring="Deterministic meta-package.") base
//#pragma SWIG nowarn=314
%feature("autodoc","1");

%pythoncode %{
from openturns.common import PlatformInfo
def TESTPREAMBLE():
    """
    TESTPREAMBLE() catches the --version argument on the command line,
    prints a identification string and then exits softly.
    """
    import sys
    if "--version" in sys.argv:
        print ( sys.argv[0] + ' ' + PlatformInfo.GetVersion() )
        sys.exit(0)
%}

%{
#include "OTconfig.hxx"
#include "OT.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

/* Base/Common */
%import common_module.i
%import BaseCommonTemplateDefs.i

/* Wrapper */
%import wrapper_module.i
%import BaseWrapperTemplateDefs.i

/* Base/Type */
%import typ_module.i
%import BaseTypTemplateDefs.i

/* Base/Stat */
%import statistics_module.i
%import BaseStatisticsTemplateDefs.i

/* Base/Graph */
%import graph_module.i
%import BaseGraphTemplateDefs.i

/* Base/Graph */
%import geom_module.i

/* Base/Func */
%import func_module.i
%import BaseFuncTemplateDefs.i

/* Base/Diff */
%import diff_module.i
%import BaseDiffTemplateDefs.i

/* Base/Optim */
%import optim_module.i
%import BaseOptimTemplateDefs.i

/* Base/Solver */
%import solver_module.i
%import BaseSolverTemplateDefs.i

/* Base/Algo */
%import algo_module.i
%import BaseAlgoTemplateDefs.i

/* Base/Experiments */
%import experiment_module.i
%import BaseExperimentTemplateDefs.i

/* At last we include template definitions */
%include BaseTemplateDefs.i