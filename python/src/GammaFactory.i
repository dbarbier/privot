// SWIG file GammaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GammaFactory.hxx"
%}

%include GammaFactory.hxx
namespace OT { %extend GammaFactory { GammaFactory(const GammaFactory & other) { return new OT::GammaFactory(other); } } }
