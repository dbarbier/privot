// SWIG file ZipfMandelbrot.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "ZipfMandelbrot.hxx"
%}

%include ZipfMandelbrot.hxx
namespace OT { %extend ZipfMandelbrot { ZipfMandelbrot(const ZipfMandelbrot & other) { return new OT::ZipfMandelbrot(other); } } }
