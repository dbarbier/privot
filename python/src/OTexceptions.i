// SWIG file OTexceptions.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (Thu, 17 Nov 2011)

%{
#include "Exception.hxx"
%}

namespace std {
  class exception
  {
  public:
    exception() throw() { }
    virtual ~exception() throw();
    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const throw();
  };
}

%include exception.i

%exception {
  try {
    $action
  }
  catch (OT::InvalidArgumentException & ex) {
    SWIG_exception(SWIG_TypeError,ex.__repr__().c_str());
  }
  catch (OT::OutOfBoundException & ex) {
    SWIG_exception(SWIG_IndexError,ex.__repr__().c_str());
  }
  catch (OT::Exception & ex) {
    SWIG_exception(SWIG_RuntimeError,ex.__repr__().c_str());
  }
  catch (std::range_error & ex) {
    SWIG_exception(SWIG_IndexError,ex.what());
  }
  catch (std::out_of_range & ex) {
    SWIG_exception(SWIG_IndexError,ex.what());
  }
  catch (std::logic_error & ex) { // IDM : Glut Debian Squeeze with libstdc++ 4.4.5
    SWIG_exception(SWIG_IndexError,ex.what());
  }
  catch (std::exception & ex) {
    SWIG_exception(SWIG_RuntimeError,ex.what());
  }
}


%pythoncode %{
class TestFailed:
  """
  TestFailed is used to raise an uniform exception in tests
  """
  __type = "TestFailed"
  def __init__(self,reason=""):
    self.reason = reason
  def type(self):
    return TestFailed.__type
  def what(self):
    return self.reason
  def __str__(self):
    return TestFailed.__type + ": " + self.reason
  def __lshift__(self,ch):
    self.reason += ch
    return self
%}

