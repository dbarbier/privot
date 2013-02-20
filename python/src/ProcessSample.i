// SWIG file ProcessSamples.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "PythonWrappingFunctions.hxx"
#include "ProcessSample.hxx"
#include "TimeSeries.hxx"
%}

%template(TimeSeriesCollection) OT::Collection<OT::TimeSeries>;
%template(TimeSeriesPersistentCollection) OT::PersistentCollection<OT::TimeSeries>;

 
%include ProcessSample.hxx


//%apply const ProcessSample & { const ProcessSample &, const ProcessSample &, const OT::ProcessSample & };

namespace OT {  
%extend ProcessSample {


ProcessSample(PyObject * pyObj)
{
  OT::Pointer<OT::Collection<OT::TimeSeries> > p_coll =  OT::buildCollectionFromPySequence<OT::TimeSeries>( pyObj );
  return new OT::ProcessSample( *p_coll );
}

ProcessSample(const ProcessSample & other)
{
  return new OT::ProcessSample(other);
}

}

}

