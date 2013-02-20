// SWIG file TensorImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TensorImplementation.hxx"
%}

%rename(TensorImplementation_operator___eq__) OT::operator ==(const TensorImplementation & lhs, const TensorImplementation & rhs);

%include TensorImplementation.hxx
namespace OT{ %extend TensorImplementation { TensorImplementation(const TensorImplementation & other) { return new OT::TensorImplementation(other); } } }
