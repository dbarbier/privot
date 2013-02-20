// SWIG file LinearTaylor.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearTaylor.hxx"
%}

%include LinearTaylor.hxx
namespace OT { %extend LinearTaylor { LinearTaylor(const LinearTaylor & other) { return new OT::LinearTaylor(other); } } }
