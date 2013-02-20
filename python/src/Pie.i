// SWIG file Pie.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Pie.hxx"
%}

%include Pie.hxx
namespace OT { %extend Pie { Pie(const Pie & other) { return new OT::Pie(other); } } }
