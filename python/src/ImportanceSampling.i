// SWIG file ImportanceSampling.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ImportanceSampling.hxx"
%}

%include ImportanceSampling.hxx
namespace OT{ %extend ImportanceSampling { ImportanceSampling(const ImportanceSampling & other) { return new OT::ImportanceSampling(other); } } }
