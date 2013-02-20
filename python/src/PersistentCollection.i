// SWIG file PersistentCollection.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PersistentCollection.hxx"
%}

%include PersistentCollection.hxx

%template(UnsignedLongPersistentCollection) OT::PersistentCollection<OT::UnsignedLong>;
