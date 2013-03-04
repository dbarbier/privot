// SWIG file Distribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Distribution.hxx"
#include "PythonDistributionImplementation.hxx"

namespace OT { 

  template <>
  struct traitsPythonType< OT::Distribution >
  {
    typedef _PyObject_ Type;
  };


  template <>
  inline
  OT::Distribution
  convert< _PyObject_, OT::Distribution >(PyObject * pyObj)
  {
    void * ptr = 0;
    if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::DistributionImplementation *"), 0 |  0 ) ) ) {
      OT::DistributionImplementation * p_dist = reinterpret_cast< OT::DistributionImplementation * >( ptr );
      return *p_dist;
    }
    else if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Distribution *"), 0 |  0 ) ) ) {
      OT::Distribution * p_dist = reinterpret_cast< OT::Distribution * >( ptr );
      return *p_dist;
    }
    else {
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a Distribution";  
    }
    return OT::Distribution();
  }

} /* namespace OT */


%}

%pythoncode %{
class PythonDistribution:
    """"
    PythonDistribution is a class to subclass
    before it can be passed on to a Distribution
    """
    def __init__(self, dim = 0):
        self.__dim = dim

    def __str__(self):
        return 'PythonDistribution -> #%d' % self.__dim

    def __repr__(self):
        return self.__str__()
      
    def getDimension(self) :
        return self.__dim

    def computeCDF(self, X):
        raise RuntimeError( 'You must define a method computePDF(x) -> cdf, where cdf is a float' )
        
    def computeComplementaryCDF(self, X):
        raise RuntimeError( 'You must define a method computeComplementaryCDF(x) -> ccdf, where ccdf is a float' )
        
    def computePDFGradient(self, X):
        raise RuntimeError( 'You must define a method computePDFGradient(x) -> dpdf, where dpdf is a sequence' )
    
    def computeCDFGradient(self, X):
        raise RuntimeError( 'You must define a method computeCDFGradient(x) -> dcdf, where dcdf is a sequence' )
    
    def getRoughness(self):
        raise RuntimeError( 'You must define a method getRoughness() -> r, where r is a float' )
        

class SciPyDistribution(PythonDistribution):
    """
    SciPyDistribution subclasses allows to 
    build a PythonDistribution from a scipy distribution
    """
    def __init__(self, dist):
        PythonDistribution.__init__(self, 1)
        if dist.__class__.__name__ != 'rv_frozen':
            raise TypeError( 'Argument is not a scipy distribution' )
        self._dist = dist
      
    def getRealization(self):
        rvs = self._dist.rvs()
        return [rvs]
      
    def getSample(self, size):
        rvs = self._dist.rvs(size)
        return rvs.reshape(size, 1)
        
    def computePDF(self, X):
        pdf = self._dist.pdf(X[0])
        return pdf
    
    def computeCDF(self, X):
        cdf = self._dist.cdf(X[0])
        return cdf
    
    def getMean(self):
        mean = self._dist.mean()
        return [mean]
      
    def getStandardDeviation(self):
        std = self._dist.std()
        return [std]        
        
    def getSkewness(self):
        skewness = float( self._dist.stats( 's' ) )
        return [skewness]  
        
    def getKurtosis(self):
        kurtosis = float( self._dist.stats( 'k' ) )
        return [kurtosis]  
        
    def getMoment(self, n):
        moment = self._dist.moment(n)
        return [moment]  
%}

OTTypedInterfaceObjectHelper(Distribution)
%template(DistributionCollection) OT::Collection<OT::Distribution>;
%typemap(in) const DistributionCollection & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_, DistributionCollection>( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const DistributionCollection & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) ||
       OT::isAPythonSequenceOf<OT::_PyObject_>( $input );
}

%apply const DistributionCollection & { const OT::ComposedDistribution::DistributionCollection & };

%include Distribution.hxx
//%copyctor Distribution;

namespace OT {  

%extend Distribution {

Distribution(const Distribution & other)
{
  return new OT::Distribution(other);
}

Distribution(PyObject * pyObj)
{
  return new OT::Distribution( new OT::PythonDistributionImplementation( pyObj ) );
} 

} // class Distribution
} // namespace OT
