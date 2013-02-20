// SWIG file LinearEnumerateFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearEnumerateFunction.hxx"
%}

%include LinearEnumerateFunction.hxx
namespace OT { %extend LinearEnumerateFunction { LinearEnumerateFunction(const LinearEnumerateFunction & other) { return new OT::LinearEnumerateFunction(other); } } }
