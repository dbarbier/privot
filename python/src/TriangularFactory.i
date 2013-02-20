// SWIG file TriangularFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TriangularFactory.hxx"
%}

%include TriangularFactory.hxx
namespace OT { %extend TriangularFactory { TriangularFactory(const TriangularFactory & other) { return new OT::TriangularFactory(other); } } }
