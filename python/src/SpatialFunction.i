// SWIG file SpatialFunction.i
// @author dutka
// @date   2010-11-09 13:44:00 +0100 (mar. 09 nov. 2010)

%{
#include "SpatialFunction.hxx"
%}

%include SpatialFunction.hxx
namespace OT { %extend SpatialFunction { SpatialFunction(const SpatialFunction & other) { return new OT::SpatialFunction(other); } } }
