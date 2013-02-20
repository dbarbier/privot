//                                               -*- C++ -*-
/**
 *  @file  HistogramPair.hxx
 *  @brief The HistogramPair class.
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
 *  @author lebrun
 *  @date   2010-11-21 19:45:57 +0100 (dim. 21 nov. 2010)
 *  Id      HistogramPair.hxx 1657 2010-11-21 18:45:57Z lebrun
 */
#ifndef OPENTURNS_HISTOGRAMPAIR_HXX
#define OPENTURNS_HISTOGRAMPAIR_HXX

#include "OTprivate.hxx"
#include "StorageManager.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class HistogramPair
 *
 * The HistogramPair class.
 */
class HistogramPair
  : public PersistentObject
{
  CLASSNAME;
public:

  HistogramPair();
  HistogramPair(const NumericalScalar width,
                const NumericalScalar height);

  virtual HistogramPair * clone() const;

  Bool operator == (const HistogramPair & other) const;

  void setWidth(const NumericalScalar width);

  void setHeight(const NumericalScalar height);

  NumericalScalar getWidth() const;

  NumericalScalar getHeight() const;

  NumericalScalar getSurface() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  virtual String __repr__() const;

private:

  /** Width of the bin */
  NumericalScalar width_;
  /** Height of the bin */
  NumericalScalar height_;
  /** Surface of the bin */
  NumericalScalar surface_;

}; /* end class HistogramPair */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HISTOGRAMPAIR_HXX */
