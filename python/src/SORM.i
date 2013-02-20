// SWIG file SORM.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SORM.hxx"
%}

%include SORM.hxx
namespace OT{ %extend SORM { SORM(const SORM & other) { return new OT::SORM(other); } } }
