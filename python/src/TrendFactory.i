// SWIG file TrendFactory.i
// @author haddad
// @date   2011-12-12 18:11:43 +0100 (lun. 12 déc. 2011)

%{
#include "TrendFactory.hxx"
%}

%include TrendFactory.hxx
namespace OT { %extend TrendFactory { TrendFactory(const TrendFactory & other) { return new OT::TrendFactory(other); } } }
