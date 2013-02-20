// SWIG file ComposedCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PythonWrappingFunctions.hxx"
#include "ComposedCopula.hxx"
%}

%template(CopulaCollection) OT::Collection<OT::Copula>;

%include ComposedCopula.hxx

%extend OT::Collection<OT::Copula>
{

OT::Collection<OT::Copula> (PyObject * pyObj)
{
  return OT::buildCollectionFromPySequence<OT::Copula>( pyObj );
}

}


%extend OT::ComposedCopula
{

OT::ComposedCopula(PyObject * pyObj)
{
 OT::Pointer<OT::Collection<OT::Copula> > p_coll =  OT::buildCollectionFromPySequence<OT::Copula>( pyObj );
 return new OT::ComposedCopula( *p_coll );
}

}


namespace OT { %extend ComposedCopula { ComposedCopula(const ComposedCopula & other) { return new OT::ComposedCopula(other); } } }
