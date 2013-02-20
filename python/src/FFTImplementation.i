// SWIG file FFTImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "FFTImplementation.hxx"
%}

%include FFTImplementation.hxx
namespace OT { %extend FFTImplementation { FFTImplementation(const FFTImplementation & other) { return new OT::FFTImplementation(other); } } }
