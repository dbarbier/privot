// SWIG file BoxCoxFactory.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "BoxCoxFactory.hxx"
%}

%include BoxCoxFactory.hxx
namespace OT { %extend BoxCoxFactory { BoxCoxFactory(const BoxCoxFactory & other) { return new OT::BoxCoxFactory(other); } } }
