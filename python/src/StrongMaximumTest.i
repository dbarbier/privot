// SWIG file StrongMaximumTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "StrongMaximumTest.hxx"
%}

%include StrongMaximumTest.hxx
namespace OT{ %extend StrongMaximumTest { StrongMaximumTest(const StrongMaximumTest & other) { return new OT::StrongMaximumTest(other); } } }
