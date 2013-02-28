//                                               -*- C++ -*-
/**
 *  @file  t_RandomWalkMetropolisHastings_regression.cxx
 *  @brief The test file of class RandomWalkMetropolisHastings
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
 */
#include <iostream>
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;
typedef ComposedDistribution::DistributionCollection DistributionCollection;
typedef RandomWalkMetropolisHastings::CalibrationStrategyCollection CalibrationStrategyCollection;

NumericalScalar prec = 1.0;

NumericalMathFunction buildPoly(const NumericalPoint &p)
{
  const UnsignedLong d = p.getDimension();
  NumericalPoint center(d);
  NumericalPoint constant(2);
  Matrix linear(2, d);
  for ( UnsignedLong j = 0; j < d; ++ j )
    {
      linear(0, j) = p[j]; // z=p1*x1+p2*x2+p3*x3
    }
  constant[1] = 1.0; // sigma
  return LinearNumericalMathFunction( center, constant, linear );
}


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      // observations
      UnsignedLong chainDim = 3;
      UnsignedLong obsDim = 1;
      UnsignedLong outputDim = 1;

      UnsignedLong obsSize = 10;

      NumericalSample y_obs(obsSize, obsDim);
      y_obs[0][0] = -9.50794871493506;
      y_obs[1][0] = -3.83296694500105;
      y_obs[2][0] = -2.44545713047953;
      y_obs[3][0] = 0.0803625289211318;
      y_obs[4][0] = 1.01898069723583;
      y_obs[5][0] = 0.661725805623086;
      y_obs[6][0] = -1.57581204592385;
      y_obs[7][0] = -2.95308465670895;
      y_obs[8][0] = -8.8878164296758;
      y_obs[9][0] = -13.0812290405651;
      std::cout << "y_obs=" << y_obs << std::endl;

      NumericalSample p(obsSize, chainDim);
      for (UnsignedLong i = 0; i < obsSize; ++ i)
        {
          for (UnsignedLong j = 0; j < chainDim; ++ j)
            {
              p[i][j] = pow(-2 + 5.*i / 9., j);
            }
        }
      std::cout << "p=" << p << std::endl;

      NumericalMathFunction::NumericalMathFunctionCollection modelCollection;
      for (UnsignedLong i = 0; i < obsSize; ++ i)
        {
          modelCollection.add(buildPoly(p[i]));
        }
      NumericalMathFunction model(modelCollection);

      // calibration parameters
      CalibrationStrategyCollection calibrationColl(chainDim);

      // proposal distribution
      DistributionCollection proposalColl;
      for (UnsignedLong i = 0; i < chainDim; ++ i)
        {
          proposalColl.add(Uniform(-1., 1.));
        }

      // prior distribution
      NumericalPoint sigma0(chainDim, 10.);// sigma0= (10.,10.,10.)
      CorrelationMatrix Q0(chainDim);// precision matrix
      CorrelationMatrix Q0_inv(chainDim);// covariance matrix
      for ( UnsignedLong i = 0; i < chainDim; ++ i )
        {
          Q0_inv (i, i) = sigma0[i] * sigma0[i];
          Q0(i, i) = 1.0 / Q0_inv (i, i);
        }
      std::cout << "Q0=" << Q0 << std::endl;
      NumericalPoint mu0(chainDim, 0.0);// mu0 = (0.,0.,0.)
      Distribution prior(Normal( mu0, Q0_inv ) );// x0 ~ N(mu0, sigma0)
      std::cout << "x~" << prior << std::endl;

      // start from te mean x0=(0.,0.,0.)
      std::cout << "x0=" << mu0 << std::endl;

      // conditional distribution y~N(z, 1.0)
      Distribution conditional = Normal();
      std::cout << "y~" << conditional << std::endl;

      // create a metropolis-hastings sampler
      RandomWalkMetropolisHastings sampler(prior, conditional, model, y_obs, mu0, proposalColl);
      sampler.setVerbose(true);
      sampler.setThinning(4);
      sampler.setBurnIn(2000);
      sampler.setCalibrationStrategyPerComponent(calibrationColl);

      // get a realization
      NumericalPoint realization(sampler.getRealization());
      std::cout << "y1=" << realization << std::endl;

      // try to generate a sample
      UnsignedLong sampleSize = 1000;
      NumericalSample sample(sampler.getSample(sampleSize));

      NumericalPoint x_mu(sample.computeMean());
      NumericalPoint x_sigma(sample.computeStandardDeviationPerComponent());

      // print acceptance rate
      std::cout << "acceptance rate=" << sampler.getAcceptanceRate() << std::endl;

      // compute covariance
      CovarianceMatrix x_cov(sample.computeCovariance());
      Matrix P(obsSize, chainDim);
      for (UnsignedLong i = 0; i < obsSize; ++ i)
        {
          for (UnsignedLong j = 0; j < chainDim; ++ j)
            {
              P(i, j) = p[i][j];
            }
        }
      Matrix Qn(P.transpose()*P + Q0);
      SquareMatrix Qn_inv(chainDim);
      for ( UnsignedLong j = 0; j < chainDim; ++j )
        {
          NumericalPoint I_j(chainDim);
          I_j[j] = 1.0;
          NumericalPoint Qn_inv_j(Qn.solveLinearSystem(I_j));
          for ( UnsignedLong i = 0; i < chainDim; ++i )
            {
              Qn_inv(i, j) = Qn_inv_j[i];
            }
        }
      NumericalPoint sigma_exp(chainDim);
      for ( UnsignedLong i = 0; i < chainDim; ++i )
        {
          sigma_exp[i] = sqrt(Qn_inv(i, i));
        }
      NumericalPoint y_vec(obsSize);
      for ( UnsignedLong i = 0; i < obsSize; ++i )
        {
          y_vec[i] = y_obs[i][0];
        }
      NumericalPoint x_emp(Qn.solveLinearSystem(P.transpose()*y_vec));
      NumericalPoint mu_exp(Qn.solveLinearSystem(P.transpose()*P * x_emp + Q0 * mu0));

      std::cout << "sample mean=" << x_mu << std::endl;
      std::cout << "expected mean=" << mu_exp << std::endl;

      std::cout << "covariance=" << x_cov << std::endl;
      std::cout << "expected covariance=" << Qn_inv << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}