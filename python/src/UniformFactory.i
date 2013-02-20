// SWIG file UniformFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UniformFactory.hxx"
%}

%include UniformFactory.hxx
namespace OT { %extend UniformFactory { UniformFactory(const UniformFactory & other) { return new OT::UniformFactory(other); } } }
