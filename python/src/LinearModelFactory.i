// SWIG file LinearModelFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearModelFactory.hxx"
%}

%include LinearModelFactory.hxx
namespace OT{ %extend LinearModelFactory { LinearModelFactory(const LinearModelFactory & other) { return new OT::LinearModelFactory(other); } } }
