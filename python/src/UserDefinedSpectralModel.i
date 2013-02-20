// SWIG file UserDefinedSpectralModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "UserDefinedSpectralModel.hxx"
%}


%include UserDefinedSpectralModel.hxx
namespace OT{ %extend UserDefinedSpectralModel { UserDefinedSpectralModel(const UserDefinedSpectralModel & other) { return new OT::UserDefinedSpectralModel(other); } } }
