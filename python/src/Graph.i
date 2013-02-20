// SWIG file Graph.i
// @author schueller
// @date   2012-01-11 13:50:09 +0100 (Wed, 11 Jan 2012)

%{
#include "Graph.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::Graph>
  {
    typedef _PySequence_ Type;
  };

  template <>
  inline
  OT::Graph
  convert<_PySequence_,OT::Graph>(PyObject * pyObj)
  {
    String title;
    String xTitle;
    String yTitle;
    Bool showAxes;
    String legendPosition;
    NumericalScalar legendFontSize = ResourceMap::GetAsNumericalScalar("GraphImplementation-DefaultLegendFontSize");

    check<_PySequence_>( pyObj );
    PyObject * newPyObj = PySequence_Fast( pyObj, "" );

    switch (PySequence_Fast_GET_SIZE( newPyObj )) {
     case 6: { PyObject * item_5 = PySequence_Fast_GET_ITEM( newPyObj, 5 );
               check<_PyFloat_>( item_5 );
               legendFontSize = convert<_PyFloat_,NumericalScalar>( item_5 ); }
     case 5: { PyObject * item_4 = PySequence_Fast_GET_ITEM( newPyObj, 4 );
               check<_PyString_>( item_4 );
               legendPosition = convert<_PyString_,String>( item_4 ); }
     case 4: { PyObject * item_0 = PySequence_Fast_GET_ITEM( newPyObj, 0 );
               check<_PyString_>( item_0 );
               title = convert<_PyString_,String>( item_0 );
               PyObject * item_1 = PySequence_Fast_GET_ITEM( newPyObj, 1 );
               check<_PyString_>( item_1 );
               xTitle = convert<_PyString_,String>( item_1 );
               PyObject * item_2 = PySequence_Fast_GET_ITEM( newPyObj, 2 );
               check<_PyString_>( item_2 );
               yTitle = convert<_PyString_,String>( item_2 );
               PyObject * item_3 = PySequence_Fast_GET_ITEM( newPyObj, 3 );
               check<_PyBool_>( item_3 );
               showAxes = convert<_PyBool_,Bool>( item_3 ); }
             break;
     default:
      Py_XDECREF( newPyObj );
      throw InvalidArgumentException(HERE) << "Sequence passed as argument is not convertible to a Graph. Expected sequence (String, String, String, Bool, String, NumericalScalar)";
    }

    Py_XDECREF( newPyObj );
    return OT::Graph( title, xTitle, yTitle, showAxes, legendPosition, legendFontSize);
  }


} /* namespace OT */

%}

OTTypedInterfaceObjectHelper(Graph)

%template(GraphCollection) OT::Collection<OT::Graph>;

%include Graph.hxx

namespace OT{  

%extend Graph {

	 Graph(const Graph & other) { return new OT::Graph(*other.getImplementation()); }

Graph(PyObject * pyObj)
{
 return new OT::Graph( OT::convert<OT::_PySequence_,OT::Graph>(pyObj) );
}

}

}
