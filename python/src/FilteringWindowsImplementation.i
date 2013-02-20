// SWIG file FilteringWindowsImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "FilteringWindowsImplementation.hxx"
%}

%include FilteringWindowsImplementation.hxx
namespace OT{%extend FilteringWindowsImplementation { FilteringWindowsImplementation(const FilteringWindowsImplementation & other) { return new OT::FilteringWindowsImplementation(other); } } }
