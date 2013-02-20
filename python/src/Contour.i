// SWIG file Contour.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Contour.hxx"
%}

%include Contour.hxx
namespace OT { %extend Contour { Contour(const Contour & other) { return new OT::Contour(other); } } }
