// SWIG file FORMResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FORMResult.hxx"
%}

%include FORMResult.hxx
namespace OT{ %extend FORMResult { FORMResult(const FORMResult & other) { return new OT::FORMResult(other); } } }
