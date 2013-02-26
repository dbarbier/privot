// SWIG file CalibrationStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CalibrationStrategy.hxx"
%}

OTDefaultCollectionConvertFunctions(CalibrationStrategy)

OTTypedInterfaceObjectHelper(CalibrationStrategy)
OTTypedCollectionInterfaceObjectHelper(CalibrationStrategy)

%include CalibrationStrategy.hxx

namespace OT {  
%extend CalibrationStrategy {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend CalibrationStrategy { CalibrationStrategy(const CalibrationStrategy & other) { return new OT::CalibrationStrategy(other); } } }
