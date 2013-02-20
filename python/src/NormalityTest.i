// SWIG file NormalityTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NormalityTest.hxx"
%}

%include NormalityTest.hxx
namespace OT { %extend NormalityTest { NormalityTest(const NormalityTest & other) { return new OT::NormalityTest(other); } } }
