// SWIG file PoissonFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PoissonFactory.hxx"
%}

%include PoissonFactory.hxx
namespace OT { %extend PoissonFactory { PoissonFactory(const PoissonFactory & other) { return new OT::PoissonFactory(other); } } }
