// SWIG file SecondOrderModel.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "SecondOrderModel.hxx"
%}

OTTypedInterfaceObjectHelper(SecondOrderModel)

%include SecondOrderModel.hxx
namespace OT{ %extend SecondOrderModel { SecondOrderModel(const SecondOrderModel & other) { return new OT::SecondOrderModel(other); } } }
