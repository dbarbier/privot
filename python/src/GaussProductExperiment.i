// SWIG file GaussProductExperiment.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "GaussProductExperiment.hxx"
%}

%include GaussProductExperiment.hxx
namespace OT { %extend GaussProductExperiment { GaussProductExperiment(const GaussProductExperiment & other) { return new OT::GaussProductExperiment(other); } } }
