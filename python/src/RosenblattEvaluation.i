// SWIG file RosenblattEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RosenblattEvaluation.hxx"
%}

%include RosenblattEvaluation.hxx
namespace OT { %extend RosenblattEvaluation { RosenblattEvaluation(const RosenblattEvaluation & other) { return new OT::RosenblattEvaluation(other); } } }
