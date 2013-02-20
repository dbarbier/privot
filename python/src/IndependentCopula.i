// SWIG file IndependentCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "IndependentCopula.hxx"
%}

%include IndependentCopula.hxx
namespace OT { %extend IndependentCopula { IndependentCopula(const IndependentCopula & other) { return new OT::IndependentCopula(other); } } }
