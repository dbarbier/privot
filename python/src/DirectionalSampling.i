// SWIG file DirectionalSampling.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DirectionalSampling.hxx"
%}

%include DirectionalSampling.hxx
namespace OT{ %extend DirectionalSampling { DirectionalSampling(const DirectionalSampling & other) { return new OT::DirectionalSampling(other); } } }
