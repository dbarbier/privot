// SWIG file ARMACoefficients.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ARMAState.hxx"
%}


%include ARMAState.hxx


namespace OT { %extend ARMAState { ARMAState(const ARMAState & other) { return new OT::ARMAState(other); } } }


