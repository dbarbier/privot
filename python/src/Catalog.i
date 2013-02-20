// SWIG file Catalog.i
// @author dutka
// @date   2010-12-16 16:59:25 +0100 (jeu. 16 déc. 2010)

%{
#include "Catalog.hxx"
%}

%include Catalog.hxx
namespace OT {  
%extend Catalog {
  const String __str__() const { return self->__repr__(); }
}
}
