// SWIG file MCMC.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MCMC.hxx"
%}

%include MCMC.hxx
namespace OT { %extend MCMC { MCMC(const MCMC & other) { return new OT::MCMC(other); } } }
