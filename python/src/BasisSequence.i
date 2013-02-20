// SWIG file BasisSequence.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "BasisSequence.hxx"
%}

OTTypedInterfaceObjectHelper(BasisSequence)

%template(BasisSequenceImplementationTypedCollectionInterfaceObject) OT::TypedCollectionInterfaceObject<OT::BasisSequenceImplementation>;
namespace OT {
%ignore BasisSequence::operator PersistentCollection<Indices> () const;
}

%include BasisSequence.hxx
namespace OT { %extend BasisSequence { BasisSequence(const BasisSequence & other) { return new OT::BasisSequence(other); } } }
