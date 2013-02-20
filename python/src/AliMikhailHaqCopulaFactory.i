// SWIG file AliMikhailHaqCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AliMikhailHaqCopulaFactory.hxx"
%}

%include AliMikhailHaqCopulaFactory.hxx
namespace OT { %extend AliMikhailHaqCopulaFactory { AliMikhailHaqCopulaFactory(const AliMikhailHaqCopulaFactory & other) { return new OT::AliMikhailHaqCopulaFactory(other); } } }
