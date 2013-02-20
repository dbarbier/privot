// SWIG file MeixnerFactory.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "MeixnerFactory.hxx"
%}

%include MeixnerFactory.hxx
namespace OT { %extend MeixnerFactory { MeixnerFactory(const MeixnerFactory & other) { return new OT::MeixnerFactory(other); } } }
