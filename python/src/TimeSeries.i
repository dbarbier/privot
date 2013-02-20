// SWIG file TimeSeries.i
// @author lebrun
// @date   2011-08-01 17:35:56 +0200 (Mon, 01 Aug 2011)

%{
#include "TimeSeriesImplementation.hxx"
#include "TimeSeries.hxx"
%}

%template(TimeSeriesImplementationTypedInterfaceObject) OT::TypedInterfaceObject<OT::TimeSeriesImplementation>;

#define OT_TYPECHECK_TIMESERIES 3

// %typemap(in) const TimeSeries & ($1_basetype temp) {
//  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
//    temp = OT::convert<OT::_PySequence_,OT::TimeSeries>( $input );
//    $1 = &temp;
//  }
// }

%typemap(typecheck,precedence=OT_TYPECHECK_TIMESERIES) const TimeSeries & {
  $1  = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0));
  // || OT::isAPythonSequenceOf<OT::_PySequence_>( $input );
}

%apply const TimeSeries & { const OT::TimeSeries & };

%include TimeSeriesImplementation.hxx
%include TimeSeries.hxx
 //%copyctor TimeSeries;

namespace OT{  
%extend TimeSeries {

	NumericalScalar __getitem__(PyObject * args) const {

          OT::UnsignedLong arg2 ;
          OT::UnsignedLong arg3 ;
          unsigned long val2 ;
          int ecode2 = 0 ;
          unsigned long val3 ;
          int ecode3 = 0 ;
          PyObject * obj1 = 0 ;
          PyObject * obj2 = 0 ;

          if (!PyArg_ParseTuple(args,(char *)"OO:TimeSeries___getitem__",&obj1,&obj2)) SWIG_fail;

          ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
          if (!SWIG_IsOK(ecode2)) {
            SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "TimeSeries___getitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
          }
          arg2 = static_cast< OT::UnsignedLong >(val2);


          ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
          if (!SWIG_IsOK(ecode3)) {
            SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "TimeSeries___getitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
          }
          arg3 = static_cast< OT::UnsignedLong >(val3);

          return (*self).at(arg2, arg3);
fail:
          return 0.;
	}



	void __setitem__(PyObject * args, NumericalScalar val) {

          OT::UnsignedLong arg2 ;
          OT::UnsignedLong arg3 ;
          unsigned long val2 ;
          int ecode2 = 0 ;
          unsigned long val3 ;
          int ecode3 = 0 ;
          PyObject * obj1 = 0 ;
          PyObject * obj2 = 0 ;

          if (!PyArg_ParseTuple(args,(char *)"OO:TimeSeries___setitem__",&obj1,&obj2)) SWIG_fail;

          ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
          if (!SWIG_IsOK(ecode2)) {
            SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "TimeSeries___setitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
          }
          arg2 = static_cast< OT::UnsignedLong >(val2);


          ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
          if (!SWIG_IsOK(ecode3)) {
            SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "TimeSeries___setitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
          }
          arg3 = static_cast< OT::UnsignedLong >(val3);
          (*self).at(arg2, arg3) = val;
fail:
	  return;
	}


TimeSeries(PyObject * pyObj)
{
 return new OT::TimeSeries( OT::convert<OT::_PySequence_,OT::TimeSeries>(pyObj) );
}

TimeSeries(const TimeSeries & other)
{
 return new OT::TimeSeries( other );
}


} // %extend
}

namespace OT { %extend RegularGrid { RegularGrid(const RegularGrid & other) { return new OT::RegularGrid(other); } } }
