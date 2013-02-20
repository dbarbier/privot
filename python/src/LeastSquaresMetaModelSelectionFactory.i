// SWIG file LeastSquaresMetaModelSelectionFactory.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LeastSquaresMetaModelSelectionFactory.hxx"
%}

%nodefaultctor LeastSquaresMetaModelSelectionFactory;

%include LeastSquaresMetaModelSelectionFactory.hxx
namespace OT { %extend LeastSquaresMetaModelSelectionFactory { LeastSquaresMetaModelSelectionFactory(const LeastSquaresMetaModelSelectionFactory & other) { return new OT::LeastSquaresMetaModelSelectionFactory(other); } } }
