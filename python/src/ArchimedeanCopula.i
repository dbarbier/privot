// SWIG file ArchimedeanCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ArchimedeanCopula.hxx"
%}

%include ArchimedeanCopula.hxx
namespace OT { %extend ArchimedeanCopula { ArchimedeanCopula(const ArchimedeanCopula & other) { return new OT::ArchimedeanCopula(other); } } }
