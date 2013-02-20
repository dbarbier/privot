// SWIG file SORMResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SORMResult.hxx"
%}

%include SORMResult.hxx
namespace OT{ %extend SORMResult { SORMResult(const SORMResult & other) { return new OT::SORMResult(other); } } }
