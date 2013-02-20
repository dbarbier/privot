// SWIG file CovarianceModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "CovarianceModel.hxx"
%}

OTTypedInterfaceObjectHelper(CovarianceModel)

%include CovarianceModel.hxx
namespace OT{ %extend CovarianceModel { CovarianceModel(const CovarianceModel & other) { return new OT::CovarianceModel(other); } } }
