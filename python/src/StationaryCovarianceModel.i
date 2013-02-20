// SWIG file StationaryCovarianceModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "StationaryCovarianceModel.hxx"
%}

%include StationaryCovarianceModel.hxx
namespace OT{ %extend StationaryCovarianceModel { StationaryCovarianceModel(const StationaryCovarianceModel & other) { return new OT::StationaryCovarianceModel(other); } } }
