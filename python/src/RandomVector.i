// SWIG file RandomVector.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "RandomVector.hxx"
#include "PythonRandomVectorImplementation.hxx"
%}

%pythoncode %{
class PythonRandomVector:
    """"
    PythonRandomVector is a class to subclass
    before it can be passed on to a RandomVector
    """
    def __init__(self, dim=0):
        # Warning: these names are used in PythonRandomVectorImplementation class. Synchronize the files if changed
        self.__dim = dim
        self.__desc = map( lambda i: 'x' + str(i), range(dim) )

    def __str__(self):
        return 'PythonRandomVector -> %s #%d' % (self.__desc, self.__dim)

    def __repr__(self):
        return self.__str__()
      
    def getDimension(self) :
        return self.__dim
    
    def setDescription(self, desc):
        if (len(desc) != self.__dim):
            raise ValueError( 'Description size does NOT match dimension' )
        self.__desc  = desc
        
    def getDescription(self):
        return self.__desc
      
    def getRealization(self) :
        raise RuntimeError( 'You must define a method getRealization() -> X, where X is a NumericalPoint' )
      
    def getMean(self):
        raise RuntimeError( 'You must define a method mean -> X, where X is a NumericalPoint' )
      
    def getCovariance(self):
        raise RuntimeError( 'You must define a method var -> M, where M is a CovarianceMatrix' )

class SciPyRandomVector(PythonRandomVector):     
    """
    SciPyRandomVector subclasses allows to 
    build a PythonRandomVector from a scipy distribution
    """
    def __init__(self, dist):
        PythonRandomVector.__init__(self, 1)
        if dist.__class__.__name__ != 'rv_frozen':
            raise TypeError( 'Argument is not a scipy distribution' )
        self._dist = dist
      
    def getRealization(self):
        rvs = self._dist.rvs()
        return [rvs]
      
    def getSample(self, size):
        rvs = self._dist.rvs(size)
        return rvs.reshape(size, 1)
    
    def getMean(self):
        mean = self._dist.mean()
        return [mean]
      
    def getCovariance(self):
        var = self._dist.var()
        return [[var]]
    
%}

OTTypedInterfaceObjectHelper(RandomVector)

%include RandomVector.hxx
namespace OT { %extend RandomVector { 

RandomVector(const RandomVector & other)
{
  return new OT::RandomVector(other);
} 

RandomVector(PyObject * pyObj)
{
  return new OT::RandomVector( new OT::PythonRandomVectorImplementation(pyObj) );
} 

} // class RandomVector
} // namespace OT
