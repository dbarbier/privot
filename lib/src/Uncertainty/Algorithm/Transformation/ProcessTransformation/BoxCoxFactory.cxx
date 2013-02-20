//                                               -*- C++ -*-
/**
 *  @file  BoxCoxFactory.cxx
 *  @brief
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
#include "BoxCoxFactory.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "TimeSeries.hxx"
#include "BoxCoxEvaluationImplementation.hxx"
#include "SpecFunc.hxx"
#include "algocobyla.h"
#include "Cobyla.hxx"
#include "Log.hxx"
#include "BoxCoxTransform.hxx"
#include "InverseBoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BoxCoxFactory
 */

CLASSNAMEINIT(BoxCoxFactory);

static Factory<BoxCoxFactory> RegisteredFactory("BoxCoxFactory");

/* Constructor with parameters */
BoxCoxFactory::BoxCoxFactory(const String & name)
  : PersistentObject(name),
    sample_(),
    scalar_(0.),
    alpha_(0.)
{
  // Nothing to do
}

BoxCoxFactory::BoxCoxFactory(const NumericalScalar alpha,
                             const String & name)
  : PersistentObject(name),
    sample_(),
    scalar_(0.),
    alpha_(0.)
{
  setAlpha(alpha);
}

/* Virtual constructor */
BoxCoxFactory * BoxCoxFactory::clone() const
{
  return new BoxCoxFactory(*this);
}

NumericalScalar BoxCoxFactory::computeLogLikelihood(const NumericalScalar & lambda) const
{
  // initial result = 0
  NumericalScalar result(0.0);
  const UnsignedLong size(sample_.getSize());
  BoxCoxEvaluationImplementation myBoxFunction(NumericalPoint(1, lambda));
  // compute the mean of the sample using the Box-Cox function
  const NumericalSample outSample(myBoxFunction(sample_));
  const NumericalScalar ratio((size - 1.0) / size);
  result -= log(outSample.computeCovariance()(0, 0) * ratio) * (size / 2);

  //result is translated
  result += (lambda - 1.0) * scalar_;
  return result;
}


/* objective function */
int BoxCoxFactory::ComputeObjectiveAndConstraint(int n,
                                                 int m,
                                                 double *x,
                                                 double *f,
                                                 double *con,
                                                 void *state)
{
  const NumericalScalar a(x[0]);

  BoxCoxFactory * factory = static_cast<BoxCoxFactory *>(state);
  *f = - factory->computeLogLikelihood( a );

  return 0;
}

/* Build the factory from data */
BoxCoxTransform * BoxCoxFactory::build(const TimeSeries & timeSeries) const
{
  const UnsignedLong size(timeSeries.getSize());
  const UnsignedLong dimension(timeSeries.getDimension());

  // output result
  NumericalPoint lambda(dimension, 0.);
  if (size == 0)
    throw InvalidArgumentException(HERE) << "Error: cannot build a Box-Cox factory from an empty time series";

  // Currently the optimization is done sequently
  // The next step will includes performance by using golden search algorithm

  // number of optimized variables
  int n(1);

  // number of constraints ==> check
  int m(0);

  // starting point
  NumericalPoint x(n);

  // cobyla rhobeg ==>  a reasonable initial change to the variables
  // cobyla rhoend ==> the required accuracy for the variables
  // if the cobyla will be the default method, then we must add these variables in the ResourceMap
  // maxfun ==> on input, the maximum number of function evaluations on output, the number of function evaluations done
  NumericalScalar rhoBeg(ResourceMap::GetAsNumericalScalar("BoxCoxFactory-DefaultRhoBeg"));
  NumericalScalar rhoEnd(ResourceMap::GetAsNumericalScalar("BoxCoxFactory-DefaultRhoEnd"));
  int maxFun(static_cast<int>(ResourceMap::GetAsNumericalScalar("BoxCoxFactory-DefaultMaxFun")));
  // verbosity level : no msg
  cobyla_message message( COBYLA_MSG_NONE );

  // use attributes to pass the data
  for (UnsignedLong d = 0; d < dimension; ++d)
    {

      sample_ = timeSeries.getSample().getMarginal(d);
      x[0] = 1.0;

      // fix scalar_ values
      scalar_ = 0.;

      for (UnsignedLong k = 0; k < size; ++k)
        {
          // translate by adding the alpha value
          sample_[k][0] += alpha_;
          scalar_ += log(sample_[k][0]);
        }

      // call cobyla algo
      int returnCode(cobyla( n, m, &x[0], rhoBeg, rhoEnd, message, &maxFun, BoxCoxFactory::ComputeObjectiveAndConstraint, (void*) this ));
      if (returnCode != 0)
        LOGWARN(OSS() << "Error solving maximum likelihood problem by cobyla method, message=" << cobyla_rc_string[returnCode - COBYLA_MINRC]);

      lambda[d] = x[0];
    }

  // return result ==> Box Cox transform
  return BoxCoxTransform(lambda).clone();
}

/* alpha accessors */
NumericalScalar BoxCoxFactory::getAlpha() const
{
  return alpha_;
}

void BoxCoxFactory::setAlpha(const NumericalScalar alpha)
{
  // alpha must be positive in theory
  if (alpha < 0)
    throw InvalidArgumentException(HERE) << "alpha should be positive or null" ;
  alpha_ = alpha;
}

/* String converter */
String BoxCoxFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << BoxCoxFactory::GetClassName();
  return oss;
}

/* String converter */
String BoxCoxFactory::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << BoxCoxFactory::GetClassName();
  return oss;
}

END_NAMESPACE_OPENTURNS
