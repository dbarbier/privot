// SWIG file MediumSafe.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MediumSafe.hxx"
%}

%include MediumSafe.hxx
namespace OT{ %extend MediumSafe { MediumSafe(const MediumSafe & other) { return new OT::MediumSafe(other); } } }
