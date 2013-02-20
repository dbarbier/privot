//                                               -*- C++ -*-
/**
 *  @file  XMLStorageManager.hxx
 *  @brief XMLStorageManager provides an interface for different storage classes
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      XMLStorageManager.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_XMLSTORAGEMANAGER_HXX
#define OPENTURNS_XMLSTORAGEMANAGER_HXX

#include <iostream>              // for std::ostream
#include <stack>                 // for std::stack
#include "OTprivate.hxx"
#include "StorageManager.hxx"


BEGIN_NAMESPACE_OPENTURNS

/* Forward declaration of XMLDoc */
class XMLDoc;




/* Forward declaration of PersistentObject */
class PersistentObject;

/* Forward declaration of XMLStorageManagerState */
struct XMLStorageManagerState;

class VersionList
{
  std::vector<UnsignedLong> versions_;
public:
  explicit VersionList() : versions_()
  {
    versions_.push_back(1);
  }

  Bool contains(UnsignedLong v) const
  {
    return find(versions_.begin(), versions_.end(), v) != versions_.end();
  }
};

/**
 * @class XMLStorageManager
 * @brief Implements a storage manager that drives a XML file
 * @see StorageManager
 * @see Study
 */

class XMLStorageManager
  : public StorageManager
{
  CLASSNAME;



  static const VersionList SupportedVersions;

public:

  /**
   * The numerical precision used to write out scientific numbers
   */
  static const int Precision_;

  /**
   * Constructor for a XML StorageManager that reads/writes
   * to a XML file
   */
  XMLStorageManager(const FileName & filename);

  /**
   * Virtual constructor
   */
  virtual XMLStorageManager * clone() const;

  /** @copydoc StorageManager::__repr__() const */
  virtual String __repr__() const;

  /** Get the name of the file containing the study */
  String getFileName() const;

  /** Set the name of the file containing the study */
  void setFileName(const String & fileName);

  /**
   * This method saves the PersistentObject onto the medium
   */
  virtual void save(const PersistentObject & obj, const String & label, bool fromStudy = false);

  /**
   * This method reloads the PersistentObject from the medium
   */
  virtual void load(Study & study);



  /** Do some administrative tasks before saving/reloading
   * @internal
   */
  virtual void initialize(const SaveAction caller);
  virtual void initialize(const LoadAction caller);

  /** Do some administrative tasks after saving/reloading
   * @internal
   */
  virtual void finalize(const SaveAction caller);
  virtual void finalize(const LoadAction caller);

  /** Read and create the internal representation
   * @internal
   */
  virtual void read();

  /** Write the internal representation
   * @internal
   */
  virtual void write();



#ifndef SWIG
  /* Create a new empty object that will gather all saved information
   * @internal
   */
  virtual Pointer<InternalObject> createObject(const String & tag) const;

  /* Append an internal object to the collection of saved ones
   * @internal
   */
  virtual void appendObject(Pointer<InternalObject> & p_obj);

  /* Set the visibility attribute of the object
   * @internal
   */
  virtual void setVisibility(Pointer<InternalObject> & p_obj, Bool visible);

  /* Set the label associated with the object
   * @internal
   */
  virtual void setLabel(Pointer<InternalObject> & p_obj, const String & label);

  /* Returns true if the internal object has an attribute
   */
  virtual Bool hasAttribute(Pointer<InternalObject> & p_obj, const String & name);

  /* Add an attribute to an internal object
   * @internal
   */
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, Bool value);
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, UnsignedLong value);
#ifndef __LP64__
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, Unsigned64BitsInteger value);
#endif
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, NumericalScalar value);
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, NumericalComplex value);
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, const String & value);
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, const InterfaceObject & value);
  virtual void addAttribute(Pointer<InternalObject> & p_obj, const String & name, const PersistentObject & value);

  /* Add an indexed value to an internal object
   * @internal
   */
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, Bool value);
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, UnsignedLong value);
#ifndef __LP64__
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, Unsigned64BitsInteger value);
#endif
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, NumericalScalar value);
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, NumericalComplex value);
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, const String & value);
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, const InterfaceObject & value);
  virtual void addIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, const PersistentObject & value);

  /* Read an attribute
   * @internal
   */
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, Bool & value);
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, UnsignedLong & value);
#ifndef __LP64__
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, Unsigned64BitsInteger & value);
#endif
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, NumericalScalar & value);
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, NumericalComplex & value);
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, String & value);
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name,  InterfaceObject & value);
  virtual void readAttribute(Pointer<InternalObject> & p_obj, const String & name, PersistentObject & value);

  /* Read an indexed value
   * @internal
   */
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, Bool & value);
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, UnsignedLong & value);
#ifndef __LP64__
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, Unsigned64BitsInteger & value);
#endif
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, NumericalScalar & value);
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, NumericalComplex & value);
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, String & value);
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, InterfaceObject & value);
  virtual void readIndexedValue(Pointer<InternalObject> & p_obj, UnsignedLong index, PersistentObject & value);
#endif


protected:

  /** Query the manager if the version is correct */
  virtual Bool canManageVersion(UnsignedLong version) const;

  /** Return the current state of the storage manager (for those having one) */
  virtual const StorageManager::InternalObject & getState() const;



private:

  struct XMLReadObject
  {
    Pointer<PersistentObject> p_obj_;
    String visibility_;
    String label_;
    XMLReadObject() : p_obj_(0), visibility_(), label_() {}
  };

  /** The file we read from/write to */
  FileName fileName_;

  /** The internal state of the manager */
  Pointer<XMLStorageManagerState> p_state_;

  /** The internal document */
  Pointer<XMLDoc> p_document_;

  /** Methods to read DOM elements */
  XMLReadObject readDOMElement();


}; /* class XMLStorageManager */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_XMLSTORAGEMANAGER_HXX */
