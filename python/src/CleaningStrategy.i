// SWIG file CleaningStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CleaningStrategy.hxx"
%}

%include CleaningStrategy.hxx
namespace OT{ %extend CleaningStrategy { CleaningStrategy(const CleaningStrategy & other) { return new OT::CleaningStrategy(other); } } }
