// SWIG file NumericalMathFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalMathFunction.hxx"
#include "PythonNumericalMathEvaluationImplementation.hxx"

namespace OT { 

  template <>
  struct traitsPythonType< OT::NumericalMathFunction >
  {
    typedef _PyObject_ Type;
  };

  template <>
  inline
  OT::NumericalMathFunction
  convert< _PyObject_, OT::NumericalMathFunction >(PyObject * pyObj)
  {
    if (!PyCallable_Check( pyObj )) {
      throw OT::InvalidArgumentException(HERE) << "Argument is not a callable object (function or class)";
    }
    return OT::NumericalMathFunction(new OT::NumericalMathFunctionImplementation(new OT::PythonNumericalMathEvaluationImplementation(pyObj) ) );
  }

} /* namespace OT */

%}

%template(NumericalMathFunctionImplementationTypedInterfaceObject) OT::TypedInterfaceObject<OT::NumericalMathFunctionImplementation>;

%include NumericalMathFunction.hxx
//%copyctor NumericalMathFunction;

namespace OT {  

%extend NumericalMathFunction {

NumericalMathFunction(PyObject * pyObj)
{
  void * ptr = 0;
  if (SWIG_IsOK(SWIG_ConvertPtr(pyObj, &ptr, SWIG_TypeQuery("OT::Object *"), 0)))
  {
    throw OT::InvalidArgumentException(HERE) << "Argument should be a pure python object";
  }
  return new OT::NumericalMathFunction( OT::convert<OT::_PyObject_,OT::NumericalMathFunction>(pyObj) );
}

NumericalMathFunction(const NumericalMathFunction & other)
{
  return new OT::NumericalMathFunction( other );
}

}

}

%pythoncode %{
# We have to make sure the submodule is loaded with absolute path
import openturns.typ

class OpenTURNSPythonFunction :
    """
    OpenTURNSPythonFunction is a class to subclass
    before it can be passed on to a NumericalMathFunction
    -----

    Constructor arguments:
    n: an integer, the input dimension
    p: an integer, the output dimension

    Functions to overload:
    _exec(X): single evaluation, X is a sequence of scalars
    _exec_sample(X): multiple evaluations, X is a 2-d sequence of scalars
    """
    def __init__(self, n=0, p=0) :
        try:
            self.__n = int(n)
        except:
            raise TypeError( 'n argument is not an integer.' )
        try:
            self.__p = int(p)
        except:
            raise TypeError( 'p argument is not an integer.' )
        self.__descIn  = map( lambda i: 'x' + str(i), range(n) )
        self.__descOut = map( lambda i: 'y' + str(i), range(p) )
        
    def setInputDescription(self, descIn):
        if (len(descIn) != self.__n):
            raise ValueError( 'Input description size does NOT match input dimension' )
        self.__descIn  = descIn

    def getInputDescription(self):
        return self.__descIn

    def setOutputDescription(self, descOut):
        if (len(descOut) != self.__p):
            raise ValueError( 'Output description size does NOT match output dimension' )
        self.__descOut  = descOut

    def getOutputDescription(self):
        return self.__descOut

    def getInputDimension(self) :
        return self.__n

    def getOutputDimension(self) :
        return self.__p

    def __str__(self):
        return 'OpenTURNSPythonFunction( %s #%d ) -> %s #%d' % (self.__descIn, self.__n, self.__descOut, self.__p)

    def __repr__(self):
        return self.__str__()

    def __call__(self, X) :
        Y = None
        try:
            pt = openturns.typ.NumericalPoint( X )
        except TypeError:
            try:
                ns = openturns.typ.NumericalSample( X )
            except TypeError:
                raise TypeError( 'Expect a 1-d or 2-d float sequence as argument' )
            else :
                Y = self._exec_sample( ns )
        else :
            Y = self._exec( pt )
        return Y

    def _exec(self, X) :
        if ( not hasattr( self, 'f' ) ):
            raise RuntimeError( 'You must define a method f(X) -> Y, where X and Y are 1-d float sequence objects' )
        import warnings
        warnings.warn( 'usage of method named "f" is now deprecated. Rename it to "_exec" instead', DeprecationWarning )
        return self.f( X )

    def f(self, X) :
        raise RuntimeError( 'You must define a method f(X) -> Y, where X and Y are 1-d float sequence objects' )

    def _exec_sample(self, X) :
        res = list()
        for point in X:
            res.append( self._exec( point ) )
        return res

        

class PythonFunction(NumericalMathFunction):
    """
    PythonFunction allows to build an OpenTURNS function
    from a python function and its dimension attributes
    -----

    Arguments:
    n: an integer, the input dimension
    p: an integer, the output dimension
    func: a pure python function, called on a single point
    func_sample: a pure python function, called on multiple points at once

    Note: you may either one of func or func_sample arguments
    """
    def __new__(self, n, p, func=None, func_sample=None):

        class OpenTURNSPythonFunctionAdvanced(OpenTURNSPythonFunction) :
            def __init__(self, n, p, func=None, func_sample=None) :
                if func == None and func_sample == None:
                    raise RuntimeError( 'no func nor func_sample given.' )
                OpenTURNSPythonFunction.__init__(self, n, p)
                import collections
                if func != None:
                    if not isinstance(func, collections.Callable):
                        raise RuntimeError( 'func argument is not callable.' )
                    self._exec = func
                    self.__class__.__name__ = func.__name__
                if func_sample != None:
                    if not isinstance(func_sample, collections.Callable):
                        raise RuntimeError( 'func_sample argument is not callable.' )
                    self._exec_sample = func_sample
                    # implement exec from exec_sample
                    if func == None:
                        self.__class__.__name__ = func_sample.__name__
                        self._exec = self.exec_point_on_exec_sample

            def exec_point_on_exec_sample(self, X):
                return self._exec_sample([X])[0]

        instance = OpenTURNSPythonFunctionAdvanced(n, p, func, func_sample)
        return NumericalMathFunction(instance)
%}
