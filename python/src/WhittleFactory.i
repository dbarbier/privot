// SWIG file WhittleFactory.i
// @author schueller
// @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)

%{
#include "WhittleFactory.hxx"
%}

%template(WhittleFactoryStateCollection)           OT::Collection<OT::WhittleFactoryState>;
%template(WhittleFactoryStatePersistentCollection) OT::PersistentCollection<OT::WhittleFactoryState>;

%include WhittleFactory.hxx
namespace OT { %extend WhittleFactory { WhittleFactory(const WhittleFactory & other) { return new OT::WhittleFactory(other); } } }
