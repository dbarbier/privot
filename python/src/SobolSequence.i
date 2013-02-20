// SWIG file SobolSequence.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SobolSequence.hxx"
%}

%include SobolSequence.hxx
namespace OT{ %extend SobolSequence { SobolSequence(const SobolSequence & other) { return new OT::SobolSequence(other); } } }
