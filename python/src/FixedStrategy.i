// SWIG file FixedStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FixedStrategy.hxx"
%}

%include FixedStrategy.hxx
namespace OT{ %extend FixedStrategy { FixedStrategy(const FixedStrategy & other) { return new OT::FixedStrategy(other); } } }
