//                                               -*- C++ -*-
/**
 *  @file  Exception.hxx
 *  @brief Exception defines top-most exception strategies
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      Exception.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_EXCEPTION_HXX
#define OPENTURNS_EXCEPTION_HXX

#include <exception>
#include "OTprivate.hxx"
#include "OSS.hxx"

BEGIN_NAMESPACE_OPENTURNS

#ifndef SWIG
/*
 * A helper class that saves the position where it was instanciated in file.
 * This class works in conjonction with the preprocessor (cpp). See HERE macro
 */
class PointInSourceFile
{
public:
  PointInSourceFile(const  char * file, int line
                    )
    : file_(file), line_(line)
  {}
  ~PointInSourceFile() {}

  const char * getFile() const
  {
    return file_;
  }
  int getLine() const
  {
    return line_;
  }

  std::string str() const
  {
    std::ostringstream oss;
    oss << file_ << ":" << line_
      ;
    return oss.str();
  }

private:
  const char * file_;
  int line_;
}; /* class PointInSourceFile */


/*
 * The macro HERE creates a PointInSourceFile object that saves the position
 * in the source file.
 */

#define HERE OT::PointInSourceFile(__FILE__,__LINE__)

#endif /* SWIG */






/**
 * @class Exception
 *
 * @brief The base class for all Open TURNS' exceptions
 *
 * Exception defines top-most exception strategies. It derives from STL std::exception
 * so Open TURNS' exceptions can be catched as standard exceptions.
 */

class Exception :
  public std::exception
{

public:

  /** Default constructor
   * @internal
   */
  Exception(const PointInSourceFile & point);

  /** Copy constructor */
  Exception(const Exception & other);

  /** Destructor */
  virtual ~Exception() throw();

  /** @copydoc Object::__repr__() const */
  String __repr__() const throw();

  /** Point accessor
   *
   * This method returns a string describing where the exception was launched.
   * No need to free the string.
   */
  const char * where() const throw();

  /** Reason accessor
   *
   * This method returns a string describing what was the reason of the exception.
   * No need to free the string.
   */
  const char * what() const throw();

  /** Class name accessor
   *
   * This method returns a string containing the class of the exception.
   * No need to free the string.
   */
  const char * type() const throw();

  /** Stream operator
   *
   * This method makes exceptions as friendly as std::ostream to add information.
   * @param obj Any object that can be streamed out to an std::ostream can be appended to the exception's reason
   */
  template <class T> Exception & operator << (T obj)
  {
    reason_ += OSS() << obj;
    return *this;
  }


protected:

  /* Inheritance constructor */
  Exception(const PointInSourceFile & point, const char * type);

private:

  /** The point in source file where the exception was raised */
  const PointInSourceFile point_;

  /** Reason describes what was the exception */
  String reason_;

  /** Exception class name */
  const char * className_;

}; /* class Exception */

/**
 * @fn std::ostream & operator <<(std::ostream & os, const Exception & obj)
 * @brief Output stream converter
 * @param os A STL output stream exception
 * @param obj The exception read by \em os
 * @return A reference to \em os
 *
 * Operator << converts the Exception object to an output stream
 * so it is easy to show the reason of the exception.
 */
std::ostream & operator <<(std::ostream & os, const Exception & obj);
#ifndef SWIG
OStream & operator <<(OStream & OS, const Exception & obj);
#endif





/*
 *
 * All exceptions defined for the user are listed below
 *
 */


#define NEW_EXCEPTION( CName ) class CName : public Exception   \
  {                                                             \
  public:                                                       \
    CName (const PointInSourceFile & point);                    \
    virtual ~CName () throw();                                  \
    template <class T> CName & operator << (T obj)              \
    {                                                           \
      this->Exception::operator << ( obj );                     \
      return *this;                                             \
    }                                                           \
  }


/**
 * @class FileNotFoundException
 * @brief Raised when a file can not be found on filesystem
 * @internal
 */
NEW_EXCEPTION( FileNotFoundException );

/**
 * @class InternalException
 * @brief Raised when an internal failure happened preventing processing termination
 * @internal
 */
NEW_EXCEPTION( InternalException );

/**
 * @class InvalidArgumentException
 * @brief Raised when an incorrect argument is passed to a method
 * @internal
 */
NEW_EXCEPTION( InvalidArgumentException );

/**
 * @class InvalidDimensionException
 * @brief Raised when an abject of an incorrect dimension is passed to a method
 * @internal
 */
NEW_EXCEPTION( InvalidDimensionException );

/**
 * @class NoWrapperFileFoundException
 * @brief Raised when a wrapper can not be found on filesystem
 * @internal
 */
NEW_EXCEPTION( NoWrapperFileFoundException );

/**
 * @class NotYetImplementedException
 * @brief Raised when a method has not been implemented yet though it should be
 * @internal
 */
NEW_EXCEPTION( NotYetImplementedException );

/**
 * @class OutOfBoundException
 * @brief Raised when an index of a collection jumped out of the expected bounds
 * @internal
 */
NEW_EXCEPTION( OutOfBoundException );

/**
 * @class WrapperFileParsingException
 * @brief Raised when a wrapper description file has a syntax error or a wrong value
 * @internal
 */
NEW_EXCEPTION( WrapperFileParsingException );

/**
 * @class WrapperInternalException
 * @brief Raised when a wrapper raised an internal error preventing it from going on
 * @internal
 */
NEW_EXCEPTION( WrapperInternalException );

/**
 * @class XMLException
 * @brief Raised when a general XML error was detected
 * @internal
 */
NEW_EXCEPTION( XMLException );

/**
 * @class XMLParserException
 * @brief Raised when an XML error was detected during file parsing
 * @internal
 */
NEW_EXCEPTION( XMLParserException );

/**
 * @class DynamicLibraryException
 * @brief Raised when a dynamic library can not be loaded or when a symbol can not be found in it
 * @internal
 */
NEW_EXCEPTION( DynamicLibraryException );

/**
 * @class NotSymmetricDefinitePositiveException
 * @brief Raised when a matrix seems not to be symmetric nor positive definite
 * @internal
 */
NEW_EXCEPTION( NotSymmetricDefinitePositiveException );

/**
 * @class InvalidRangeException
 * @brief Raised when an incorrect
 * @internal
 */
NEW_EXCEPTION( InvalidRangeException );

/**
 * @class NotDefinedException
 * @brief Raised when raise when a method has no definition and should not have one
 * @internal
 */
NEW_EXCEPTION( NotDefinedException );

/**
 * @class FileOpenException
 * @brief Raised when a file can not be opened
 * @internal
 */
NEW_EXCEPTION( FileOpenException );

/**
 * @class StudyFileParsingException
 * @brief Raised when the study file can be read
 * @internal
 */
NEW_EXCEPTION( StudyFileParsingException );

/**
 * @class ObjectNotInStudyException
 * @brief Raised when a file does not belong to a saved study
 * @internal
 */
NEW_EXCEPTION( ObjectNotInStudyException );

/**
 * @class ConfigurationFileParsingException
 * @brief Raised when the configuration file can not be read
 * @internal
 */
NEW_EXCEPTION( ConfigurationFileParsingException );


#undef NEW_EXCEPTION

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EXCEPTION_HXX */
