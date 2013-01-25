// SWIG file KSubset.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KSubset.hxx"
%}

%include KSubset.hxx
namespace OT { %extend KSubset { KSubset(const KSubset & other) { return new OT::KSubset(other); } } }
