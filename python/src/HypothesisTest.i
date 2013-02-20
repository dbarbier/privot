// SWIG file HypothesisTest.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HypothesisTest.hxx"
%}

%template(TestResultCollection) OT::Collection<OT::TestResult>;

%include HypothesisTest.hxx
namespace OT { %extend HypothesisTest { HypothesisTest(const HypothesisTest & other) { return new OT::HypothesisTest(other); } } }
