// SWIG file FarlieGumbelMorgensternCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "FarlieGumbelMorgensternCopulaFactory.hxx"
%}

%include FarlieGumbelMorgensternCopulaFactory.hxx
namespace OT { %extend FarlieGumbelMorgensternCopulaFactory { FarlieGumbelMorgensternCopulaFactory(const FarlieGumbelMorgensternCopulaFactory & other) { return new OT::FarlieGumbelMorgensternCopulaFactory(other); } } }
