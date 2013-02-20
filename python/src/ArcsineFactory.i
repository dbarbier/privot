// SWIG file ArcsineFactory.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "ArcsineFactory.hxx"
%}

%include ArcsineFactory.hxx
namespace OT { %extend ArcsineFactory { ArcsineFactory(const ArcsineFactory & other) { return new OT::ArcsineFactory(other); } } }
