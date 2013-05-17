//                                               -*- C++ -*-
/**
 *  @file  OTtestcode.hxx
 *  @brief The header file that declares exit codes for tests
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      OTtestcode.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_OTTESTCODE_HXX
#define OPENTURNS_OTTESTCODE_HXX

#include "OTconfig.hxx"

#include <exception>
#include <string>         // for std::string
#include <sstream>        // for std::ostringstream
#include <cstdlib>        // for exit codes
#include <cstring>        // for strcmp

#include "RandomGenerator.hxx"
#include "OStream.hxx"

#define TESTPREAMBLE { OT::Test::parseOptions(argc, argv); }

BEGIN_NAMESPACE_OPENTURNS

/**
 * @brief      The namespace for test specific definitions
 */

namespace Test
{

  /**
   * @typedef int ExitCodeValue
   * ExitCodeValue is the type of the exit code to the operating
   * system. The actual type is determined at configuration time.
   */
  typedef int ExitCodeValue;


  /**
   * @class ExitCode
   * Class ExitCode is a compound that declares exit codes to the
   * operating system when doing the test suite of the project.
   * Each code has a special meaning to the operating system, and
   * precisely to the compilation tools (automake et al.) that use
   * them to know if the test succeeded or failed.
   */

  class ExitCode
  {

  public:
    /** You should return Success when it's OK */
    static const ExitCodeValue Success = EXIT_SUCCESS;

    /** You should return Error when something went wrong */
    static const ExitCodeValue Error = EXIT_FAILURE;

    /** You should return ExpectedToFail when it failed as it should */
    static const ExitCodeValue ExpectedToFail = 77;

  }

    ; /* class ExitCode */





  /**
   * Function parseOptions analyse what the user put
   * on the command line of the executable.
   */
  void parseOptions(int argc, char *argv[])
  {
    for(int i = 1; i < argc; i++)
      {
        if (!strcmp(argv[i], "--version"))
          {
            std::cout << argv[0] << " version " << PACKAGE_VERSION
                      << " (copyright 2005-2010 " << PACKAGE_NAME << ")"
                      << std::endl;
            exit(ExitCode::Success);
          }
      } /* end for */
  }



  /**
   * Random generator initialization
   */
  void setRandomGenerator()
  {
    RandomGenerator::SetSeed(0);
  }





  /**
   * @class TestFailed
   *
   * Class TestFailed is an exception derived class that should be
   * used to throw exception in unitary test code.
   */
  class TestFailed : std::exception
  {
  public:
    TestFailed(const std::string & message) : message_(message) {}
    ~TestFailed() throw() {}
    const char * what() const throw()
    {
      return message_.c_str();
    }

  private:

    const std::string message_;
  };

  std::ostream & operator <<(std::ostream & os, const TestFailed & obj)
  {
    return os << "*** EXCEPTION ***" << std::endl
              << "TestFailed : " << obj.what() << std::endl
              << "*****************";
  }


  /**
   * @fn streamObject(const T & anObject)
   *
   * Try to stream an object onto an ostream object (std::cout).
   * This method tests the operator << of the object.
   */
  template <class T>
  void streamObject(const T & anObject)
  {
    std::cout << "streamObject(const T & anObject)"
              << std::endl;
    OT::OStream fullprint(std::cout);
    fullprint << anObject << std::endl;
  }


  /**
   * @fn show_className()
   *
   * Try to get the name of the class. Check the GetClassName static
   * method.
   */
  template <class T>
  void showClassName()
  {
    std::cout << "Testing class "
              << T::GetClassName()
              << std::endl;
  }


  /**
   * @fn checkConstructorAndDestructor()
   *
   * Try to instanciate an object and delete it. This method tests the
   * default constructor and the destructor ot the class.
   */
  template <class T>
  void checkConstructorAndDestructor()
  {
    std::cout << "checkConstructorAndDestructor()"
              << std::endl;

    // Call the default constructor
    T anObject;
  }


  /**
   * @fn checkCopyConstructor()
   *
   * Try to instanciate an object and copy-construct it.
   */
  template <class T>
  void checkCopyConstructor()
  {
    std::cout << "checkCopyConstructor()"
              << std::endl;

    // Call the default constructor
    T anObject;

    // Call the copy-contructor
    T anCopiedObject( anObject );
  }


  /**
   * @fn checkNameConstructor()
   *
   * Try to instanciate an object with a name.
   */
  template <class T>
  void checkNameConstructor()
  {
    std::cout << "checkNameConstructor()"
              << std::endl;

    OT::String aName("myName");

    // Call the default constructor
    T anObject( aName );

    // Stream out the object
    streamObject( anObject );
  }


  /**
   * @fn areSameObjects(const T & firstObject, const T & secondObject)
   *
   * Try to compare two objects supposed to be identical.
   * This method tests the operator == of the object.
   */
  template <class T>
  OT::Bool areSameObjects(const T & firstObject,
                          const T & secondObject)
  {
    std::cout << "areSameObjects(const T & firstObject, const T & secondObject)"
              << std::endl;

    return (firstObject == secondObject);
  }


  /**
   * @fn areDifferentObjects(const T & firstObject, const T & secondObject)
   *
   * Try to compare two objects supposed to be different.
   * This method tests the operator == of the object.
   */
  template <class T>
  OT::Bool areDifferentObjects(const T & firstObject,
                               const T & secondObject)
  {
    std::cout << "areDifferentObjects(const T & firstObject, const T & secondObject)"
              << std::endl;

    return (firstObject != secondObject);
  }


  /**
   * @fn checkNameAccessors()
   *
   * Try to read and modify the name of an object.
   */
  template <class T>
  void checkNameAccessors()
  {
    std::cout << "checkNameAccessors()"
              << std::endl;

    // Call the default constructor
    T anObject;

    // Change the name of the object
    OT::String aNewName("myNewName");
    anObject.setName( aNewName );

    // Get the name of the object
    OT::String readName = anObject.getName();

    // Stream out the object
    streamObject( anObject );

    // Throw an exception if the name was not correctly set
    if (aNewName != readName)
      {
        OSS errorMessage;
        errorMessage << "OT::Object.getName does NOT return the correct value. Right = "
                     << aNewName
                     << " - Wrong = "
                     << readName;
        throw OT::Test::TestFailed(errorMessage);
      }
  }


  /**
   * @fn checkNameFeature()
   *
   * Try to create an nammed object and access in R/W its name
   */
  template <class T>
  void checkNameFeature()
  {
    std::cout << "checkNameFeature()"
              << std::endl;

    checkNameConstructor<T>();
    checkNameAccessors<T>();
  }


  /**
   * @fn checkClassWithoutClassName()
   *
   * Try to check the standard functionnalities of the class
   */
  template <class T>
  void checkClassWithoutClassName()
  {
    checkConstructorAndDestructor<T>();
    checkCopyConstructor<T>();

    T object1;
    streamObject<T>(object1);

    T object2(object1);
    streamObject<T>(object2);

    if (! areSameObjects<T>(object1, object2))
      {
        throw TestFailed("areSameObjects<T>(object1, object2)");
      }

    if (areDifferentObjects<T>(object1, object2))
      {
        throw TestFailed("areDifferentObjects<T>(object1, object2)");
      }
  }


  /**
   * @fn checkClassWithClassName()
   *
   * Try to check some basic functionnalities of the class
   */
  template <class T>
  void checkClassWithClassName()
  {
    showClassName<T>();
    checkClassWithoutClassName<T>();
  }




} /* namespace Test */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_OTTESTCODE_HXX */