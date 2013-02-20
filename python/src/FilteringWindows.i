// SWIG file FilteringWindows.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "FilteringWindows.hxx"
%}

OTTypedInterfaceObjectHelper(FilteringWindows)
 
%include FilteringWindows.hxx
namespace OT{%extend FilteringWindows { FilteringWindows(const FilteringWindows & other) { return new OT::FilteringWindows(other); } } }
