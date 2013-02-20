// SWIG file PostAnalyticalImportanceSampling.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PostAnalyticalImportanceSampling.hxx"
%}

%include PostAnalyticalImportanceSampling.hxx
namespace OT{ %extend PostAnalyticalImportanceSampling { PostAnalyticalImportanceSampling(const PostAnalyticalImportanceSampling & other) { return new OT::PostAnalyticalImportanceSampling(other); } } }
