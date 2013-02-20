// SWIG file RandomGenerator.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RandomGenerator.hxx"
%}

%include RandomGenerator.hxx

namespace OT { %extend RandomGenerator { RandomGenerator(const RandomGenerator & other) { return new OT::RandomGenerator(other); } } }
namespace OT { %extend RandomGeneratorState { RandomGeneratorState(const RandomGeneratorState & other) { return new OT::RandomGeneratorState(other); } } }
