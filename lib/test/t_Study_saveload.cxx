//                                               -*- C++ -*-
/**
 *  @file  t_Study_saveload.cxx
 *  @brief The test file of class Study for standard methods
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
 */
#include <iostream>
#include <sstream>

#include "OTconfig.hxx"
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

template<class T>
static inline
void compare(const T & savedInstance, const Study & study, const String & tag = String(""))
{
  OStream fullprint(std::cout);
  T loadedInstance;
  const String className = loadedInstance.getClassName();
  String label = tag;
  if (label.empty())
    {
      label = className;
      label[0] = tolower(label[0]);
    }
  study.fillObject(label, loadedInstance);
  fullprint << "saved  " << className << " = " << savedInstance  << std::endl;
  fullprint << "loaded " << className << " = " << loadedInstance << std::endl;
}



int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  const char fileName[] = "myStudy.xml";

  try
    {

      // Create a Study Object
      Study study;
      study.setStorageManager(XMLStorageManager(fileName));


      // Add a PersistentObject to the Study (here a NumericalPoint)
      NumericalPoint numericalPoint(3, 0.);
      numericalPoint[0] = 10.;
      numericalPoint[1] = 11.;
      numericalPoint[2] = 12.;
      numericalPoint.setName("point");
      study.add("Good", numericalPoint);

      // Add another PersistentObject to the Study (here a NumericalSample)
      NumericalSample numericalSample(3, 2);
      NumericalPoint p2(2, 0.);
      p2[0] = 100.;
      p2[1] = 200.;
      numericalSample[0] = p2;
      p2.setName("One");
      NumericalPoint p3(2, 0.);
      p3[0] = 101.;
      p3[1] = 201.;
      numericalSample[1] = p3;
      p3.setName("Two");
      NumericalPoint p4(2, 0.);
      p4[0] = 102.;
      p4[1] = 202.;
      numericalSample[2] = p4;
      p4.setName("Three");
      study.add("mySample", numericalSample);

      // Create a NumericalPoint that we will try to reinstaciate after reloading
      NumericalPoint namedNumericalPoint(2, 1000.);
      namedNumericalPoint.setName("point");
      study.add("namedNumericalPoint", namedNumericalPoint);

      // Add a point with a description
      NumericalPointWithDescription numericalPointWithDescription(numericalPoint);
      Description desc = numericalPointWithDescription.getDescription();
      desc[0] = "x";
      desc[1] = "y";
      desc[2] = "z";
      numericalPointWithDescription.setDescription(desc);
      study.add("pDesc", numericalPointWithDescription);

      // Add a matrix
      Matrix matrix(2, 3);
      matrix(0, 0) = 0;
      matrix(0, 1) = 1;
      matrix(0, 2) = 2;
      matrix(1, 0) = 3;
      matrix(1, 1) = 4;
      matrix(1, 2) = 5;
      study.add("m", matrix);


      // Add a Staircase
      Staircase staircase;
      {
        // Instanciate one distribution object
        UnsignedLong dim(1);
        NumericalPoint meanPoint(dim, 1.0);
        meanPoint[0] = 0.5;
        NumericalPoint sigma(dim, 1.0);
        sigma[0] = 2.0;
        CorrelationMatrix R = IdentityMatrix(dim);
        Normal distribution1(meanPoint, sigma, R);

        // Instanciate another distribution object
        meanPoint[0] = -1.5;
        sigma[0] = 4.0;
        Normal distribution2(meanPoint, sigma, R);

        // Test for sampling
        UnsignedLong size = 2000;
        UnsignedLong nPoints = 20;
        NumericalSample sample1(distribution1.getSample( size ));
        NumericalSample sample2(distribution2.getSample( size ));

        // Construct empirical CDF for each sample
        NumericalSample data1(nPoints, 2), data2(nPoints, 2);
        NumericalPoint cursor1(2);
        NumericalPoint cursor2(2);
        NumericalScalar count1;
        NumericalScalar count2;

        for(UnsignedLong i = 0; i < nPoints; i++)
          {
            cursor1[0] = 13.*i / nPoints - 6.5;
            count1 = 0;
            cursor2[0] = 24.*i / nPoints - 13.5;
            count2 = 0;

            for(UnsignedLong j = 0; j < size; j++)
              {
                if(sample1[j][0] < cursor1[0]) count1++;
                if(sample2[j][0] < cursor2[0]) count2++;
              }

            cursor1[1] = count1 / size;
            cursor2[1] = count2 / size;

            data1[i] = cursor1;
            data2[i] = cursor2;
          }
        staircase = Staircase (data2, "red", "dashed", "S", "eCDF2, pat=S");
      }
      study.add("staircase", staircase);

      // Create a NearestPointAlgorithm::Result
      NearestPointAlgorithm::Result nearestPointAlgorithmResult(NumericalPoint(4, 1.0), 15, 1e-10, 2e-10, 3e-10, 4e-10);
      nearestPointAlgorithmResult.setName("nearestPointAlgorithmResult");
      study.add("nearestPointAlgorithmResult", nearestPointAlgorithmResult);

      // Create a Simulation::Result
      SimulationResult simulationResult(Event(), 0.5, 0.01, 150, 4);
      simulationResult.setName("simulationResult");
      study.add("simulationResult", simulationResult);

      // Create a Beta distribution
      Beta beta(3.0, 5.0, -1.0, 4.0);
      study.add("beta", beta);

      // Create an Exponential distribution
      Exponential exponential(3.0, -2.0);
      study.add("exponential", exponential);

      // Create a Gamma distribution
      Gamma gamma(1.5, 3.0, -2.0);
      study.add("gamma", gamma);

      // Create a Geometric distribution
      Geometric geometric(0.15);
      study.add("geometric", geometric);

      // Create a IndependentCopula distribution
      IndependentCopula independentCopula(5);
      study.add("independentCopula", independentCopula);

      // Create a KernelSmoothing algorithm */
      KernelSmoothing kernelSmoothing;
      kernelSmoothing.build(independentCopula.getSample(20));
      study.add("kernelSmoothing", kernelSmoothing);

      // Create a Logistic distribution
      Logistic logistic(0.5, 1.5);
      study.add("logistic", logistic);

      // Create a LogNormal distribution
      LogNormal logNormal(0.5, 1.5, -1.0);
      study.add("logNormal", logNormal);

      // Create an InverseNormal
      InverseNormal inverseNormal(0.5, 2.0);
      study.add("inverseNormal", inverseNormal);

      // Create a Student
      Student student(5.0, -0.5, 2.0);
      study.add("student", student);

      // Create a Triangular
      Triangular triangular(-0.5, 1.5, 2.5);
      study.add("triangular", triangular);

      // Create a TruncatedDistribution
      TruncatedDistribution truncatedDistribution(Normal(2.0, 1.5), 1.0, 4.0);
      study.add("truncatedDistribution", truncatedDistribution);

      // Create a TruncatedNormal
      TruncatedNormal truncatedNormal(0.5, 3.0, -2.0, 2.0);
      study.add("truncatedNormal", truncatedNormal);

      // Create an UserDefined
      UserDefined userDefined;
      {
        UserDefined::UserDefinedPairCollection collection(3);
        NumericalPoint point(1);
        point[0] = 1.0;
        collection[0] = UserDefinedPair(point, 0.3);
        point[0] = 2.0;
        collection[1] = UserDefinedPair(point, 0.1);
        point[0] = 3.0;
        collection[2] = UserDefinedPair(point, 0.6);
        userDefined = UserDefined(collection);
      }
      study.add("userDefined", userDefined);

      // Create a Weibull distribution
      Weibull weibull(2.0, 1.5, -0.5);
      study.add("weibull", weibull);

      // Create a NormalCopula distribution
      CorrelationMatrix R(3);
      R(0, 1) = 0.5;
      R(1, 2) = 0.5;
      NormalCopula normalCopula(R);
      study.add("normalCopula", normalCopula);

      // Create a Uniform distribution
      Uniform uniform(-1.5, 2.0);
      study.add("uniform", uniform);

      // Create a ComposedDistribution
      ComposedDistribution::DistributionCollection collection;
      collection.add(beta);
      collection.add(gamma);
      collection.add(logistic);
      ComposedDistribution composedDistribution(collection, normalCopula);
      study.add("composedDistribution", composedDistribution);

      // Create an analytical NumericalMathFunction
      Description input(3);
      input[0] = "a";
      input[1] = "b";
      input[2] = "c";
      Description output(3);
      output[0] = "squaresum";
      output[1] = "prod";
      output[2] = "complex";
      Description formulas(output.getSize());
      formulas[0] = "a+b+c";
      formulas[1] = "a-b*c";
      formulas[2] = "(a+2*b^2+3*c^3)/6";
      NumericalMathFunction analytical(input, output, formulas);
      analytical.setName("analytical");
      study.add("analytical", analytical);

      // Create an Event Object
      Event event;
      {
        NumericalPoint point(3);
        point[0] = 101;
        point[1] = 202;
        point[2] = 303;
        RandomVector vect(point);
        RandomVector output(analytical.getMarginal(0), vect);
        event = Event (output, Less(), 50);
      }
      study.add("event", event);

      // Create a StandardEvent Object
      StandardEvent standardEvent(event);
      study.add("standardEvent", standardEvent);

      // Create a QuadraticCumul algorithm
      QuadraticCumul quadraticCumul;
      {
        RandomVector antecedent(IndependentCopula(analytical.getInputDimension()));
        antecedent.setName("antecedent");
        RandomVector composite(analytical, antecedent);
        composite.setName("composite");
        quadraticCumul = QuadraticCumul(composite);
        quadraticCumul.setName("quadraticCumul");
        quadraticCumul.getMeanFirstOrder();
        quadraticCumul.getMeanSecondOrder();
        quadraticCumul.getCovariance();
      }

      study.add("quadraticCumul", quadraticCumul);

      // Create an AbdoRackwitz algorithm
      AbdoRackwitz abdoRackwitz;
      abdoRackwitz.setMaximumIterationsNumber(100);
      abdoRackwitz.setMaximumAbsoluteError(1.0e-10);
      abdoRackwitz.setMaximumRelativeError(1.0e-10);
      abdoRackwitz.setMaximumResidualError(1.0e-10);
      abdoRackwitz.setMaximumConstraintError(1.0e-10);
      abdoRackwitz.setSpecificParameters(AbdoRackwitzSpecificParameters());
      study.add("abdoRackwitz", abdoRackwitz);

      // Create a SQP algorithm
      SQP sqp;
      sqp.setLevelValue(3.0);
      sqp.setMaximumIterationsNumber(100);
      sqp.setMaximumAbsoluteError(1.0e-10);
      sqp.setMaximumRelativeError(1.0e-10);
      sqp.setMaximumResidualError(1.0e-10);
      sqp.setMaximumConstraintError(1.0e-10);
      study.add("sqp", sqp);

      // Create a TNC algorithm
      TNC tnc;
      {
        TNCSpecificParameters specific;
        NumericalPoint startingPoint(3, 1.0);
        Interval bounds(NumericalPoint(3, -3.0), NumericalPoint(3, 5.0));
        tnc = TNC(specific, analytical, bounds, startingPoint, TNC::Result::MINIMIZATION);
      }
      study.add("tnc", tnc);

      // Create a SORM algorithm
      SORM sorm(abdoRackwitz, event, NumericalPoint(3, 4.));
      study.add("sorm", sorm);

      // Create a FORMResult
      FORMResult formResult;
      // Create a SORMResult
      SORMResult sormResult;
      {
        Description input2(2);
        input2[0] = "x";
        input2[1] = "y";
        Description output2(1);
        output2[0] = "d";
        Description formula2(1);
        formula2[0] = "y^2-x";
        NumericalMathFunction model(input2, output2, formula2);
        model.setName("sum");
        RandomVector input3(Normal(2));
        input3.setName("input");
        RandomVector output3(model, input3);
        output3.setName("output");
        Event event(output3, Greater(), 1.0);
        event.setName("failureEvent");
        NumericalPoint designPoint(2, 0.0);
        designPoint[0] = 1.0;
        formResult = FORMResult(NumericalPoint(2, 1.0), event, false);
        formResult.setName("formResult");
        formResult.getImportanceFactors();
        formResult.getImportanceFactors(true);
        formResult.getEventProbabilitySensitivity();

        sormResult = SORMResult (NumericalPoint(2, 1.0), event, false);
        sormResult.setName("sormResult");
        sormResult.getEventProbabilityBreitung();
        sormResult.getEventProbabilityHohenBichler();
        sormResult.getEventProbabilityTvedt();
        sormResult.getGeneralisedReliabilityIndexBreitung();
        sormResult.getGeneralisedReliabilityIndexHohenBichler();
        sormResult.getGeneralisedReliabilityIndexTvedt();
      }
      study.add("formResult", formResult);
      study.add("sormResult", sormResult);

      // Create an ARMACoefficients
      ARMACoefficients armaCoefficients;
      {
        const UnsignedLong dim(2);
        SquareMatrix squareMatrix1(dim);
        squareMatrix1(0, 0) = 1.;
        squareMatrix1(1, 0) = 2.;
        squareMatrix1(0, 1) = 3.;
        squareMatrix1(1, 1) = 4.;
        double s = 3.;
        SquareMatrix squareMatrix2 = squareMatrix1.operator * (s) ;
        double t = 1.5;
        SquareMatrix squareMatrix3 = squareMatrix1.operator / (t) ;
        const UnsignedLong size(3);
        armaCoefficients = ARMACoefficients(size, dim);
        armaCoefficients[0] = squareMatrix1 ;
        armaCoefficients[1] = squareMatrix2 ;
        armaCoefficients[2] = squareMatrix3 ;
      }
      study.add("armaCoefficients", armaCoefficients);

      // Create an ARMAState
      ARMAState armaState;
      {
        const UnsignedLong dim(1);
        const UnsignedLong p(6);
        const UnsignedLong q(4);
        NumericalSample valuesX(p, dim);
        NumericalSample valuesEpsilon(q, dim);
        for(UnsignedLong j = 0 ; j < dim ; ++j)
          {
            for(UnsignedLong i = 0 ; i < p ; ++i)
              {
                valuesX[i][j] = 2.0 * i + 3.0 * j + 1.0;
              }
            for(UnsignedLong i = 0 ; i < q ; ++i)
              {
                valuesEpsilon[i][j] = RandomGenerator::Generate() ;
              }
          }
        armaState = ARMAState(valuesX, valuesEpsilon);
      }
      study.add("armaState", armaState);

      // Create a RegularGrid
      RegularGrid regularGrid;
      {
        const UnsignedLong points(8);
        const NumericalScalar tMin(0.0);
        const NumericalScalar tStep(1.0 / (points - 1));
        regularGrid = RegularGrid(tMin, tStep, points);
      }
      study.add("regularGrid", regularGrid);

      // Create a WhiteNoise
      WhiteNoise whiteNoise;
      {
        Distribution dist1 = Normal(0.0, 0.01);
        Distribution dist2 = Normal(0.0, 0.02);
        ComposedDistribution::DistributionCollection aCollection;
        aCollection.add( dist1 );
        aCollection.add( dist2 );
        Distribution dist  = ComposedDistribution(aCollection);
        whiteNoise = WhiteNoise(dist);
        whiteNoise.setTimeGrid(regularGrid);
      }
      study.add("whiteNoise", whiteNoise);

      // Create an WhittleFactory
      WhittleFactory whittleFactory;
      {
        const UnsignedLong p(1);
        const UnsignedLong q(2);
        whittleFactory = WhittleFactory( p, q );
      }
      study.add("whittleFactory", whittleFactory);

      // Create an ARMA
      ARMA arma;
      {
        const UnsignedLong dim(2);
        SquareMatrix squareMatrix1(dim);
        squareMatrix1(0, 0) = 0.2 ;
        squareMatrix1(1, 0) = 0.3 ;
        squareMatrix1(0, 1) = 0.7 ;
        squareMatrix1(1, 1) = 0.4 ;
        SquareMatrix squareMatrix2(dim) ;
        squareMatrix2(0, 0) = 0.1;
        squareMatrix2(1, 0) = 0.0;
        squareMatrix2(0, 1) = 0.0;
        squareMatrix2(1, 1) = 0.5;
        const UnsignedLong p(1);
        const UnsignedLong q(1);
        ARMACoefficients coefficientsP(p, dim);
        coefficientsP[0] = squareMatrix1 ;
        ARMACoefficients coefficientsQ(q, dim);
        coefficientsQ[0] = squareMatrix2 ;
        arma = ARMA(coefficientsP, coefficientsQ, whiteNoise);
      }
      study.add("arma", arma);

      // Create a Basis, BasisSequence
      Basis basis;
      BasisSequence basisSequence;
      {
        const UnsignedLong dimension = 3;
        Description input;
        Description output[dimension];
        for ( UnsignedLong j = 0; j < dimension; ++ j )
          {
            OSS oss;
            oss << "x" << j;
            String in(oss);
            input.add(in);
            output[j].add(in);
          }
        Collection<NumericalMathFunction> coll;
        Indices cumulBasis;
        Indices partialBasis[dimension];
        for ( UnsignedLong j = 0; j < dimension; ++ j )
          {
            NumericalMathFunction ei(input, output[j], output[j]);
            coll.add(ei);
            cumulBasis.add(j);
            partialBasis[j] = cumulBasis;
          }

        basis = Basis(coll);
        basisSequence = BasisSequence(basis);
        for ( UnsignedLong j = 0; j < dimension; ++ j )
          {
            basisSequence.add(partialBasis[j]);
          }
      }
      study.add("basis", basis);
      study.add("basisSequence", basisSequence);

      // Create an UniVariatePolynomial
      UniVariatePolynomial uniVariatePolynomial;
      {
        UniVariatePolynomial::Coefficients coefficients(3);
        coefficients[0] = -2.5;
        coefficients[1] = 3.5;
        coefficients[2] = -1.2;
        uniVariatePolynomial = UniVariatePolynomial(coefficients);
      }
      study.add("uniVariatePolynomial", uniVariatePolynomial);

      // Create a BoundConstrainedAlgorithmImplementationResult
      BoundConstrainedAlgorithmImplementationResult boundConstrainedAlgorithmImplementationResult;
      {
        UnsignedLong dim(4);
        NumericalPoint optimizer(dim, 1.0);
        NumericalScalar optimalValue(5.0);
        BoundConstrainedAlgorithmImplementationResult::OptimizationProblem optimization(BoundConstrainedAlgorithmImplementationResult::MINIMIZATION);
        UnsignedLong evaluationsNumber(10);
        NumericalScalar absoluteError(1e-6);
        NumericalScalar relativeError(1e-7);
        NumericalScalar objectiveError(1e-8);
        NumericalScalar constraintError(1e-10);
        boundConstrainedAlgorithmImplementationResult = BoundConstrainedAlgorithmImplementationResult(optimizer, optimalValue, optimization, evaluationsNumber, absoluteError, relativeError, objectiveError, constraintError);
      }
      study.add("boundConstrainedAlgorithmImplementationResult", boundConstrainedAlgorithmImplementationResult);

      // Create a Burr
      Burr burr(1.5, 4.5);
      study.add("burr", burr);



      // Create a CauchyModel
      CauchyModel cauchyModel;
      // Create an ExponentialCauchy
      ExponentialCauchy exponentialCauchy;
      // Create an ExponentialModel
      ExponentialModel exponentialModel;
      {
        const UnsignedLong defaultDimension(1);
        const UnsignedLong highDimension(3);
        NumericalPoint amplitude(highDimension, 1.0);
        NumericalPoint scale(highDimension, 1.0);
        CorrelationMatrix spatialCorrelation(highDimension);
        for (UnsignedLong index = 0 ; index < highDimension; ++index)
          {
            amplitude[index] = 1.0 ;
            scale[index] = (index + 1.0) / defaultDimension ;
            if (index > 0) spatialCorrelation(index, index - 1) = 1.0 / index;
          }
        cauchyModel = CauchyModel (amplitude, scale, spatialCorrelation);
        exponentialCauchy = ExponentialCauchy (amplitude, scale, spatialCorrelation);
        exponentialModel = ExponentialModel (amplitude, scale, spatialCorrelation);
      }
      study.add("cauchyModel", cauchyModel);
      study.add("exponentialCauchy", exponentialCauchy);
      study.add("exponentialModel", exponentialModel);

      // Create an ExponentialModel
      SecondOrderModel secondOrderModel(exponentialModel, cauchyModel);
      study.add("secondOrderModel", secondOrderModel);

      // Create a SpectralNormalProcess
      SpectralNormalProcess spectralNormalProcess(exponentialCauchy, regularGrid);
      study.add("spectralNormalProcess", spectralNormalProcess);

      // Create an  UserDefinedSpectralModel
      UserDefinedSpectralModel userDefinedSpectralModel;
      {
        UnsignedLong size(5);
        UserDefinedSpectralModel::HermitianMatrixCollection dspCollection(size);
        RegularGrid frequencyGrid(0.0, 2.0 / size, size);
        for (UnsignedLong i = 0; i < size; ++i)
          dspCollection[i] = cauchyModel.computeSpectralDensity(frequencyGrid.getValue(i));
        userDefinedSpectralModel = UserDefinedSpectralModel(frequencyGrid, dspCollection);
      }
      study.add("userDefinedSpectralModel", userDefinedSpectralModel);

      // Create a TemporalNormalProcess
      TemporalNormalProcess temporalNormalProcess(exponentialCauchy, regularGrid);
      study.add("temporalNormalProcess", temporalNormalProcess);

      // Create a CompositeProcess Object
      CompositeProcess compositeProcess;
      {
        NumericalScalar Tmin(0.0);
        NumericalScalar deltaT(0.1);
        UnsignedLong steps(11);
        RegularGrid timeGrid(Tmin, deltaT, steps);
        ARMA myARMAProcess;
        myARMAProcess.setTimeGrid(timeGrid);
        Description inputDescription(1, "x");
        Description formula(1, "2 * x + 5");
        NumericalMathFunction myOneDimensionalFunction(inputDescription, formula);
        SpatialFunction myFunction(myOneDimensionalFunction);
        compositeProcess = CompositeProcess (myFunction, myARMAProcess);
      }
      study.add("compositeProcess", compositeProcess);

      // Create a Domain Object
      Domain domain;
      {
        UnsignedLong dim(2);
        NumericalPoint a(dim, -1.0);
        NumericalPoint b(dim, 2.0);
        domain  = Interval(a, b);
      }
      study.add("domain", domain);

      // Create an EventDomainImplementation
      EventDomainImplementation eventDomainImplementation;
      {
        UnsignedLong dim(2);
        Normal distribution(dim);
        RandomVector X(distribution);
        Description inVars(dim);
        for (UnsignedLong i = 0; i < dim; ++i) inVars[i] = OSS() << "x" << i;
        NumericalMathFunction model(inVars, inVars);
        CompositeRandomVector Y(model, X);
        Interval domain(dim);
        eventDomainImplementation = EventDomainImplementation(Y, domain);
      }
      study.add("eventDomainImplementation", eventDomainImplementation);

      // Create an EventProcess
      EventProcess eventProcess;
      {
        UnsignedLong dim(2);
        Normal distribution(dim);

        WhiteNoise X(distribution);
        Interval domain(dim);
        eventProcess = EventProcess(X, domain);
      }
      study.add("eventProcess", eventProcess);

      // Create a ConstantStep
      ConstantStep constantStep;
      // Create a BlendedStep
      BlendedStep blendedStep;
      {
        UnsignedLong dimension = 2;
        NumericalPoint epsilon( dimension, 1e-5 );
        constantStep = ConstantStep(epsilon);
        NumericalPoint eta( dimension, 1.0 );
        blendedStep = BlendedStep(epsilon, eta);
      }
      study.add("constantStep", constantStep);
      study.add("blendedStep", blendedStep);

      // Create a FisherSnedecor
      FisherSnedecor fisherSnedecor(5.5, 10.5);
      study.add("fisherSnedecor", fisherSnedecor);

      // Create a FunctionalBasisProcess
      FunctionalBasisProcess functionalBasisProcess;
      {
        UnsignedLong basisDimension(10);
        Basis basis(basisDimension);
        Collection<Distribution> coefficients(basisDimension);
        for (UnsignedLong i = 0; i < basisDimension; ++i)
          {
            basis[i] = NumericalMathFunction("x", String(OSS() << "sin(" << i << "*x)"));
            coefficients[i] = Normal(0.0, (1.0 + i));
          }
        functionalBasisProcess = FunctionalBasisProcess(ComposedDistribution(coefficients), basis);
      }
      study.add("functionalBasisProcess", functionalBasisProcess);

      // Create a SquareMatrix
      SquareMatrix squareMatrix(2);
      squareMatrix(0, 0) = 1. ;
      squareMatrix(1, 0) = 2. ;
      squareMatrix(0, 1) = 3. ;
      squareMatrix(1, 1) = 4. ;
      study.add("squareMatrix", squareMatrix);

      // Create a SymmetricMatrix
      SymmetricMatrix symmetricMatrix(2);
      symmetricMatrix(0, 0) = 1. ;
      symmetricMatrix(1, 0) = 2. ;
      symmetricMatrix(0, 1) = 3. ;
      symmetricMatrix(1, 1) = 4. ;
      study.add("symmetricMatrix", symmetricMatrix);

      // Create a Tensor
      Tensor tensor(2, 2, 3);
      tensor.setName("tensor1");
      tensor(0, 0, 0) = 1. ;
      tensor(1, 0, 0) = 2. ;
      tensor(0, 1, 0) = 3. ;
      tensor(1, 1, 0) = 4. ;
      tensor(0, 0, 1) = 5. ;
      tensor(1, 0, 1) = 6. ;
      tensor(0, 1, 1) = 7. ;
      tensor(1, 1, 1) = 8. ;
      tensor(0, 0, 2) = 9. ;
      tensor(1, 0, 2) = 10. ;
      tensor(0, 1, 2) = 11. ;
      tensor(1, 1, 2) = 12. ;
      study.add("tensor", tensor);

      // Create a SymmetricTensor
      SymmetricTensor symmetricTensor(2, 3);
      symmetricTensor.setName("symtensor1");
      symmetricTensor(0, 0, 0) = 1. ;
      symmetricTensor(1, 0, 0) = 2. ;
      symmetricTensor(1, 1, 0) = 3. ;
      symmetricTensor(0, 0, 1) = 4. ;
      symmetricTensor(1, 0, 1) = 5. ;
      symmetricTensor(1, 1, 1) = 6. ;
      symmetricTensor(0, 0, 2) = 7. ;
      symmetricTensor(1, 0, 2) = 8. ;
      symmetricTensor(1, 1, 2) = 9. ;
      study.add("symmetricTensor", symmetricTensor);

      // Create a SquareComplexMatrix
      SquareComplexMatrix squareComplexMatrix(2);
      squareComplexMatrix(0, 0) = NumericalComplex(1.0, 1.0) ;
      squareComplexMatrix(0, 1) = NumericalComplex(3.0, 1.0) ;
      squareComplexMatrix(1, 0) = NumericalComplex(0.0, 1.0) ;
      squareComplexMatrix(1, 1) = NumericalComplex(5.0, 1.0) ;
      study.add("squareComplexMatrix", squareComplexMatrix);

      // Create a TriangularComplexMatrix
      TriangularComplexMatrix triangularComplexMatrix(2);
      triangularComplexMatrix(0, 0) = 1. ;
      triangularComplexMatrix(1, 0) = NumericalComplex(3., 1.0) ;
      triangularComplexMatrix(1, 1) = 4. ;
      study.add("triangularComplexMatrix", triangularComplexMatrix);

      // Create an HermitianMatrix
      HermitianMatrix hermitianMatrix(2);
      study.add("hermitianMatrix", hermitianMatrix);

      // Create a LHS
      LHS lhs(event);
      lhs.setMaximumOuterSampling(250);
      lhs.setBlockSize(4);
      lhs.setMaximumCoefficientOfVariation(0.1);
      study.add("lhs", lhs);

      // Create a LogUniform
      LogUniform logUniform(-0.5, 1.5);
      study.add("logUniform", logUniform);

      // Create a Mixture
      Mixture mixture;
      {
        UnsignedLong dimension(3);
        NumericalPoint meanPoint(dimension, 1.0);
        meanPoint[0] = 0.5;
        meanPoint[1] = -0.5;
        NumericalPoint sigma(dimension, 1.0);
        sigma[0] = 2.0;
        sigma[1] = 3.0;
        CorrelationMatrix R(dimension);
        for (UnsignedLong i = 1; i < dimension; i++)
          {
            R(i, i - 1) = 0.5;
          }
        // Create a collection of distribution
        Mixture::DistributionCollection aCollection;

        aCollection.add( Normal(meanPoint, sigma, R) );
        meanPoint += NumericalPoint(dimension, 1.0);
        aCollection.add( Normal(meanPoint, sigma, R) );
        meanPoint += NumericalPoint(dimension, 1.0);
        aCollection.add( Normal(meanPoint, sigma, R) );

        mixture = Mixture(aCollection, NumericalPoint(aCollection.getSize(), 2.0));
      }
      study.add("mixture", mixture);

      // Create a MixtureClassifier
      MixtureClassifier mixtureClassifier(mixture);
      study.add("mixtureClassifier", mixtureClassifier);

      // Create a MonteCarlo
      MonteCarlo monteCarlo(event);
      monteCarlo.setMaximumOuterSampling(250);
      monteCarlo.setBlockSize(4);
      monteCarlo.setMaximumCoefficientOfVariation(0.1);
      study.add("monteCarlo", monteCarlo);

      // Create a QuasiMonteCarlo
      QuasiMonteCarlo quasiMonteCarlo(event);
      study.add("quasiMonteCarlo", quasiMonteCarlo);

      // Create a RandomizedLHS
      RandomizedLHS randomizedLHS(event);
      study.add("randomizedLHS", randomizedLHS);

      // Create a RandomizedLHS
      RandomizedQuasiMonteCarlo randomizedQuasiMonteCarlo(event);
      study.add("randomizedQuasiMonteCarlo", randomizedQuasiMonteCarlo);

      // Create a RandomWalk
      RandomWalk randomWalk;
      {
        Distribution dist = Uniform();
        NumericalPoint origin(dist.getDimension());
        randomWalk = RandomWalk(origin, dist);
      }
      study.add("randomWalk", randomWalk);

      // Create a TestResult
      TestResult testResult;
      {
        String type = "testResult";
        Bool testBool = 1;
        NumericalScalar testVal = 0.372;
        NumericalScalar testThres = 0.42;
        testResult = TestResult(type, testBool, testVal, testThres);
      }
      study.add("testResult", testResult);

      // Create a StrongMaximumTest
      StrongMaximumTest strongMaximumTest;
      {
        UnsignedLong dim(analytical.getInputDimension());
        double seuil(10);
        NumericalPoint designPoint(dim, 0.0);
        double C(0.3);
        designPoint[0] = - sqrt(seuil) + C;
        NumericalPoint pseudoDesignPoint(dim, 0.0);
        pseudoDesignPoint[0] = sqrt(seuil) + C;
        NumericalScalar importanceLevel(0.01);
        NumericalScalar accuracyLevel(2);
        NumericalScalar confidenceLevel(0.999999);
        strongMaximumTest = StrongMaximumTest(standardEvent, designPoint, importanceLevel, accuracyLevel, confidenceLevel);
      }
      study.add("strongMaximumTest", strongMaximumTest);

      // Create an ExpertMixture
      ExpertMixture expertMixture;
      {
        Mixture::DistributionCollection aCollection;
        CorrelationMatrix R(2);
        R(0, 1) = -0.99;
        NumericalPoint mean(2);
        mean[0] = -1.0;
        mean[1] = 1.0;
        aCollection.add( Normal(mean, NumericalPoint(2, 1.0), R) );
        R(0, 1) = 0.99;
        mean[0] = 1.0;
        aCollection.add( Normal(mean, NumericalPoint(2, 1.0), R) );
        Mixture distribution(aCollection, NumericalPoint(aCollection.getSize(), 1.0));
        MixtureClassifier classifier(distribution);
        Basis experts(0);
        experts.add(NumericalMathFunction("x", "-x"));
        experts.add(NumericalMathFunction("x", "x"));
        expertMixture = ExpertMixture(experts, classifier);
      }
      study.add("expertMixture", expertMixture);

      // Create a CovarianceModel
      UserDefinedCovarianceModel userDefinedCovarianceModel;
      UserDefinedStationaryCovarianceModel userDefinedStationaryCovarianceModel;
      {
        const UnsignedLong dimension(1);
        NumericalPoint amplitude(dimension);
        NumericalPoint scale(dimension);
        CorrelationMatrix spatialCorrelation(dimension);
        for (UnsignedLong index = 0 ; index < dimension; ++index)
          {
            amplitude[index] = 1.0 ;
            scale[index] = (index + 1.0) / dimension ;
            if (index > 0) spatialCorrelation(index, index - 1) = 1.0 / index;
          }
        ExponentialModel referenceModel(amplitude, scale, spatialCorrelation);
        UnsignedLong size(20);
        RegularGrid timeGrid(0.0, 0.1, size);
        {
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
          userDefinedCovarianceModel = UserDefinedCovarianceModel(timeGrid, covarianceCollection);
        }
        {
          UserDefinedStationaryCovarianceModel::CovarianceMatrixCollection covarianceCollection(size);
          for (UnsignedLong i = 0; i < size; ++i)
            {
              const NumericalScalar t(timeGrid.getValue(i));
              covarianceCollection[i] = referenceModel.computeCovariance(0, t);
            }
          userDefinedStationaryCovarianceModel = UserDefinedStationaryCovarianceModel(timeGrid, covarianceCollection);
        }
      }
      study.add("userDefinedCovarianceModel", userDefinedCovarianceModel);
      study.add("userDefinedStationaryCovarianceModel", userDefinedStationaryCovarianceModel);



      fullprint << "Save the study..." << std::endl;
      study.save();


      // Create a Study Object
      Study study2;
      study2.setStorageManager(XMLStorageManager(fileName));
      study2.load();
      //     fullprint << "saved  Study = " << study << std::endl;
      //     fullprint << "loaded Study = " << study2    << std::endl;

      NumericalPoint namedNumericalPoint2;
      study2.fillObjectByName( namedNumericalPoint2, "point" );
      fullprint << "saved  NumericalPoint = " << namedNumericalPoint  << std::endl;
      fullprint << "loaded NumericalPoint = " << namedNumericalPoint2 << std::endl;


      // Type
      compare<NumericalPoint >( numericalPoint, study2, "Good");
      compare<NumericalPointWithDescription >( numericalPointWithDescription, study2, "pDesc");
      compare<Matrix >( matrix, study2, "m");
      compare<SquareMatrix>(squareMatrix, study2 );
      compare<SymmetricMatrix>(symmetricMatrix, study2 );
      compare<Tensor>(tensor, study2 );
      compare<SymmetricTensor>(symmetricTensor, study2 );
      compare<SquareComplexMatrix>(squareComplexMatrix, study2 );
      compare<TriangularComplexMatrix>(triangularComplexMatrix, study2 );
      compare<HermitianMatrix>(hermitianMatrix, study2 );

      // Graph
      compare<Staircase >( staircase, study2 );

      // Stat
      compare<NumericalSample >( numericalSample, study2, "mySample");

      // Func
      compare<NumericalMathFunction >( analytical, study2 , "analytical");

      // Optim
      compare<NearestPointAlgorithm::Result >( nearestPointAlgorithmResult, study2, "nearestPointAlgorithmResult");
      compare<AbdoRackwitz >( abdoRackwitz, study2 );
      compare<SQP >( sqp, study2, "sqp" );
      compare<TNC >( tnc, study2, "tnc" );
      compare<BoundConstrainedAlgorithmImplementationResult >( boundConstrainedAlgorithmImplementationResult, study2 );

      // Model
      compare<Event >( event, study2 );
      compare<StandardEvent >( standardEvent, study2 );

      // Distribution
      compare<Beta >( beta, study2 );
      compare<Exponential >( exponential, study2 );
      compare<Gamma >( gamma, study2 );
      compare<Geometric >( geometric, study2 );
      compare<IndependentCopula >( independentCopula, study2 );
      compare<KernelSmoothing >( kernelSmoothing, study2 );
      compare<Logistic >( logistic, study2 );
      compare<LogNormal >( logNormal, study2 );
      compare<NormalCopula >( normalCopula, study2 );
      compare<Uniform >( uniform, study2 );
      compare<ComposedDistribution >( composedDistribution, study2 );
      compare<Burr >( burr, study2 );
      compare<FisherSnedecor >( fisherSnedecor, study2 );
      compare<InverseNormal>(inverseNormal, study2 );
      compare<LogUniform>(logUniform, study2 );
      compare<Mixture>(mixture, study2 );
      compare<Student>(student, study2 );
      compare<Triangular>(triangular, study2 );
      compare<TruncatedDistribution>(truncatedDistribution, study2 );
      compare<TruncatedNormal>(truncatedNormal, study2 );
      compare<UserDefined>(userDefined, study2 );
      compare<Weibull>(weibull, study2 );

      // Simulation
      compare<MonteCarlo>(monteCarlo, study2 );
      compare<LHS>(lhs, study2, "lhs");
      compare<QuasiMonteCarlo>(quasiMonteCarlo, study2 );
      compare<RandomizedLHS>(randomizedLHS, study2 );
      compare<RandomizedQuasiMonteCarlo>(randomizedQuasiMonteCarlo, study2 );
      compare<SimulationResult >( simulationResult, study2 );

      // Analytical
      compare<SORM >( sorm, study2, "sorm");
      compare<FORMResult >( formResult, study2 , "formResult");
      compare<SORMResult >( sormResult, study2,  "sormResult");

      compare<QuadraticCumul >( quadraticCumul, study2 );

      // Process
      compare<ARMACoefficients >( armaCoefficients, study2 , "armaCoefficients");
      compare<ARMAState >( armaState, study2, "armaState");
      compare<WhiteNoise >( whiteNoise, study2 );
      compare<WhittleFactory >( whittleFactory, study2 );
      compare<ARMA >( arma, study2, "arma");
      compare<Basis >( basis, study2 );
      compare<BasisSequence >( basisSequence, study2 );
      compare<UniVariatePolynomial >( uniVariatePolynomial, study2 );
      compare<CauchyModel >( cauchyModel, study2 );
      compare<ExponentialCauchy >( exponentialCauchy, study2 );
      compare<ExponentialModel >( exponentialModel, study2 );
      compare<SecondOrderModel >( secondOrderModel, study2 );
      compare<CompositeProcess >( compositeProcess, study2 );
      compare<SpectralNormalProcess >( spectralNormalProcess, study2 );
      compare<TemporalNormalProcess >( temporalNormalProcess, study2 );
      compare<Domain >( domain, study2 );
      compare<EventDomainImplementation >( eventDomainImplementation, study2 );
      compare<EventProcess >( eventProcess, study2 );
      compare<ConstantStep >( constantStep, study2 );
      compare<BlendedStep >( blendedStep, study2 );
      compare<FunctionalBasisProcess >( functionalBasisProcess, study2 );
      compare<RandomWalk>(randomWalk, study2 );
      compare<RegularGrid>(regularGrid, study2 );
      compare<MixtureClassifier>(mixtureClassifier, study2 );

      // StatTest
      compare<TestResult>( testResult, study2 );
      compare<StrongMaximumTest>( strongMaximumTest, study2 );

      // ExpertMixture
      compare<ExpertMixture>( expertMixture, study2 );

      // CovarianceModel
      compare<UserDefinedCovarianceModel>( userDefinedCovarianceModel, study2 );
      compare<UserDefinedStationaryCovarianceModel>( userDefinedStationaryCovarianceModel, study2 );

      // cleanup
      Os::Remove(fileName);
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }




  return ExitCode::Success;
}
