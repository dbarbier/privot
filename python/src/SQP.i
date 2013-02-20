// SWIG file SQP.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SQP.hxx"
%}

%include SQP.hxx
namespace OT{ %extend SQP { SQP(const SQP & other) { return new OT::SQP(other); } } }
