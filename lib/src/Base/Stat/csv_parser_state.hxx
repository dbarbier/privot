//                                               -*- C++ -*-
/**
 *  @file  csv_parser_state.hxx
 *  @brief The definition of the internal state of the CSV parser
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
 *  Id      csv_parser_state.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_CSV_PARSER_STATE_HXX
#define OPENTURNS_CSV_PARSER_STATE_HXX

#include <string>
#include "NumericalPoint.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS



struct CSVParserState
{

  enum FieldType { NotSet, RealField, StringField };

  FieldType                     Type;
  std::string                   St;
  double                        Val;
  bool                          errors;
  NumericalPoint                Point;
  Description                   Header;
  FileName                      theFileName;

  CSVParserState() : Type(NotSet), St(), Val(0.), errors(false), Point(), Header(), theFileName() {}

}; /* end struct CSVParserState */


END_NAMESPACE_OPENTURNS


#endif /* OPENTURNS_CSV_PARSER_STATE_HXX */
