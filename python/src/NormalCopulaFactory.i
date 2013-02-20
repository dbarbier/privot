// SWIG file NormalCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NormalCopulaFactory.hxx"
%}

%include NormalCopulaFactory.hxx
namespace OT { %extend NormalCopulaFactory { NormalCopulaFactory(const NormalCopulaFactory & other) { return new OT::NormalCopulaFactory(other); } } }
