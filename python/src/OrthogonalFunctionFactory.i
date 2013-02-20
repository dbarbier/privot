// SWIG file OrthogonalFunctionFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalFunctionFactory.hxx"
%}

%include OrthogonalFunctionFactory.hxx
namespace OT{ %extend OrthogonalFunctionFactory { OrthogonalFunctionFactory(const OrthogonalFunctionFactory & other) { return new OT::OrthogonalFunctionFactory(other); } } }
