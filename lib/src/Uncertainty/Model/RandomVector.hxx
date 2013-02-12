//                                               -*- C++ -*-
/**
 *  @file  RandomVector.hxx
 *  @brief The class that implements all random vectors
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 *  Id      RandomVector.hxx 2608 2012-07-16 13:59:45Z schueller
 */
#ifndef OPENTURNS_RANDOMVECTOR_HXX
#define OPENTURNS_RANDOMVECTOR_HXX

#include "TypedInterfaceObject.hxx"
#include "Pointer.hxx"
#include "RandomVectorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RandomVector
 *
 * The class that implements all random vectors
 */
class RandomVector
  : public TypedInterfaceObject<RandomVectorImplementation>
{
  CLASSNAME;

public:

  /* Some typedefs for easy reading */
  typedef RandomVectorImplementation::Antecedent Antecedent;

  /** Default constructor */
  RandomVector();

  /** Default constructor */
  RandomVector(const RandomVectorImplementation & implementation,
               const String & name = DefaultName);


  /** Constructor from implementation */
  RandomVector(const Implementation & p_implementation,
               const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  RandomVector(RandomVectorImplementation * p_implementation,
               const String & name = DefaultName);
#endif

  /** Constructor for constant vector */
  RandomVector(const NumericalPoint & point,
               const String & name = DefaultName);

  /** Constructor for distribution-based vector */
  RandomVector(const Distribution & distribution,
               const String & name = DefaultName);

  /** Constructor for distribution-based conditional vector */
  RandomVector(const Distribution & distribution,
               const RandomVector & randomParameters,
               const String & name = DefaultName);

  /** Constructor for composite vector */
  RandomVector(const NumericalMathFunction & function,
               const RandomVector & antecedent,
               const String & name = DefaultName);

  /** Constructor for functional chaos vector */
  RandomVector(const FunctionalChaosResult & functionalChaosResult);


  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Description Accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Is the underlying random vector composite ? */
  Bool isComposite() const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Realization accessor */
  NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  NumericalSample getSample(const UnsignedLong size) const;

  /** Numerical sample accessor
   * @deprecated
   */
  NumericalSample getNumericalSample(const UnsignedLong size) const;

  /** Get the random vector corresponding to the i-th marginal component */
  RandomVector getMarginal(const UnsignedLong i) const;

  /** Get the marginal random vector corresponding to indices components */
  RandomVector getMarginal(const Indices & indices) const;

  /** Mean accessor */
  NumericalPoint getMean() const;

  /** Covariance accessor */
  CovarianceMatrix getCovariance() const;

  /** This method allows to access the antecedent RandomVector in case of a composite RandomVector */
  Antecedent getAntecedent() const;

  /** This method allows to access the NumericalMathFunction in case of a composite RandomVector */
  NumericalMathFunction getFunction() const;

  /** This method allows to access the Distribution in case of a usual RandomVector */
  Distribution getDistribution() const;

  /** Operator accessor */
  ComparisonOperator getOperator() const;

  /** Threshold accessor */
  NumericalScalar getThreshold() const;

protected:
  /** Constructor for event vector */
  RandomVector(const RandomVector & antecedent,
               const ComparisonOperator & op,
               const NumericalScalar threshold,
               const String & name = DefaultName);

  /** Constructor for event vector */
  explicit RandomVector(const RandomVector & antecedent,
                        const Domain & domain,
                        const String & name = DefaultName);


  /** Constructor for event vector */
  explicit RandomVector(const Process & process,
                        const Domain & domain,
                        const String & name = DefaultName);

}; /* class RandomVector */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMVECTOR_HXX */
