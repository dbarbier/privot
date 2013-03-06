// SWIG file Histogram.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Histogram.hxx"
%}

%include Histogram.hxx

%template(HistogramPairCollection) OT::Collection<OT::HistogramPair>;

%extend OT::Collection<OT::HistogramPair>
{

Collection<OT::HistogramPair> (PyObject * pyObj)
{
  return OT::buildCollectionFromPySequence<OT::Histogram::Pair>( pyObj );
}

}


namespace OT {  

%extend Histogram {
  Histogram(NumericalScalar first, const OT::Collection<OT::HistogramPair> & collection)
  {
    return new OT::Histogram(first,collection);
  }
}

}

namespace OT { %extend Histogram { Histogram(const Histogram & other) { return new OT::Histogram(other); } } }
