// SWIG file LinearModelTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearModelTest.hxx"
%}

%include LinearModelTest.hxx
namespace OT { %extend LinearModelTest { LinearModelTest(const LinearModelTest & other) { return new OT::LinearModelTest(other); } } }
