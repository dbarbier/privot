// SWIG file DrawableImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DrawableImplementation.hxx"
%}

%include DrawableImplementation.hxx
namespace OT { %extend DrawableImplementation { DrawableImplementation(const DrawableImplementation & other) { return new OT::DrawableImplementation(other); } } }
