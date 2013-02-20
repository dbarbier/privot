// SWIG file GraphImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GraphImplementation.hxx"
%}

%template(DrawableCollection) OT::Collection<OT::Drawable>;

%ignore OT::GraphImplementation::addDrawable(const OT::Drawable&);
%ignore OT::GraphImplementation::addDrawables(const DrawableCollection&);

%include GraphImplementation.hxx

namespace OT{ %extend GraphImplementation { GraphImplementation(const GraphImplementation & other) { return new OT::GraphImplementation(other); } } }
