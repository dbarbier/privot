// SWIG file FrankCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FrankCopula.hxx"
%}

%include FrankCopula.hxx
namespace OT { %extend FrankCopula { FrankCopula(const FrankCopula & other) { return new OT::FrankCopula(other); } } }
