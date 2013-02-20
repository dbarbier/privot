//                                               -*- C++ -*-
/**
 *  @file  AnalyticalParser.hxx
 *  @brief A math expression parser
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 *  Id      AnalyticalParser.hxx 1649 2010-11-09 12:44:00Z dutka
 */

#ifndef OPENTURNS_ANALYTICALPARSER_HXX
#define OPENTURNS_ANALYTICALPARSER_HXX

#include "Object.hxx"
#include "muParser.h"

BEGIN_NAMESPACE_OPENTURNS


class AnalyticalParser : public mu::Parser
{

public:
  /** Default constructor */
  AnalyticalParser();

protected:
  static mu::value_type ASinh( mu::value_type v );
  static mu::value_type ACosh( mu::value_type v );
  static mu::value_type ATanh( mu::value_type v );
  static mu::value_type Ln( mu::value_type v );
  static mu::value_type Log2( mu::value_type v );
  static mu::value_type LnGamma( mu::value_type v );
  static mu::value_type Gamma( mu::value_type v );
  static mu::value_type Erf( mu::value_type v );
  static mu::value_type Erfc( mu::value_type v );
  static mu::value_type Abs( mu::value_type v );
  static mu::value_type Cbrt( mu::value_type v );
  static mu::value_type J0( mu::value_type v );
  static mu::value_type J1( mu::value_type v );
  static mu::value_type Y0( mu::value_type v );
  static mu::value_type Y1( mu::value_type v );
  static mu::value_type Rint( mu::value_type v );

}; /* class AnalyticalParser */


END_NAMESPACE_OPENTURNS

#endif // OPENTURNS_ANALYTICALPARSER_HXX
