//                                               -*- C++ -*-
/**
 *  @file  Classifier.hxx
 *  @brief Classification algorithm interface
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 *  Id      Classifier.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_CLASSIFIER_HXX
#define OPENTURNS_CLASSIFIER_HXX

#include "TypedInterfaceObject.hxx"
#include "NumericalSample.hxx"
#include "ClassifierImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class Classifier
 *
 * Classification algorithm interface
 */
class Classifier
  : public TypedInterfaceObject<ClassifierImplementation>
{
  CLASSNAME;

public:

  typedef TypedInterfaceObject<ClassifierImplementation>::Implementation Implementation;

  /** Default constructor */
  Classifier();

  /** Constructor from implementation */
  Classifier(const ClassifierImplementation & implementation);

  /** Constructor from implementation */
  Classifier(const Implementation & p_implementation);

  /** Constructor from sample */
  Classifier(const NumericalSample & inS, const Indices & outC);

  /** Associate a point with a class */
  UnsignedLong classify(const NumericalPoint & inP) const;
  Indices classify(const NumericalSample & inS) const;

  /** Grade a point as if it were associated to a class */
  NumericalScalar grade(const NumericalPoint & inP,
                        UnsignedLong outC) const;
  NumericalPoint grade(const NumericalSample & inS,
                       const Indices & outC) const;

  /** Input space dimension */
  UnsignedLong getDimension() const;

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

}; /* class Classifier */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CLASSIFIER_HXX */
