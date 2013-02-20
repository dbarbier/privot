// SWIG file UniVariatePolynomial.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UniVariatePolynomial.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::UniVariatePolynomial>
  {
    typedef _PySequence_ Type;
  };


  template <>
  inline
  OT::UniVariatePolynomial
  convert<_PySequence_,OT::UniVariatePolynomial>(PyObject * pyObj)
  {
    return OT::convert<OT::_PySequence_,OT::NumericalPoint>(pyObj);
  }

} /* namespace OT */

%}

OTTypedInterfaceObjectHelper(UniVariatePolynomial)

%include UniVariatePolynomial.hxx
namespace OT {  

%extend UniVariatePolynomial {

UniVariatePolynomial(const UniVariatePolynomial & other) { return new OT::UniVariatePolynomial(other); }

UniVariatePolynomial(PyObject * pyObj)
{
 return new OT::UniVariatePolynomial( OT::convert<OT::_PySequence_,OT::NumericalPoint>(pyObj) );
}

}

}
