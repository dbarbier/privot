// SWIG file Hamming.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "Hamming.hxx"
%}

%include Hamming.hxx
namespace OT { %extend Hamming { Hamming(const Hamming & other) { return new OT::Hamming(other); } } }
