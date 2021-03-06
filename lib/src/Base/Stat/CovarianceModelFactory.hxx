//                                               -*- C++ -*-
/**
 *  @file   CovarianceModelFactory.hxx
 *  @brief Top-level class for all spectral model factories
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
 *  @author: $LastChangedBy: schueller $
 *  @date:   $LastChangedDate: 2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012) $
 *  Id:      $Id: CovarianceModelFactory.hxx 2392 2012-02-17 18:35:43Z schueller $
 */
#ifndef OPENTURNS_COVARIANCEMODELFACTORY_HXX
#define OPENTURNS_COVARIANCEMODELFACTORY_HXX

#include "CovarianceModelFactoryImplementation.hxx"
#include "PersistentObject.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class  CovarianceModelFactory
 */
class  CovarianceModelFactory
  : public TypedInterfaceObject<CovarianceModelFactoryImplementation>
{
  CLASSNAME;
public:

  typedef CovarianceModelFactoryImplementation::Implementation    Implementation;

  /** Default constructor */
  CovarianceModelFactory(const String & name = OT::DefaultName);

  /** Copy constructors */
  CovarianceModelFactory(const CovarianceModelFactoryImplementation & implementation,
                         const String & name = OT::DefaultName);

  /** Constructor from implementation */
  CovarianceModelFactory(const Implementation & p_implementation,
                         const String & name = OT::DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  CovarianceModelFactory(CovarianceModelFactoryImplementation * p_implementation,
                         const String & name = OT::DefaultName);
#endif

  /** Virtual constructor */
  virtual  CovarianceModelFactory * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */

  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */
  /** Build a a spectral model based on a sample */
  virtual CovarianceModel build(const ProcessSample & sample) const;

  /** Build a a spectral model based on a TimeSeries */
  virtual CovarianceModel build(const TimeSeries & timeSerie) const;

}; /* class  CovarianceModelFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMODELFACTORY_HXX */
