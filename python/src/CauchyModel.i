// SWIG file CauchyModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "CauchyModel.hxx"
%}

%include CauchyModel.hxx
namespace OT { %extend CauchyModel { CauchyModel(const CauchyModel & other) { return new OT::CauchyModel(other); } } }
