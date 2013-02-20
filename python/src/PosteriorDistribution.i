// SWIG file PosteriorDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "PosteriorDistribution.hxx"
%}

%include PosteriorDistribution.hxx
namespace OT { %extend PosteriorDistribution { PosteriorDistribution(const PosteriorDistribution & other) { return new OT::PosteriorDistribution(other); } } }
