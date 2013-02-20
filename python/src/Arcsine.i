// SWIG file Arcsine.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Arcsine.hxx"
%}

%include Arcsine.hxx
namespace OT { %extend Arcsine { Arcsine(const Arcsine & other) { return new OT::Arcsine(other); } } }
