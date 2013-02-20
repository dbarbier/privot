// SWIG file FunctionalBasisProcess.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (mer. 04 janv. 2012)

%{
#include "FunctionalBasisProcess.hxx"
%}

%include FunctionalBasisProcess.hxx
namespace OT { %extend FunctionalBasisProcess { FunctionalBasisProcess(const FunctionalBasisProcess & other) { return new OT::FunctionalBasisProcess(other); } } }
