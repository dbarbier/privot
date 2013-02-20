// SWIG file CorrectedLeaveOneOut.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "CorrectedLeaveOneOut.hxx"
%}

%include CorrectedLeaveOneOut.hxx
namespace OT { %extend CorrectedLeaveOneOut { CorrectedLeaveOneOut(const CorrectedLeaveOneOut & other) { return new OT::CorrectedLeaveOneOut(other); } } }
