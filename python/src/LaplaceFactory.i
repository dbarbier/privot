// SWIG file LaplaceFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LaplaceFactory.hxx"
%}

%include LaplaceFactory.hxx
namespace OT { %extend LaplaceFactory { LaplaceFactory(const LaplaceFactory & other) { return new OT::LaplaceFactory(other); } } }
