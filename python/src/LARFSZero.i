// SWIG file LARFSZero.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LARFSZero.hxx"
%}

%include LARFSZero.hxx
namespace OT { %extend LARFSZero { LARFSZero(const LARFSZero & other) { return new OT::LARFSZero(other); } } }
