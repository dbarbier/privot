// SWIG file TNC.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TNC.hxx"
%}

%include TNC.hxx
namespace OT{ %extend TNC { TNC(const TNC & other) { return new OT::TNC(other); } } }
