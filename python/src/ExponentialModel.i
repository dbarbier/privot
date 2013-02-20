// SWIG file ExponentialModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "ExponentialModel.hxx"
%}

%include ExponentialModel.hxx
namespace OT { %extend ExponentialModel { ExponentialModel(const ExponentialModel & other) { return new OT::ExponentialModel(other); } } }
