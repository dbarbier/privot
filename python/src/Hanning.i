// SWIG file Hanning.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "Hanning.hxx"
%}

%include Hanning.hxx
namespace OT { %extend Hanning { Hanning(const Hanning & other) { return new OT::Hanning(other); } } }
