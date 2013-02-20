// SWIG file MetaModelResult.i
// @author souchaud
// @date   2009-11-16 16:07:57 +0100 (lun. 16 nov. 2009)

%{
#include "MetaModelResult.hxx"
%}

%include MetaModelResult.hxx
namespace OT{ %extend MetaModelResult { MetaModelResult(const MetaModelResult & other) { return new OT::MetaModelResult(other); } } }
