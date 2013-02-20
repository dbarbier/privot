// SWIG file BasisSequenceImplementation.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "BasisSequenceImplementation.hxx"
%}

%template(IndicesCollection) OT::Collection<OT::Indices>;
%template(IndicesPersistentCollection) OT::PersistentCollection<OT::Indices>;

//%template(BasisCollection) OT::Collection<OT::Basis>;
//%template(BasisPersistentCollection) OT::PersistentCollection<OT::Basis>;

%include BasisSequenceImplementation.hxx
namespace OT { %extend BasisSequenceImplementation { BasisSequenceImplementation(const BasisSequenceImplementation & other) { return new OT::BasisSequenceImplementation(other); } } }
