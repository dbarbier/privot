//                                               -*- C++ -*-
/**
 * @file  TTY.hxx
 * @brief This file provides definitions for tty control sequences
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
 * @author dutka
 * @date   2011-04-08 17:04:10 +0200 (Fri, 08 Apr 2011)
 */

#ifndef OPENTURNS_TTY_HXX
#define OPENTURNS_TTY_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS

struct TTY
{

  enum Color
    {
      DEFAULT = 0,
      BOLD,
      UNDERLINE,
      BLINK,
      BLACKFG,
      REDFG,
      GREENFG,
      YELLOWFG,
      BLUEFG,
      PURPLEFG,
      CYANFG,
      WHITEFG,
      BLACKBG,
      REDBG,
      GREENBG,
      YELLOWBG,
      BLUEBG,
      PURPLEBG,
      CYANBG,
      WHITEBG,
      LASTCOLOR /* for internal usage only */
    };

  /** Return a control sequence corresponding to the current color */
  static const char * GetColor( Color c );

  /** Define the colorization strategy. Flag = true if colored output */
  static void ShowColors( Bool flag );

  /** Query the colorization strategy. Return true if colored output */
  static Bool ColoredOutput();

}; // struct TTY

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TTY_HXX */
