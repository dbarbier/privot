// SWIG file FarlieGumbelMorgensternCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "FarlieGumbelMorgensternCopula.hxx"
%}

%include FarlieGumbelMorgensternCopula.hxx
namespace OT { %extend FarlieGumbelMorgensternCopula { FarlieGumbelMorgensternCopula(const FarlieGumbelMorgensternCopula & other) { return new OT::FarlieGumbelMorgensternCopula(other); } } }
