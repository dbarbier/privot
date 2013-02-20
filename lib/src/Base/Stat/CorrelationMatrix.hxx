//                                               -*- C++ -*-
/**
 *  @file  CorrelationMatrix.hxx
 *  @brief The class CorrelationMatrix implements correlation matrices
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
 *  Id      CorrelationMatrix.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_CORRELATIONMATRIX_HXX
#define OPENTURNS_CORRELATIONMATRIX_HXX

#include "OTprivate.hxx"
#include "CovarianceMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class CorrelationMatrix
 */

class CorrelationMatrix
  : public CovarianceMatrix
{
  CLASSNAME;

public:
  typedef Collection<NumericalScalar>       NumericalScalarCollection;


  /** Default constructor */
  CorrelationMatrix();

  /** Constructor with size */
  CorrelationMatrix(UnsignedLong const dim);

  /** Constructor from external collection */
  CorrelationMatrix(const UnsignedLong dim,
                    const NumericalScalarCollection & elementsValues);

  /** Constructor with implementation */
  CorrelationMatrix(const Implementation & i);

  /** String converter */
  virtual String __repr__() const;

  /** CorrelationMatrix transpose */
  CorrelationMatrix transpose () const ;

  /** CorrelationMatrix multiplication (must have consistent dimensions) */
  CorrelationMatrix operator * (const IdentityMatrix & m) const;
  using CovarianceMatrix::operator *;

protected:


private:

}; /* class CorrelationMatrix */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMATRIX_HXX */
