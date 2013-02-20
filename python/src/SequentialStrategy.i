// SWIG file SequentialStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SequentialStrategy.hxx"
%}

%include SequentialStrategy.hxx
namespace OT{ %extend SequentialStrategy { SequentialStrategy(const SequentialStrategy & other) { return new OT::SequentialStrategy(other); } } }
