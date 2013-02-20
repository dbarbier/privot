// SWIG file FisherSnedecor.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "FisherSnedecor.hxx"
%}

%include FisherSnedecor.hxx
namespace OT { %extend FisherSnedecor { FisherSnedecor(const FisherSnedecor & other) { return new OT::FisherSnedecor(other); } } }
