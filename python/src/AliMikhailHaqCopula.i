// SWIG file AliMikhailHaqCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AliMikhailHaqCopula.hxx"
%}

%include AliMikhailHaqCopula.hxx
namespace OT { %extend AliMikhailHaqCopula { AliMikhailHaqCopula(const AliMikhailHaqCopula & other) { return new OT::AliMikhailHaqCopula(other); } } }
