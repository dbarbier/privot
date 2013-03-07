// SWIG file UserDefinedPair.i
// @author lebrun
// @date   2010-11-21 22:52:28 +0100 (dim. 21 nov. 2010)

%{
#include "UserDefinedPair.hxx"

  namespace OT {

    template <>
      struct traitsPythonType<OT::UserDefinedPair>
      {
        typedef _PyObject_ Type;
      };

    template <>
      inline
      OT::UserDefinedPair *
      buildObjectFromPySequence<OT::UserDefinedPair>(PyObject * pyObj)
      {
        check<_PySequence_>( pyObj );

        const UnsignedLong size = PySequence_Fast_GET_SIZE( pyObj );
        if (size != 2) {
          throw OT::InvalidArgumentException(HERE) << "Sequence object has incorrect size " << size << ". Must be 2.";
        }
        PyObject * elt1 = PySequence_Fast_GET_ITEM( pyObj, 0 );
        check<_PySequence_>( elt1 );
        PyObject * elt2 = PySequence_Fast_GET_ITEM( pyObj, 1 );
        check<_PyFloat_>( elt2 );
        return new OT::UserDefinedPair( convert<_PySequence_,OT::NumericalPoint>( elt1 ),
                                        convert<_PyFloat_,OT::NumericalScalar>( elt2 ) );
      }

    template <>
      inline
      OT::UserDefinedPair
      convert<_PySequence_,OT::UserDefinedPair>(PyObject * pyObj)
      {
        OT::Pointer<OT::UserDefinedPair> ptr = buildObjectFromPySequence<OT::UserDefinedPair>( pyObj );
        return *ptr;
      }

    template <>
      inline
      OT::UserDefinedPair
      convert<_PyObject_,OT::UserDefinedPair>(PyObject * pyObj)
      {
        void * ptr = 0;
        if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__UserDefinedPair, 0 ))) {
          OT::UserDefinedPair * p_hp = reinterpret_cast< OT::UserDefinedPair * >( ptr );
          return *p_hp;

        } else if (isAPython<_PySequence_>( pyObj )) {
          return convert<_PySequence_,OT::UserDefinedPair>( pyObj );

        } else {
          throw OT::InvalidArgumentException(HERE) << "Object passed as argument is neither an UserDefinedPair nor an object convertible to an UserDefinedPair";
        }
        return OT::UserDefinedPair();
      }


  } /* namespace OT */

  %}

%include UserDefinedPair.hxx

namespace OT { %extend UserDefinedPair { UserDefinedPair(const UserDefinedPair & other) { return new OT::UserDefinedPair(other); } } }
