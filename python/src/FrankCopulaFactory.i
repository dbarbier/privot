// SWIG file FrankCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FrankCopulaFactory.hxx"
%}

%include FrankCopulaFactory.hxx
namespace OT { %extend FrankCopulaFactory { FrankCopulaFactory(const FrankCopulaFactory & other) { return new OT::FrankCopulaFactory(other); } } }
