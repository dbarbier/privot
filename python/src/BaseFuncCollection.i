// SWIG file BaseFuncCollection.i

%{
#include "PythonNumericalMathEvaluationImplementation.hxx"

namespace OT {

  template <>
  struct traitsPythonType< OT::NumericalMathFunction >
  {
    typedef _PyObject_ Type;
  };

  template <>
  inline
  bool
  canConvert< _PyObject_, OT::NumericalMathFunction >(PyObject * pyObj)
  {
    void * ptr = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__NumericalMathFunction, 0 ))) {
      OT::NumericalMathFunction * p_nmf = reinterpret_cast< OT::NumericalMathFunction * >( ptr );
      return p_nmf != NULL;
    } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__NumericalMathFunctionImplementation, 0 ))) {
      OT::NumericalMathFunctionImplementation * p_impl = reinterpret_cast< OT::NumericalMathFunctionImplementation * >( ptr );
      return p_impl != NULL;
    } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Pointer<OT::NumericalMathFunctionImplementation> *"), 0))) {
      OT::Pointer<OT::NumericalMathFunctionImplementation> * p_impl = reinterpret_cast< OT::Pointer<OT::NumericalMathFunctionImplementation> * >( ptr );
      return p_impl != NULL && !p_impl->isNull();
    } else {
      return PyCallable_Check( pyObj );
    }
  }

  template <>
  inline
  OT::NumericalMathFunction
  convert< _PyObject_, OT::NumericalMathFunction >(PyObject * pyObj)
  {
    void * ptr = 0;
    if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__NumericalMathFunction, 0 ) ) ) {
      OT::NumericalMathFunction * p_nmf = reinterpret_cast< OT::NumericalMathFunction * >( ptr );
      return *p_nmf;
    } else if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__NumericalMathFunctionImplementation, 0 ) ) ) {
      OT::NumericalMathFunctionImplementation * p_impl = reinterpret_cast< OT::NumericalMathFunctionImplementation * >( ptr );
      return *p_impl;
    } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Pointer<OT::NumericalMathFunctionImplementation> *"), 0))) {
      OT::Pointer<OT::NumericalMathFunctionImplementation> * p_impl = reinterpret_cast< OT::Pointer<OT::NumericalMathFunctionImplementation> * >( ptr );
      return **p_impl;
    } else if (!PyCallable_Check( pyObj )) {
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a NumericalMathFunction";
    }
    return OT::NumericalMathFunction(new OT::NumericalMathFunctionImplementation(new OT::PythonNumericalMathEvaluationImplementation(pyObj) ) );
  }

} /* namespace OT */

%}
