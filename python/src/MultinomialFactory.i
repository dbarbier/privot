// SWIG file MultinomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MultinomialFactory.hxx"
%}

%include MultinomialFactory.hxx
namespace OT { %extend MultinomialFactory { MultinomialFactory(const MultinomialFactory & other) { return new OT::MultinomialFactory(other); } } }
