//                                               -*- C++ -*-
/**
 *  @file  Cloud.hxx
 *  @brief Cloud class for cloud plots
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
 *  Id      Cloud.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_CLOUD_HXX
#define OPENTURNS_CLOUD_HXX

#include "OTprivate.hxx"
#include "DrawableImplementation.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Cloud
 *
 * The class describing a cloud plot
 * Instance of Drawable
 */

class Cloud : public DrawableImplementation
{
  CLASSNAME;
public:

  typedef Collection<NumericalComplex>               NumericalComplexCollection;

  /** Default constructor */
  Cloud(const NumericalSample & data,
        const String & legend = "");

  /** Constructor from complex numbers */
  Cloud(const NumericalComplexCollection & data,
        const String & legend = "");

  /** Contructor from 2 data sets */
  Cloud(const NumericalSample & dataX,
        const NumericalSample & dataY,
        const String & legend = "");

  /** Constructor with parameters */
  Cloud(const NumericalSample & data,
        const String & color,
        const String & pointStyle,
        const String & legend = "");

  /** String converter */
  String __repr__() const;

  /** Draw method */
  String draw() const;

  /** Clone method */
  Cloud * clone() const;

  /** Check fo data validity */
  Bool isValidData(const NumericalSample & data) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

private:

  Cloud() {};
  friend class Factory<Cloud>;

}; /* class Cloud */

END_NAMESPACE_OPENTURNS
#endif /* OPENTURNS_CLOUD_HXX */
