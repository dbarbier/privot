// SWIG file FittingTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FittingTest.hxx"
%}

%include FittingTest.hxx
namespace OT { %extend FittingTest { FittingTest(const FittingTest & other) { return new OT::FittingTest(other); } } }
