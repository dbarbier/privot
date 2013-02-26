// SWIG file DistributionFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DistributionFactory.hxx"
%}

%include UncertaintyModelCopulaCollection.i

OTTypedInterfaceObjectImplementationHelper(DistributionFactory, DistributionImplementationFactory)
OTTypedCollectionInterfaceObjectHelper(DistributionFactory)

%include DistributionFactory.hxx

namespace OT {  

%extend DistributionFactory {

DistributionFactory(const DistributionFactory & other) { return new OT::DistributionFactory(other); }

}
}
