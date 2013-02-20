// SWIG file QuadraticCumul.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticCumul.hxx"
%}

%include QuadraticCumul.hxx
namespace OT{ %extend QuadraticCumul { QuadraticCumul(const QuadraticCumul & other) { return new OT::QuadraticCumul(other); } } }
