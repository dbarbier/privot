// SWIG file TrapezoidalFactory.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "TrapezoidalFactory.hxx"
%}

%include TrapezoidalFactory.hxx
namespace OT { %extend TrapezoidalFactory { TrapezoidalFactory(const TrapezoidalFactory & other) { return new OT::TrapezoidalFactory(other); } } }
