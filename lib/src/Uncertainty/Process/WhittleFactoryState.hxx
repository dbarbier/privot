//                                               -*- C++ -*-
/**
 *  @file  WhittleFactoryState.hxx
 *  @brief This class stores the current state of the fitting step in the WhittleFactory class.
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
 *  @author lebrun
 *  @date   2012-04-05 17:05:15 +0200 (jeu. 05 avril 2012)
 *  Id      WhittleFactoryState.hxx 2447 2012-04-05 15:05:15Z lebrun
 */
#ifndef OPENTURNS_WHITTLEFACTORYSTATE_HXX
#define OPENTURNS_WHITTLEFACTORYSTATE_HXX

#include "PersistentObject.hxx"
#include "ARMA.hxx"
#include "ARMACoefficients.hxx"
#include "NumericalPoint.hxx"
#include "WhiteNoise.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class WhittleFactoryState
 *
 * The class implements the state of the WhittleFactory class that results form a particular fitting
 */
class WhittleFactoryState
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Default constructor */
  WhittleFactoryState(const String & name = DefaultName);

  /** Default constructor */
  WhittleFactoryState(const UnsignedLong p,
                      const NumericalPoint & theta,
                      const NumericalScalar sigma2,
                      const NumericalPoint & informationCriteria,
                      const RegularGrid & timeGrid,
                      const String & name = DefaultName);

  /** Virtual constructor */
  WhittleFactoryState * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** P accessor */
  UnsignedLong getP() const;

  /** Q accessor */
  UnsignedLong getQ() const;

  /** Theta accessor */
  NumericalPoint getTheta() const;

  /** Sigma2 accessor */
  NumericalScalar getSigma2() const;

  /** AR coefficients accessor */
  ARMACoefficients getARCoefficients() const;

  /** MA coefficients accessor */
  ARMACoefficients getMACoefficients() const;

  /** White noise accessor */
  WhiteNoise getWhiteNoise() const;

  /** ARMA process accessor */
  ARMA getARMA() const;

  /** Information criteria accessor */
  NumericalPoint getInformationCriteria() const;

  /** Time grid accessor */
  RegularGrid getTimeGrid() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

  /* The AR order of the ARMA process */
  UnsignedLong p_;

  /* The flat vector of coefficients of the ARMA process */
  NumericalPoint theta_;

  /* The variance of the white noise */
  NumericalScalar sigma2_;

  /* The information criteria of the fitting */
  NumericalPoint informationCriteria_;

  /* The time grid associated with the estimation */
  RegularGrid timeGrid_;

}; /* class WhittleFactoryState */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WHITTLEFACTORYSTATE_HXX */
