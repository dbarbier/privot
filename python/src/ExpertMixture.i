// SWIG file ExpertMixture.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ExpertMixture.hxx"
%}

%include ExpertMixture.hxx
namespace OT { %extend ExpertMixture { ExpertMixture(const ExpertMixture & other) { return new OT::ExpertMixture(other); } } }
