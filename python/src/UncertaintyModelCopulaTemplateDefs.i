// SWIG file UncertaintyModelCopulaTemplateDefs.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%template(DistributionImplementationFactoryPointer) OT::Pointer<OT::DistributionImplementationFactory>;
%template(DistributionImplementationPointer) OT::Pointer<OT::DistributionImplementation>;
%template(RandomVectorImplementationPointer) OT::Pointer<OT::RandomVectorImplementation>;

%template(DistributionCollection) OT::Collection<OT::Distribution>;
%template(DistributionFactoryCollection) OT::Collection<OT::DistributionFactory>;
