// SWIG file Brent.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Brent.hxx"
%}

%include Brent.hxx
namespace OT { %extend Brent { Brent(const Brent & other) { return new OT::Brent(other); } } }
