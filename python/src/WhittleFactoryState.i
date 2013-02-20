// SWIG file WhittleFactoryState.i
// @author schueller
// @date   2012-02-17 19:18:57 +0100 (ven. 17 févr. 2012)

%{
#include "WhittleFactoryState.hxx"
%}

%include WhittleFactoryState.hxx
namespace OT { %extend WhittleFactoryState { WhittleFactoryState(const WhittleFactoryState & other) { return new OT::WhittleFactoryState(other); } } }
