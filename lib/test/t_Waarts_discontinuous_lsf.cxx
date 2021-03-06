//                                               -*- C++ -*-
/**
 *  @file  t_Waarts_discontunuous_lsf.cxx
 *  @brief The test file for Waarts discontinuous limit state function
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
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

NumericalScalar clean(NumericalScalar in)
{
  if (fabs(in) < 1.e-10) in = 0.0;
  return in;
}

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();
  try
    {

      Description inputFunction(2);
      inputFunction[0] = "x1";
      inputFunction[1] = "x2";

      Description outputFunction(1);
      outputFunction[0] = "G";

      Description formulas(outputFunction.getSize());
      formulas[0] = "if( x2 <= x1,-0.5+sqrt(x1-x2),-0.5 )";

      NumericalMathFunction EtatLimite(inputFunction, outputFunction, formulas);

      UnsignedLong dim = EtatLimite.getInputDimension();
      fullprint << dim << std::endl;

      //   #########################################################################################################;
      //   # Probabilistic model;
      //   #########################################################################################################;

      NumericalPoint mean(dim, 0.0);
      mean[0] = 15.0;
      mean[1] = 5.0;

      NumericalPoint sigma(dim, 0.0);
      sigma[0] = 2.5;
      sigma[1] = 0.5;

      CorrelationMatrix CORR(dim);
      Normal myDistribution(mean, sigma, CORR);

      NumericalPoint start(myDistribution.getMean());
      CovarianceMatrix Covariance = myDistribution.getCovariance();

      //   #########################################################################################################;
      //   # Limit state;
      //   #########################################################################################################;

      RandomVector vect(myDistribution);

      RandomVector output(EtatLimite, vect);

      Event myEvent(output, Less(), 0.0);

      //   #########################################################################################################;
      //   # Calculs;
      //   #########################################################################################################;
      //   ;
      //   #########################################################################################################;
      //   # FORM/SORM Cobyla;
      Cobyla myCobyla;
      myCobyla.setSpecificParameters(CobylaSpecificParameters());
      myCobyla.setMaximumIterationsNumber(1000);
      myCobyla.setMaximumAbsoluteError(1.0e-10);
      myCobyla.setMaximumRelativeError(1.0e-10);
      myCobyla.setMaximumResidualError(1.0e-10);
      myCobyla.setMaximumConstraintError(1.0e-10);

      FORM myAlgoC(myCobyla, myEvent, start);
      SORM myAlgoC2(myCobyla, myEvent, start);

      myAlgoC.run();
      myAlgoC2.run();

      FORMResult resultC(myAlgoC.getResult());
      SORMResult resultC2(myAlgoC2.getResult());

      //   #########################################################################################################;
      //   # FORM/SORM Abdo Rackwitz;
      AbdoRackwitz myAbdoRackwitz;
      myAbdoRackwitz.setSpecificParameters(AbdoRackwitzSpecificParameters());
      myAbdoRackwitz.setMaximumIterationsNumber(100);
      myAbdoRackwitz.setMaximumAbsoluteError(1.0e-10);
      myAbdoRackwitz.setMaximumRelativeError(1.0e-10);
      myAbdoRackwitz.setMaximumResidualError(1.0e-10);
      myAbdoRackwitz.setMaximumConstraintError(1.0e-10);

      FORM myAlgoAR(myAbdoRackwitz, myEvent, start);
      SORM myAlgoAR2(myAbdoRackwitz, myEvent, start);

      myAlgoAR.run();
      myAlgoAR2.run();

      FORMResult resultAR(myAlgoAR.getResult());
      SORMResult resultAR2(myAlgoAR2.getResult());

      //   #########################################################################################################;
      //   # Monte Carlo;
      NumericalScalar CoV_MC = 0.5;
      MonteCarlo myMC(myEvent);
      myMC.setMaximumOuterSampling(10000000);
      myMC.setBlockSize(1000);
      myMC.setMaximumCoefficientOfVariation(CoV_MC);
      myMC.run();

      //   #########################################################################################################;
      //   # LHS;
      NumericalScalar CoV_LHS = 0.1;
      LHS myLHS(myEvent);
      myLHS.setMaximumOuterSampling(100000);
      myLHS.setBlockSize(100);
      myLHS.setMaximumCoefficientOfVariation(CoV_LHS);
      myLHS.run();
      /*  ;

          #########################################################################################################;

          #########################################################################################################;
          # Results;
          #########################################################################################################;

          #########################################################################################################;
          # FORM/SORM Cobyla*/;
          NumericalScalar PfC = resultC.getEventProbability();
          NumericalScalar Beta_generalizedC = resultC.getGeneralisedReliabilityIndex();
          NumericalPoint u_starC = resultC.getStandardSpaceDesignPoint();
          NumericalPoint x_starC = resultC.getPhysicalSpaceDesignPoint();
          Bool PtC = resultC.getIsStandardPointOriginInFailureSpace();
          NumericalPoint gammaC = resultC.getImportanceFactors();
          NumericalPoint gammaCC = resultC.getImportanceFactors(true);
          NumericalScalar beta_hasoferC = resultC.getHasoferReliabilityIndex();
          Analytical::Sensitivity SensitivityC = resultC.getEventProbabilitySensitivity();

          NumericalScalar PFBreitC2 = resultC2.getEventProbabilityBreitung();
          NumericalScalar BetaBreitC2 = resultC2.getGeneralisedReliabilityIndexBreitung();
          NumericalScalar PFHBC2 = resultC2.getEventProbabilityHohenBichler();
          NumericalScalar BetaHBC2 = resultC2.getGeneralisedReliabilityIndexHohenBichler();
          NumericalScalar PFTvedtC2 = resultC2.getEventProbabilityTvedt();
          NumericalScalar BetaTvedtC2 = resultC2.getGeneralisedReliabilityIndexTvedt();
          NumericalPoint CurvC2 = resultC2.getSortedCurvatures();
          NumericalPoint u_starC2 = resultC2.getStandardSpaceDesignPoint();
          NumericalPoint x_starC2 = resultC2.getPhysicalSpaceDesignPoint();
          Bool PtC2 = resultC2.getIsStandardPointOriginInFailureSpace();
          NumericalPoint gammaC2 = resultC2.getImportanceFactors();
          NumericalPoint gammaCC2 = resultC2.getImportanceFactors(true);
          NumericalScalar beta_hasoferC2 = resultC2.getHasoferReliabilityIndex();

          //   #########################################################################################################;
          //   # FORM/SORM Abdo Rackwitz;
          NumericalScalar PfAR = resultAR.getEventProbability();
          NumericalScalar Beta_generalizedAR = resultAR.getGeneralisedReliabilityIndex();
          NumericalPoint u_starAR = resultAR.getStandardSpaceDesignPoint();
          NumericalPoint x_starAR = resultAR.getPhysicalSpaceDesignPoint();
          Bool PtAR = resultAR.getIsStandardPointOriginInFailureSpace();
          NumericalPoint gammaAR = resultAR.getImportanceFactors();
          NumericalPoint gammaCAR = resultAR.getImportanceFactors(true);
          NumericalScalar beta_hasoferAR = resultAR.getHasoferReliabilityIndex();
          Analytical::Sensitivity SensitivityAR = resultAR.getEventProbabilitySensitivity();

          NumericalScalar PFBreitAR2 = resultAR2.getEventProbabilityBreitung();
          NumericalScalar BetaBreitAR2 = resultAR2.getGeneralisedReliabilityIndexBreitung();
          NumericalScalar PFHBAR2 = resultAR2.getEventProbabilityHohenBichler();
          NumericalScalar BetaHBAR2 = resultAR2.getGeneralisedReliabilityIndexHohenBichler();
          NumericalScalar PFTvedtAR2 = resultAR2.getEventProbabilityTvedt();
          NumericalScalar BetaTvedtAR2 = resultAR2.getGeneralisedReliabilityIndexTvedt();
          NumericalPoint CurvAR2 = resultAR2.getSortedCurvatures();
          NumericalPoint u_starAR2 = resultAR2.getStandardSpaceDesignPoint();
          NumericalPoint x_starAR2 = resultAR2.getPhysicalSpaceDesignPoint();
          Bool PtAR2 = resultAR2.getIsStandardPointOriginInFailureSpace();
          NumericalPoint gammaAR2 = resultAR2.getImportanceFactors();
          NumericalPoint gammaCAR2 = resultAR2.getImportanceFactors(true);
          NumericalScalar beta_hasoferAR2 = resultAR2.getHasoferReliabilityIndex();

          //   ######################################/*###################################################################;
          //   # Monte Carlo*/;
          SimulationResult ResultMC = myMC.getResult();
          NumericalScalar PFMC = ResultMC.getProbabilityEstimate();
          NumericalScalar CVMC = ResultMC.getCoefficientOfVariation();
          NumericalScalar Variance_PF_MC = ResultMC.getVarianceEstimate();
          NumericalScalar length90MC = ResultMC.getConfidenceLength(0.90);

          //   #########################################################################################################;
          //   # LHS;
          SimulationResult ResultLHS = myLHS.getResult();
          NumericalScalar PFLHS = ResultLHS.getProbabilityEstimate();
          NumericalScalar CVLHS = ResultLHS.getCoefficientOfVariation();
          NumericalScalar Variance_PF_LHS = ResultLHS.getVarianceEstimate();
          NumericalScalar length90LHS = ResultLHS.getConfidenceLength(0.90);

          //   #########################################################################################################;
          //   # Printting;
          //   #########################################################################################################;
          fullprint <<  "" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "***************************************** FORM  COBYLA *****************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "event probability =" << PfC << std::endl;
          fullprint <<  "generalized reliability index =" << Beta_generalizedC << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < u_starC.getDimension(); i++)
            fullprint << "standard space design point =" << u_starC[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < x_starC.getDimension(); i++)
            fullprint << "physical space design point =" << x_starC[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint << "is standard point origin in failure space? " << PtC << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < gammaC.getDimension(); i++)
            fullprint << "importance factors =" << gammaC[i] << std::endl;
          for (UnsignedLong i = 0; i < gammaCC.getDimension(); i++)
            fullprint << "importance factors (classical)=" << gammaCC[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Hasofer reliability index =" << beta_hasoferC << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < SensitivityAR.getSize(); ++i)
            for (UnsignedLong j = 0; j < SensitivityAR[i].getDimension(); ++j)
              fullprint <<  "Pf sensitivity =" << i << j << SensitivityC[i][j] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "************************************** FORM ABDO RACKWITZ **************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "event probability =" << PfAR << std::endl;
          fullprint <<  "generalized reliability index =" << Beta_generalizedAR << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < u_starAR.getDimension(); i++)
            fullprint << "standard space design point =" << u_starAR[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < x_starAR.getDimension(); i++)
            fullprint << "physical space design point =" << x_starAR[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint << "is standard point origin in failure space? " << PtAR << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < gammaAR.getDimension(); i++)
            fullprint << "importance factors =" << gammaAR[i] << std::endl;
          for (UnsignedLong i = 0; i < gammaCAR.getDimension(); i++)
            fullprint << "importance factors (classical)=" << gammaCAR[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Hasofer reliability index =" << beta_hasoferAR << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < SensitivityAR.getSize(); ++i)
            for (UnsignedLong j = 0; j < SensitivityAR[i].getDimension(); ++j)
              fullprint <<  "Pf sensitivity =" << i << j << SensitivityAR[i][j] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "***************************************** SORM  COBYLA *****************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Breitung event probability =" << PFBreitC2 << std::endl;
          fullprint <<  "Breitung generalized reliability index =" << BetaBreitC2 << std::endl;
          fullprint <<  "HohenBichler event probability =" << PFHBC2 << std::endl;
          fullprint <<  "HohenBichler generalized reliability index =" << BetaHBC2 << std::endl;
          fullprint <<  "Tvedt event probability =" << PFTvedtC2 << std::endl;
          fullprint <<  "Tvedt generalized reliability index =" << BetaTvedtC2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < CurvC2.getDimension(); i++)
            fullprint << "sorted curvatures =" << clean(CurvC2[i]) << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < u_starC2.getDimension(); i++)
            fullprint << "standard space design point =" << u_starC2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < x_starC2.getDimension(); i++)
            fullprint << "physical space design point =" << x_starC2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint << "is standard point origin in failure space? " << PtC2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < gammaC2.getDimension(); i++)
            fullprint << "importance factors =" << gammaC2[i] << std::endl;
          for (UnsignedLong i = 0; i < gammaCC2.getDimension(); i++)
            fullprint << "importance factors (classical)=" << gammaCC2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Hasofer reliability index =" << beta_hasoferC2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "************************************** SORM ABDO RACKWITZ **************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Breitung event probability =" << PFBreitAR2 << std::endl;
          fullprint <<  "Breitung generalized reliability index =" << BetaBreitAR2 << std::endl;
          fullprint <<  "HohenBichler event probability =" << PFHBAR2 << std::endl;
          fullprint <<  "HohenBichler generalized reliability index =" << BetaHBAR2 << std::endl;
          fullprint <<  "Tvedt event probability =" << PFTvedtAR2 << std::endl;
          fullprint <<  "Tvedt generalized reliability index =" << BetaTvedtAR2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < CurvAR2.getDimension(); i++)
            fullprint << "sorted curvatures =" << clean(CurvAR2[i]) << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < u_starAR2.getDimension(); i++)
            fullprint << "standard space design point =" << u_starAR2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < x_starAR2.getDimension(); i++)
            fullprint << "physical space design point =" << x_starAR2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint << "is standard point origin in failure space? " << PtAR2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          for (UnsignedLong i = 0; i < gammaAR2.getDimension(); i++)
            fullprint << "importance factors =" << gammaAR2[i] << std::endl;
          for (UnsignedLong i = 0; i < gammaCAR2.getDimension(); i++)
            fullprint << "importance factors (classical)=" << gammaCAR2[i] << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Hasofer reliability index =" << beta_hasoferAR2 << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "**************************************** MONTE CARLO *******************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Pf estimation =" << PFMC << std::endl;
          fullprint <<  "Pf Variance estimation =" << Variance_PF_MC << std::endl;
          fullprint <<  "CoV =" << CVMC << std::endl;
          fullprint <<  "90% Confidence Interval =" << length90MC << std::endl;
          fullprint <<  "CI at 90% =[" << PFMC - 0.5 * length90MC << ";" << PFMC + 0.5 * length90MC << "]" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "******************************************* L H S **********************************************" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
          fullprint <<  "Pf estimation =" << PFLHS << std::endl;
          fullprint <<  "Pf Variance estimation =" << Variance_PF_LHS << std::endl;
          fullprint <<  "CoV =" << CVLHS << std::endl;
          fullprint <<  "90% Confidence Interval =" << length90LHS << std::endl;
          fullprint <<  "CI at 90% =[" << PFLHS - 0.5 * length90LHS << ";" << PFLHS + 0.5 * length90LHS << "]" << std::endl;
          fullprint <<  "************************************************************************************************" << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;

}
