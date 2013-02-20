// SWIG file Sampler.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Sampler.hxx"
%}

OTTypedInterfaceObjectHelper(Sampler)

%include Sampler.hxx
namespace OT {  
%extend Sampler {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend Sampler { Sampler(const Sampler & other) { return new OT::Sampler(other); } } }
