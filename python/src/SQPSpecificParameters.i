// SWIG file SQPSpecificParameters.i
// @author dutka
// @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)

%{
#include "SQPSpecificParameters.hxx"
%}

%include SQPSpecificParameters.hxx
namespace OT{ %extend SQPSpecificParameters { SQPSpecificParameters(const SQPSpecificParameters & other) { return new OT::SQPSpecificParameters(other); } } }
