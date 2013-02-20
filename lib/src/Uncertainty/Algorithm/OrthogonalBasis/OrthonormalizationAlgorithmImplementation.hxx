//                                               -*- C++ -*-
/**
 *  @file  OrthonormalizationAlgorithmImplementation.hxx
 *  @brief Top level class for the orthonormalization algorithms
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
 *  @author dutka
 *  @date   2009-09-14 14:39:35 +0200 (lun. 14 sept. 2009)
 *  Id      OrthonormalizationAlgorithmImplementation.hxx 1331 2009-09-14 12:39:35Z dutka
 */
#ifndef OPENTURNS_ORTHONORMALIZATIONALGORITHMIMPLEMENTATION_HXX
#define OPENTURNS_ORTHONORMALIZATIONALGORITHMIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Distribution.hxx"
#include "OrthogonalUniVariatePolynomial.hxx" // for the definition of Coefficients

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class OrthonormalizationAlgorithmImplementation
 */

class OrthonormalizationAlgorithmImplementation
  : public PersistentObject
{
  CLASSNAME;
public:

  typedef OrthogonalUniVariatePolynomial::Coefficients Coefficients;

  /** Default constructor with Brent solver */
  explicit OrthonormalizationAlgorithmImplementation();

  /** Constructor with parameters */
  OrthonormalizationAlgorithmImplementation(const Distribution & measure);

  /** Virtual constructor */
  virtual OrthonormalizationAlgorithmImplementation * clone() const;

  /** Calculate the coefficients of recurrence a0, a1, a2 such that
      Pn+1(x) = (a0 * x + a1) * Pn(x) + a2 * Pn-1(x) */
  virtual Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** String converter */
  virtual String __repr__() const;

  /** Measure accessor */
  Distribution getMeasure() const;
  void setMeasure(const Distribution & measure);

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  Distribution measure_;

private:

} ; /* class OrthonormalizationAlgorithmImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ORTHONORMALIZATIONALGORITHMIMPLEMENTATION_HXX */
