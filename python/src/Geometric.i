// SWIG file Geometric.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Geometric.hxx"
%}

%include Geometric.hxx
namespace OT { %extend Geometric { Geometric(const Geometric & other) { return new OT::Geometric(other); } } }
