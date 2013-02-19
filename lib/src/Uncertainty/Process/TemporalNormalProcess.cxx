//                                               -*- C++ -*-
/**
 *  @file  TemporalNormalProcess.cxx
 *  @brief A class which implements the TemporalNormalProcess process
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
 */

#include "TemporalNormalProcess.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "CovarianceMatrix.hxx"
#include "Lapack.hxx"
#include "DistFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TemporalNormalProcess);

static Factory<TemporalNormalProcess> RegisteredFactory("TemporalNormalProcess");

TemporalNormalProcess::TemporalNormalProcess(const String & name)
  : ProcessImplementation(name)
  , covarianceModel_()
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_()
  , stationaryTrendValue_()
{
  // Nothing to do
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const TrendTransform & trend,
					     const SecondOrderModel & model,
                                             const RegularGrid & timeGrid,
                                             const String & name)
  : ProcessImplementation(name)
  , covarianceModel_(model.getCovarianceModel())
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(false)
  , checkedStationaryTrend_(false)
  , trend_(trend)
  , stationaryTrendValue_()
{
  if (trend.getOutputDimension() != model.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given trend has an output dimension=" << trend.getOutputDimension() << " different from the second order model dimension=" << model.getDimension();
  setTimeGrid(timeGrid);
  setDimension(model.getDimension());
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const TrendTransform & trend,
					     const CovarianceModel & covarianceModel,
                                             const RegularGrid & timeGrid,
                                             const String & name)
  : ProcessImplementation(name)
  , covarianceModel_(covarianceModel)
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(false)
  , checkedStationaryTrend_(false)
  , trend_(trend)
  , stationaryTrendValue_()
{
  if (trend.getOutputDimension() != covarianceModel.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given trend has an output dimension=" << trend.getOutputDimension() << " different from the covariance model dimension=" << covarianceModel.getDimension();
  setTimeGrid(timeGrid);
  setDimension(covarianceModel.getDimension());
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const SecondOrderModel & model,
                                             const RegularGrid & timeGrid,
                                             const String & name)
  : ProcessImplementation(name)
  , covarianceModel_(model.getCovarianceModel())
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_(NumericalMathFunction(Description(1, "t"), Description(model.getDimension(), "0.0")))
  , stationaryTrendValue_()
{
  setTimeGrid(timeGrid);
  setDimension(model.getDimension());
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const CovarianceModel & covarianceModel,
                                             const RegularGrid & timeGrid,
                                             const String & name)
  : ProcessImplementation(name)
  , covarianceModel_(covarianceModel)
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_(NumericalMathFunction(Description(1, "t"), Description(covarianceModel.getDimension(), "0.0")))
  , stationaryTrendValue_()
{
  setTimeGrid(timeGrid);
  setDimension(covarianceModel.getDimension());
}

/* Virtual constructor */
TemporalNormalProcess * TemporalNormalProcess::clone() const
{
  return new TemporalNormalProcess(*this);
}

void TemporalNormalProcess::initialize() const
{
  // Initialization of the process
  // Get the covariance matrix (its Cholesky factor)

  CovarianceMatrix covarianceMatrix(covarianceModel_.discretizeCovariance(timeGrid_));
  const UnsignedLong fullSize(covarianceMatrix.getDimension());
  // Boolean flag to tell if the regularization is enough
  Bool continuationCondition(true);
  // Scaling factor of the matrix : M-> M + \lambda I with \lambda very small
  // The regularization is needed for fast decreasing covariance models
  const NumericalScalar startingScaling(ResourceMap::GetAsNumericalScalar("TemporalNormalProcess-StartingScaling"));
  const NumericalScalar maximalScaling(ResourceMap::GetAsNumericalScalar("TemporalNormalProcess-MaximalScaling"));
  NumericalScalar cumulatedScaling(0.0);

  NumericalScalar scaling(startingScaling);
  while (continuationCondition && (cumulatedScaling < maximalScaling))
    {
      try
        {
          choleskyFactorCovarianceMatrix_ = covarianceMatrix.computeCholesky();
          continuationCondition = false;
        }
      // If it has not yet been computed, compute it and store it
      catch (InvalidArgumentException & ex)
        {
          cumulatedScaling += scaling ;
          // Unroll the regularization to optimize the computation
          for (UnsignedLong i = 0; i < fullSize; ++i) covarianceMatrix(i, i) += scaling;
          scaling *= 2.0;
        }
    }

  if (scaling >= maximalScaling)
    throw InvalidArgumentException(HERE) << "Error; Could not compute the Cholesky factor"
                                         << " Scaling up to "  << cumulatedScaling << " was not enough";

  if (cumulatedScaling > 0.0)
    LOGWARN(OSS() <<  "Warning! Scaling up to "  << cumulatedScaling << " was needed in order to get an admissible covariance. ");

  // The process has been initialized
  isInitialized_ = true;
}

/* String converter */
String TemporalNormalProcess::__repr__() const
{
  OSS oss;
  oss << "class=" << TemporalNormalProcess::GetClassName();
  oss << " timeGrid=" << getTimeGrid()
      << " trend=" << trend_
      << " covarianceModel=" << covarianceModel_
      << " choleskyFactorCovarianceMatrix=" << choleskyFactorCovarianceMatrix_
      << " isInitialized=" << isInitialized_
      << " hasStationaryTrend=" << hasStationaryTrend_
      << " checkedStationaryTrend=" << checkedStationaryTrend_;
  return oss;
}

String TemporalNormalProcess::__str__(const String & offset) const
{
  OSS oss;
  oss << " TemporalNormalProcess=" << TemporalNormalProcess::GetClassName()
      << " dimension=" << dimension_
      << " timeGrid=" << timeGrid_.__str__(offset)
      << " trend=" << trend_.__str__(offset)
      << " covarianceModel=" << covarianceModel_.__str__(offset);
  return oss;
}

/* TimeGrid accessor */
void TemporalNormalProcess::setTimeGrid(const RegularGrid & timeGrid)
{
  checkedStationaryTrend_ = false;
  ProcessImplementation::setTimeGrid(timeGrid);
}

/* Realization accessor */
TimeSeries TemporalNormalProcess::getRealization() const
{
  if (!isInitialized_) initialize();
  // Constantes values
  const UnsignedLong size(getTimeGrid().getN());
  const UnsignedLong fullSize(choleskyFactorCovarianceMatrix_.getDimension());
  NumericalPoint gaussianPoint(fullSize);
  // N gaussian realizations
  for (UnsignedLong index = 0; index <  fullSize; ++index) gaussianPoint[index] = DistFunc::rNormal();

  // Product matrix vector using Blas
  // side tells if the triangular matrix is "Lower" or "Upper"
  char uplo('L');
  int luplo(1);

  // trans tells if the matrix is transposed or not
  char trans('N');
  int ltrans(1);

  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);

  // the dimension of the matrix
  int n(fullSize);

  // leading dimension of M
  int lda(fullSize);
  int one(1);

  // Call to optimize triangular matrix/vector product using Lapack
  DTRMV_F77(&uplo, &trans, &diag, &n, const_cast<double*>(&((*choleskyFactorCovarianceMatrix_.getImplementation())[0])), &lda, const_cast<double*>(&(gaussianPoint[0])), &one, &luplo, &ltrans, &ldiag);

  NumericalSample gaussianSample(size, dimension_);
  UnsignedLong position(0);
  for (UnsignedLong index = 0; index < size; ++ index)
    {
      for (UnsignedLong dimensionIndex = 0; dimensionIndex < dimension_; ++ dimensionIndex)
        {
          gaussianSample[index][dimensionIndex] = gaussianPoint[position] ;
          ++position;
        }
    }
  // If null trend
  if (isTrendStationary() && (stationaryTrendValue_.norm() == 0.0)) return TimeSeries(timeGrid_, gaussianSample);
  // else apply the trend
  return trend_(TimeSeries(timeGrid_, gaussianSample));
}

/* Covariance model accessor */
CovarianceModel TemporalNormalProcess::getCovarianceModel() const
{
  return covarianceModel_;
}

/* Trend accessor */
TrendTransform TemporalNormalProcess::getTrend() const
{
  return trend_;
}

/* Check if the process is stationary */
Bool TemporalNormalProcess::isStationary() const
{
  return covarianceModel_.isStationary() && isTrendStationary();
}

/* Tell if the process is trend stationary */
Bool TemporalNormalProcess::isTrendStationary() const
{
  if (!checkedStationaryTrend_) checkStationaryTrend();
  return hasStationaryTrend_;
}

/* Check if the process is trend stationary */
void TemporalNormalProcess::checkStationaryTrend() const
{
  hasStationaryTrend_ = true;
  checkedStationaryTrend_ = true;
  const RegularGrid timeGrid(getTimeGrid());
  const UnsignedLong n(timeGrid.getN());
  if (n == 0) return;
  const NumericalPoint stationaryTrendValue_((*trend_.getEvaluation())(NumericalPoint(1, timeGrid.getValue(0))));
  for (UnsignedLong i = 1; i < n; ++i)
    {
      if ((*trend_.getEvaluation())(NumericalPoint(1, timeGrid.getValue(i))) != stationaryTrendValue_)
	{
	  hasStationaryTrend_ = false;
	  return;
	}
    }
  return;
}

/* Check if the process is Normal */
Bool TemporalNormalProcess::isNormal() const
{
  return true;
}


/* Method save() stores the object through the StorageManager */
void TemporalNormalProcess::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute("covarianceModel_", covarianceModel_);
  adv.saveAttribute("choleskyFactorCovarianceMatrix_", choleskyFactorCovarianceMatrix_);
  adv.saveAttribute("isInitialized_", isInitialized_);
}

/* Method load() reloads the object from the StorageManager */
void TemporalNormalProcess::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute("covarianceModel_", covarianceModel_);
  adv.loadAttribute("choleskyFactorCovarianceMatrix_", choleskyFactorCovarianceMatrix_);
  adv.loadAttribute("isInitialized_", isInitialized_);
}

END_NAMESPACE_OPENTURNS
