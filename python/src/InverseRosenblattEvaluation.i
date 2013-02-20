// SWIG file InverseRosenblattEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseRosenblattEvaluation.hxx"
%}

%include InverseRosenblattEvaluation.hxx
namespace OT { %extend InverseRosenblattEvaluation { InverseRosenblattEvaluation(const InverseRosenblattEvaluation & other) { return new OT::InverseRosenblattEvaluation(other); } } }
