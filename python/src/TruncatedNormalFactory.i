// SWIG file TruncatedNormalFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TruncatedNormalFactory.hxx"
%}

%include TruncatedNormalFactory.hxx
namespace OT { %extend TruncatedNormalFactory { TruncatedNormalFactory(const TruncatedNormalFactory & other) { return new OT::TruncatedNormalFactory(other); } } }
