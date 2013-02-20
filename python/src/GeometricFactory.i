// SWIG file GeometricFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GeometricFactory.hxx"
%}

%include GeometricFactory.hxx
namespace OT { %extend GeometricFactory { GeometricFactory(const GeometricFactory & other) { return new OT::GeometricFactory(other); } } }
