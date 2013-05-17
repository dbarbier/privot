              SWIG development for automatic casts

by Sofiane HADDAD (IMACS), Denis BARBIER (IMACS)

Introduction
============

In this document, we illustrate the use of swig functionalities implemented in order to make the OpenTURNS
user interface more user friendly.
This work has been achieved by the development of two generic macros:
 * OTTypedInterfaceObjectImplementationHelper for the usual interface/implementation pattern
 * OTTypedCollectionInterfaceObjectHelper for collections of interface

These macros are described respectively into the first and second section.

Automatic conversion from implementation into interface
-------------------------------------------------------

The objective of this section is to illustrate the use of the macro OTTypedInterfaceObjectImplementationHelper.
This macro is implemented in python/src/TypedInterfaceObject.i and is defined like this:

  %define OTTypedInterfaceObjectImplementationHelper(Interface,Implementation)

  %template(Implementation ## TypedInterfaceObject)   OT::TypedInterfaceObject<OT::Implementation>;

  %typemap(in) const OT::Interface & {
    void * ptr = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
      // From interface class, ok
    } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_OT__ ## Implementation, 0))) {
      // From Implementation*
      OT::Implementation * p_impl = reinterpret_cast< OT::Implementation * >( ptr );
      $1 = new OT::Interface( *p_impl );
    } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIG_TypeQuery("OT::Pointer<OT::Implementation> *"), 0))) {
      // From Pointer<Implementation>
      OT::Pointer<OT::Implementation> * p_impl = reinterpret_cast< OT::Pointer<OT::Implementation> * >( ptr );
      $1 = new OT::Interface( **p_impl );
    }
  }

  %typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::Interface & {
    $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
      || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIGTYPE_p_OT__ ## Implementation, 0))
      || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIG_TypeQuery("OT::Pointer<OT::Implementation> *"), 0));
  }
  %enddef

There are two input arguments:
 * Interface: this is the interface class, which is used as arguments of most methods;
 * Implementation: this argument refers to the generic implementation class.

Most classes follow the same naming pattern, the most common implementation class is named by appending
Implementation to the interface class.  The macro OTTypedInterfaceObjectHelper was defined in OT1.1
for these cases, and it is now redefined to use OTTypedInterfaceObjectImplementationHelper:

  %define OTTypedInterfaceObjectHelper(Interface)
  OTTypedInterfaceObjectImplementationHelper(Interface,Interface ## Implementation)
  %enddef

The definition above consists of three stanzas:

 1. %template(Implementation ## TypedInterfaceObject)
Tells swig to define a new type and wrap OT::TypedInterfaceObject<OT::Implementation> objects into this type.

 2. %typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::Interface &
When an input argument is of type 'const OT::Interface &', swig checks whether the current argument can be converted into
this type.  As swig treats everything as a pointer, it will in fact try to convert into a pointer on this type.
It will first check whether argument can be casted into the expected type.  If not, it will check whether it can
be casted into a C pointer on implementation, and finally if argument is of type Pointer<Implementation>.

 3. %typemap(in) const OT::Interface &
We saw just above that %typemap(typecheck) reports whether an argument can be converted into the expected type.
The conversion is performed by %typemap(in), $1 contains a pointer on a variable of the expected type.  We can see
the same three checks, and for each case, a pointer is created and returned into $1.


In order to use this interface/implementation automatic conversion, a macro call must be inserted into the
file interface.i.  For instance, Distribution.i contains
  OTTypedInterfaceObjectHelper(Distribution)
and OrthogonalUniVariatePolynomialFamily.i contains
  OTTypedInterfaceObjectImplementationHelper(OrthogonalUniVariatePolynomialFamily, OrthogonalUniVariatePolynomialFactory)
because this class does not follow the usual interface/implementation naming scheme.

This macro call must be inserted before the line
  %include interface.hxx
Otherwise, compilation fails with an error similar to:
 .../python/src/model_copula_modulePYTHON_wrap.cxx: In function ‘PyObject* _wrap_new_Distribution__SWIG_3(PyObject*, PyObject*)’:
 .../python/src/model_copula_modulePYTHON_wrap.cxx:17565:3: error: ‘Implementation’ was not declared in this scope

It is not necessary to %include TypedInterfaceObject.i, because all modules include it at the beginning.


Automatic conversion from Python sequence into Collection<interface> objects
----------------------------------------------------------------------------

The macro OTTypedCollectionInterfaceObjectMisnamedHelper is defined in python/src/TypedCollectionInterfaceObject.i:

  %define OTTypedCollectionInterfaceObjectMisnamedHelper(Interface,CollectionType)

  %template(CollectionType)           OT::Collection<OT::Interface>;

  %typemap(in) const CollectionType & {
    if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
      // From interface class, ok
    } else {
      $1 = OT::buildCollectionFromPySequence< OT::Interface >( $input );
    }
  }

  %typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const CollectionType & {
    $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
      || OT::canConvertCollectionObjectFromPySequence< OT::Interface >( $input );
  }

  %apply const CollectionType & { const OT::Collection<OT::Interface> & };

  %enddef

As with interface/implementation conversion, in the usual case, OpenTURNS defines a Collection<interface> type
by naming it like interfaceCollection.  The OTTypedCollectionInterfaceObjectHelper macro below has been defined
to handle these common cases, but there are few classes which do not follow this naming scheme, this is why
OTTypedCollectionInterfaceObjectMisnamedHelper is needed.

  %define OTTypedCollectionInterfaceObjectHelper(Interface)
  OTTypedCollectionInterfaceObjectMisnamedHelper(Interface,Interface ## Collection)
  %enddef

This is very similar to the previous macro, but there is a new %apply directive.  From bottom to top,
we find:

 1. %apply const CollectionType & { const OT::Collection<OT::Interface> & };
This directive tells swig that when an argument is of type 'const OT::Collection<OT::Interface> &', a typemap defined on
'const CollectionType &' can be used.

 2. %typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::Interface &
This is exactly as in the previous section, swig first checks whether the current argument can be converted into
the right type.  If not, swig will then call canConvertCollectionObjectFromPySequence<Interface>.
This function is defined in PythonWrappingFunctions.hxx, it iterates over the Python sequence and for each item,
call canConvert<typename traitsPythonType<Interface>::Type,Interface>.
If any element cannot be converted, $1 is false, otherwise it is true.
We can see that we have to define a templated traitsPythonType<Interface> structure with a Type member, and a
templated canConvert function.  We will see later how to write them.

 3. %typemap(in) const CollectionType &
The conversion is performed by %typemap(in), $1 contains a pointer on a variable of the expected type.  It is
unchanged if argument is of the right type, otherwise a pointer on a Collection<Interface> is created by calling
buildCollectionFromPySequence<Interface >.

To use these macros, one must define:
 + a templated struct traitsPythonType with a member Type; the canonical form is
    template <> struct traitsPythonType<OT::Interface>
    {
      typedef _PyObject_ Type;
    };
 + template <> inline bool canConvert<_PyObject_, OT::Interface>(PyObject * pyObj)
 + template <> inline OT::Interface convert< _PyObject_, OT::Interface>(PyObject * pyObj)

For convenience, two macros are defined in python/src/TypedCollectionInterfaceObject.i;
  OTDefaultCollectionConvertFunctions(Interface)
  OTDefaultCollectionConvertFunctionsMisnamed(Interface,Implementation)

The latter is expanded into:

  namespace OT {
    template <>
    struct traitsPythonType<OT::Interface>
    {
      typedef _PyObject_ Type;
    };

    template <>
    inline
    bool
    canConvert< _PyObject_, OT::Interface >(PyObject * pyObj)
    {
      void * ptr = 0;
      if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__ ## Interface, 0 ))) {
        OT::Interface * p_it = reinterpret_cast< OT::Interface * >( ptr );
        return p_it != NULL;
      } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__ ## Implementation, 0 ))) {
        OT::Implementation * p_impl = reinterpret_cast< OT::Implementation * >( ptr );
        return p_impl != NULL;
      } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Pointer<OT::Implementation> *"), 0))) {
        OT::Pointer<OT::Implementation> * p_impl = reinterpret_cast< OT::Pointer<OT::Implementation> * >( ptr );
        return p_impl != NULL && !p_impl->isNull();
      }
      return false;
    }

    template <>
    inline
    OT::Interface
    convert< _PyObject_, OT::Interface >(PyObject * pyObj)
    {
      void * ptr = 0;
      if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__ ## Interface, 0 ))) {
        OT::Interface * p_it = reinterpret_cast< OT::Interface * >( ptr );
        return *p_it;
      } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__ ## Implementation, 0 ))) {
        OT::Implementation * p_impl = reinterpret_cast< OT::Implementation * >( ptr );
        return *p_impl;
      } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Pointer<OT::Implementation> *"), 0))) {
        OT::Pointer<OT::Implementation> * p_impl = reinterpret_cast< OT::Pointer<OT::Implementation> * >( ptr );
        return **p_impl;
      }
      else {
        throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a Interface";
      }
      return OT::Interface();
    }

  } /* namespace OT */


If these functions are defined in foo.i, there may be trouble if another module has Collection<foo> as an argument.
For instance, if
  OTDefaultCollectionConvertFunctions(Distribution)
is added into python/src/Distribution.i, bayesian_modulePYTHON_wrap.cxx fails to compile with this error message:
  In file included from .../build/python/src/bayesian_modulePYTHON_wrap.cxx:3946:0:
  .../python/src/PythonWrappingFunctions.hxx: In instantiation of ‘OT::Collection<T>* OT::buildCollectionFromPySequence(PyObject*, int) [with T = OT::Distribution; PyObject = _object]’:
  .../build/python/src/bayesian_modulePYTHON_wrap.cxx:14380:74:   required from here
  .../python/src/PythonWrappingFunctions.hxx:474:11: error: invalid use of incomplete type ‘struct OT::traitsPythonType<OT::Distribution>’
  .../python/src/PythonWrappingFunctions.hxx:35:53: error: declaration of ‘struct OT::traitsPythonType<OT::Distribution>’
  .../python/src/PythonWrappingFunctions.hxx:482:7: error: invalid use of incomplete type ‘struct OT::traitsPythonType<OT::Distribution>’
  .../python/src/PythonWrappingFunctions.hxx:35:53: error: declaration of ‘struct OT::traitsPythonType<OT::Distribution>’
  .../python/src/PythonWrappingFunctions.hxx: In instantiation of ‘bool OT::canConvertCollectionObjectFromPySequence(PyObject*) [with T = OT::Distribution; PyObject = _object]’:
  .../build/python/src/bayesian_modulePYTHON_wrap.cxx:15042:92:   required from here
  .../python/src/PythonWrappingFunctions.hxx:439:7: error: invalid use of incomplete type ‘struct OT::traitsPythonType<OT::Distribution>’
  .../python/src/PythonWrappingFunctions.hxx:35:53: error: declaration of ‘struct OT::traitsPythonType<OT::Distribution>’

Line 1438 contains
      arg5 = OT::buildCollectionFromPySequence< OT::Distribution >( obj4 );
and is used when parsing RandomWalkMetropolisHastings constructor, because two constructors have DistributionCollection as argument.

To avoid this failure, these functions are written into a separate file.  It had been decided
to write them into <module>Collection.i, where <module> is named after <module>TemplateDefs.i.

With the previous example, a line
  %include UncertaintyModelCopulaCollection.i
is inserted into all xxx_module.i which fail to compile otherwise.  This file must be %include'd
and not %import'ed: function definitions are not copied into the *_wrap.cxx file with the latter.
The position of this line in the module file must also be chosen carefully.  If this line is
inserted after
  %import model_copula_module.i
the error is still there.  If it is written very early, some declarations may be missing;
the best solution is to write this %include line just before the %import module, as in
bayesian_module.i:
  %include UncertaintyModelCopulaCollection.i
  %import model_copula_module.i

Line
  OTDefaultCollectionConvertFunctions(Distribution)
in Distribution.i must then be replaced by
  %include UncertaintyModelCopulaCollection.i
otherwise there is a compilation error because of functions being redefined:
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5188:10: error: redefinition of ‘struct OT::traitsPythonType<OT::Distribution>’
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5129:10: error: previous definition of ‘struct OT::traitsPythonType<OT::Distribution>’
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5196:3: error: redefinition of ‘bool OT::canConvert(PyObject*) [with PYTHON_Type = OT::_PyObject_; CPP_Type = OT::Distribution; PyObject = _object]’
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5137:3: error: ‘bool OT::canConvert(PyObject*) [with PYTHON_Type = OT::_PyObject_; CPP_Type = OT::Distribution; PyObject = _object]’ previously declared here
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5213:3: error: redefinition of ‘CPP_Type OT::convert(PyObject*) [with PYTHON_Type = OT::_PyObject_; CPP_Type = OT::Distribution; PyObject = _object]’
  .../build/python/src/model_copula_modulePYTHON_wrap.cxx:5154:3: error: ‘CPP_Type OT::convert(PyObject*) [with PYTHON_Type = OT::_PyObject_; CPP_Type = OT::Distribution; PyObject = _object]’ previously declared here

To summarize, python/src/Drawable.i now contains:

  OTDefaultCollectionConvertFunctions(Drawable)

  OTTypedInterfaceObjectHelper(Drawable)
  OTTypedCollectionInterfaceObjectHelper(Drawable)

  %include Drawable.hxx

If compilation aborts because of traitsPythonType being undefined, function definitions must be moved
into a separate file.  For instance, python/src/Distribution.i now contains:

  %include UncertaintyModelCopulaCollection.i

  OTTypedInterfaceObjectHelper(Distribution)
  OTTypedCollectionInterfaceObjectHelper(Distribution)

  %include Distribution.hxx

and UncertaintyModelCopulaCollection.i contains the following two lines:

  OTDefaultCollectionConvertFunctions(Distribution)
  OTDefaultCollectionConvertFunctionsMisnamed(DistributionFactory, DistributionImplementationFactory)

CAVEAT: If user makes an error and the collection contains an invalid element, swig rightfully discard the
  argument as being convertible into a Collection<foo> and complains that arguments do not match:
    >>> coll = ComposedDistribution([Normal(), 2])
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
      File ".../openturns/model_copula.py", line 4155, in __init__
        this = _model_copula.new_ComposedDistribution(*args)
    NotImplementedError: Wrong number or type of arguments for overloaded function 'new_ComposedDistribution'.
      Possible C/C++ prototypes are:
        OT::ComposedDistribution::ComposedDistribution()
        OT::ComposedDistribution::ComposedDistribution(OT::ComposedDistribution::DistributionCollection const &)
        OT::ComposedDistribution::ComposedDistribution(OT::ComposedDistribution::DistributionCollection const &,OT::Copula const &)
        OT::ComposedDistribution::ComposedDistribution(OT::ComposedDistribution const &)
  An alternative is to let canConvert only check whether argument is a Python sequence and not try to
  convert elements into the expected type.  But then, there must be no overloaded methods which
  differ only by the type of collection, as in FittingTest.hxx:
     static Distribution BestModelBIC(const NumericalSample  & sample,
                                 const DistributionFactoryCollection & factoryCollection);
     static Distribution BestModelBIC(const NumericalSample  & sample,
                                 const DistributionCollection & distributionCollection);

