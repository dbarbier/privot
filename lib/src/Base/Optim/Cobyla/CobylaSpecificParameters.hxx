//                                               -*- C++ -*-
/**
 *  @file  CobylaSpecificParameters.hxx
 *  @brief CobylaSpecificParameters is an encapsulation of the Cobyla specific
 *         parameters.
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
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 *  Id      Cobyla.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_COBYLASPECIFICPARAMETERS_HXX
#define OPENTURNS_COBYLASPECIFICPARAMETERS_HXX

#include "OTprivate.hxx"
#include "NearestPointAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class CobylaSpecificParameters
 * Specific parameters of the cobyla library
 */
class CobylaSpecificParameters
  : public PersistentObject
{

  CLASSNAME;

public:
  /** Default constructor */
  CobylaSpecificParameters();

  /** Parameters constructor */
  CobylaSpecificParameters(const NumericalScalar rhoBeg);

  /** Virtual constructor */
  virtual CobylaSpecificParameters * clone() const;

  /** RhoBeg accessor */
  NumericalScalar getRhoBeg() const;

  /** RhoBeg accessor */
  void setRhoBeg(const NumericalScalar rhoBeg);

  /** String converter */
  String __repr__() const;

private:

  NumericalScalar rhoBeg_;
}; /* class CobylaSpecificParameters */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COBYLASPECIFICPARAMETERS_HXX */
