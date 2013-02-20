// SWIG file PostAnalyticalControlledImportanceSampling.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PostAnalyticalControlledImportanceSampling.hxx"
%}

%include PostAnalyticalControlledImportanceSampling.hxx
namespace OT{ %extend PostAnalyticalControlledImportanceSampling { PostAnalyticalControlledImportanceSampling(const PostAnalyticalControlledImportanceSampling & other) { return new OT::PostAnalyticalControlledImportanceSampling(other); } } }
