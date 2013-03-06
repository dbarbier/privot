// SWIG file UserDefined.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UserDefined.hxx"
%}

%include UserDefined.hxx

%template(UserDefinedPairCollection_) OT::Collection<OT::UserDefinedPair>;
%template(UserDefinedPairCollection) OT::PersistentCollection<OT::UserDefinedPair>;

%extend OT::PersistentCollection<OT::UserDefinedPair>
{

PersistentCollection<OT::UserDefinedPair> (PyObject * pyObj)
{
  OT::Pointer<OT::Collection<OT::UserDefinedPair> > ptr = OT::buildCollectionFromPySequence<OT::UserDefinedPair>( pyObj );
  return new OT::PersistentCollection<OT::UserDefinedPair>(ptr->begin(), ptr->end());
}

}

namespace OT {  

%extend UserDefined {
  UserDefined(const OT::Collection<OT::UserDefinedPair> & collection)
  {
    return new OT::UserDefined(collection);
  }
}

}

namespace OT { %extend UserDefined { UserDefined(const UserDefined & other) { return new OT::UserDefined(other); } } }
