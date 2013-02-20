// SWIG file Basis.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "Basis.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::Basis>
  {
    typedef _PySequence_ Type;
  };

  template <>
  inline
  OT::Basis
  convert<_PySequence_,OT::Basis>(PyObject * pyObj)
  {
   if (!OT::isAPythonSequenceOf<OT::_PyObject_>( pyObj )) {
     throw OT::InvalidArgumentException(HERE) << "Argument is not a sequence of objects";
   }

  OT::Pointer<Collection<OT::NumericalMathFunction> > ptr = buildCollectionFromPySequence<OT::NumericalMathFunction>(pyObj);

   return OT::Basis( *ptr );
  }

} /* namespace OT */

%}

%template(NumericalMathFunctionCollection) OT::Collection<OT::NumericalMathFunction>;
%template(NumericalMathFunctionPersistentCollection) OT::PersistentCollection<OT::NumericalMathFunction>;

%include Basis.hxx
namespace OT {  

%extend Basis {

Basis(const Basis & other)
{
  return new OT::Basis(other);
}

Basis(PyObject * pyObj)
{
  return new OT::Basis( OT::convert<OT::_PySequence_,OT::Basis>(pyObj) );
}

}

}
