// SWIG file ARMALikelihoodFactory.i
// Author : $LastChangedBy: schueller $
// Date : $LastChangedDate: 2012-02-17 19:18:57 +0100 (ven. 17 févr. 2012) $
// Id : $Id: ARMALikelihoodFactory.i 2390 2012-02-17 18:18:57Z schueller $

%{
#include "ARMALikelihoodFactory.hxx"
%}

%include ARMALikelihoodFactory.hxx
namespace OT { %extend ARMALikelihoodFactory { ARMALikelihoodFactory(const ARMALikelihoodFactory & other) { return new OT::ARMALikelihoodFactory(other); } } }
