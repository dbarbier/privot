//                                               -*- C++ -*-
/**
 *  @file  KFold.hxx
 *  @brief K-Fold cross validation
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
 *  Id      KFold.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_KFOLD_HXX
#define OPENTURNS_KFOLD_HXX

#include "ResourceMap.hxx"
#include "FittingAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class KFold
 *
 * Creates all the possible training sets by removing P samples from the complete set
 */
class KFold
  : public FittingAlgorithmImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  KFold(const UnsignedLong k = ResourceMap::GetAsUnsignedLong( "KFold-DefaultK" ));

  /** Virtual constructor */
  KFold * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Perform cross-validation */
  virtual NumericalScalar run(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & basis) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** K accessor */
  void setK(const UnsignedLong p);
  UnsignedLong getK() const;

protected:

  /** The data is split into k subsamples */
  UnsignedLong k_;

}; /* class KFold */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KFOLD_HXX */
