// SWIG file DescriptionImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DescriptionImplementation.hxx"
%}

%template(StringCollection) OT::Collection<OT::String>;
%template(StringPersistentCollection) OT::PersistentCollection<OT::String>;
%include DescriptionImplementation.hxx
