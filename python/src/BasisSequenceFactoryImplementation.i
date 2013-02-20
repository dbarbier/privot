// SWIG file BasisSequenceFactoryImplementation.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "BasisSequenceFactoryImplementation.hxx"
%}

%include BasisSequenceFactoryImplementation.hxx
namespace OT { %extend BasisSequenceFactoryImplementation { BasisSequenceFactoryImplementation(const BasisSequenceFactoryImplementation & other) { return new OT::BasisSequenceFactoryImplementation(other); } } }
