// SWIG file KrawtchoukFactory.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "KrawtchoukFactory.hxx"
%}

%include KrawtchoukFactory.hxx
namespace OT { %extend KrawtchoukFactory { KrawtchoukFactory(const KrawtchoukFactory & other) { return new OT::KrawtchoukFactory(other); } } }
