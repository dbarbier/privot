//                                               -*- C++ -*-
/**
 *  @file  t_UserDefinedCovarianceModel_std.cxx
 *  @brief The test file of class UserDefinedCovarianceModel
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
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include <iostream>
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      /* Default constructor */
      UserDefinedCovarianceModel myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;

      // Default dimension parameter to evaluate the model
      const UnsignedLong dimension(1);

      /* Amplitude values */
      NumericalPoint amplitude(dimension);

      /* Scale values */
      NumericalPoint scale(dimension);
      /* Spatial correclation */
      CorrelationMatrix spatialCorrelation(dimension);
      for (UnsignedLong index = 0 ; index < dimension; ++index)
        {
          // constant amplitude
          amplitude[index] = 1.0 ;
          scale[index] = (index + 1.0) / dimension ;
          if (index > 0) spatialCorrelation(index, index - 1) = 1.0 / index;
        }

      /* Sample a CauchyModel */
      ExponentialModel referenceModel(amplitude, scale, spatialCorrelation);

      UnsignedLong size(20);
      RegularGrid timeGrid(0.0, 0.1, size);
      const UnsignedLong collectionSize(size * (size + 1) / 2);
      UserDefinedCovarianceModel::CovarianceMatrixCollection covarianceCollection(collectionSize);
      UnsignedLong k(0);
      for (UnsignedLong i = 0; i < timeGrid.getN(); ++i)
        {
          const NumericalScalar t(timeGrid.getValue(i));
          for (UnsignedLong j = i; j < timeGrid.getN(); ++j)
            {
              const NumericalScalar s(timeGrid.getValue(j));
              covarianceCollection[k] = referenceModel.computeCovariance(t, s);
              k++;
            }
        }

      /* Create a UserDefinedCovarianceModel */
      UserDefinedCovarianceModel myModel(timeGrid, covarianceCollection);
      fullprint << "myModel= " << myModel << std::endl;

      /* Sample the UserDefinedCovarianceModel */
      for (UnsignedLong i = 0; i < timeGrid.getN(); ++i)
        {
          const NumericalScalar t(timeGrid.getValue(i));
          for (UnsignedLong j = i; j < timeGrid.getN(); ++j)
            {
              const NumericalScalar s(timeGrid.getValue(j));
              // We look for cov(s,t) ==> when adding to the collection, we compute cov(t,s)
              // Because of symmetry, we check the right index computation
              fullprint << "myModel = " << myModel.computeCovariance(s, t)(0, 0) << ", referenceModel = " << referenceModel.computeCovariance(s, t)(0 , 0) << std::endl;
            }
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
