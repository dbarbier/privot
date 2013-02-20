// SWIG file FisherSnedecorFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FisherSnedecorFactory.hxx"
%}

%include FisherSnedecorFactory.hxx
namespace OT { %extend FisherSnedecorFactory { FisherSnedecorFactory(const FisherSnedecorFactory & other) { return new OT::FisherSnedecorFactory(other); } } }
