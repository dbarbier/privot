// SWIG file VisualTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "VisualTest.hxx"
%}

%include VisualTest.hxx
namespace OT { %extend VisualTest { VisualTest(const VisualTest & other) { return new OT::VisualTest(other); } } }

