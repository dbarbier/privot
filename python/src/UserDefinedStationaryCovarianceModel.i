// SWIG file UserDefinedStationaryCovarianceModel.i
// Author : $LastChangedBy$
// Date : $LastChangedDate$
// Id : $Id$

%{
#include "UserDefinedStationaryCovarianceModel.hxx"
%}


%include UserDefinedStationaryCovarianceModel.hxx
namespace OT{ %extend UserDefinedStationaryCovarianceModel { UserDefinedStationaryCovarianceModel(const UserDefinedStationaryCovarianceModel & other) { return new OT::UserDefinedStationaryCovarianceModel(other); } } }
