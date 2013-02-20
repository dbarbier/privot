// SWIG file CalibrationStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CalibrationStrategy.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::CalibrationStrategy>
  {
    typedef _PyObject_ Type;
  };


  template <>
  inline
  OT::CalibrationStrategy
  convert< _PyObject_, OT::CalibrationStrategy >(PyObject * pyObj)
  {
    void * ptr = 0;
    if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::CalibrationStrategyImplementation *"), 0 |  0 ) ) ) {
      OT::CalibrationStrategyImplementation * p_dist = reinterpret_cast< OT::CalibrationStrategyImplementation * >( ptr );
      return *p_dist;
    }
    else if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::CalibrationStrategy *"), 0 |  0 ) ) ) {
      OT::CalibrationStrategy * p_dist = reinterpret_cast< OT::CalibrationStrategy * >( ptr );
      return *p_dist;
    }
    else {
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a CalibrationStrategy";  
    }
    return OT::CalibrationStrategy();
  }

} /* namespace OT */


%}

OTTypedInterfaceObjectHelper(CalibrationStrategy)

%include CalibrationStrategy.hxx
namespace OT {  
%extend CalibrationStrategy {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend CalibrationStrategy { CalibrationStrategy(const CalibrationStrategy & other) { return new OT::CalibrationStrategy(other); } } }
