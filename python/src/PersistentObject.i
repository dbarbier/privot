// SWIG file PersistentObject.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PersistentObject.hxx"
%}

%ignore OT::PersistentObject::operator=(const PersistentObject &);
%rename(Is) is;

%include PersistentObject.hxx
