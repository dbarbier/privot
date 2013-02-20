// SWIG file BaseTypTemplateDefs.i
// @author schueller
// @date   2012-02-20 08:55:15 +0100 (Mon, 20 Feb 2012)

%template(NumericalSampleImplementationPointer) OT::Pointer<OT::NumericalSampleImplementation>;
%template(DescriptionImplementationPointer) OT::Pointer<OT::DescriptionImplementation>;
%template(MatrixImplementationPointer) OT::Pointer<OT::MatrixImplementation>;
%template(NumericalPointPointer) OT::Pointer<OT::NumericalPoint>;
%template(TensorImplementationPointer) OT::Pointer<OT::TensorImplementation>;
%template(DomainImplementationPointer) OT::Pointer<OT::DomainImplementation>;
%template(ComplexMatrixImplementationPointer) OT::Pointer<OT::ComplexMatrixImplementation>;

%template(HermitianMatrixCollection) OT::Collection<OT::HermitianMatrix>;
