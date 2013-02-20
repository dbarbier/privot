// SWIG file ARMAFactoryImplementation.i
// @author schueller
// @date   2012-02-17 19:18:57 +0100 (Fri, 17 Feb 2012)

%{
#include "ARMAFactoryImplementation.hxx"
%}

%include ARMAFactoryImplementation.hxx
namespace OT { %extend ARMAFactoryImplementation { ARMAFactoryImplementation(const ARMAFactoryImplementation & other) { return new OT::ARMAFactoryImplementation(other); } } }
