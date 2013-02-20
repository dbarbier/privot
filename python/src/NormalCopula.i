// SWIG file NormalCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NormalCopula.hxx"
%}

%include NormalCopula.hxx
namespace OT { %extend NormalCopula { NormalCopula(const NormalCopula & other) { return new OT::NormalCopula(other); } } }
