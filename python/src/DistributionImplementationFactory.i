// SWIG file DistributionImplementationFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DistributionImplementationFactory.hxx"
%}

%include DistributionImplementationFactory.hxx
namespace OT {  

%extend DistributionImplementationFactory {

DistributionImplementationFactory(const DistributionImplementationFactory & other)
{
  return new OT::DistributionImplementationFactory(other);
}

}
}
