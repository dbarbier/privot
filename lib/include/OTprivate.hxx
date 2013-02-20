//                                               -*- C++ -*-
/**
 *  @file  OTprivate.hxx
 *  @brief The internal header file of Open TURNS
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
 *  @author dutka
 *  @date   2009-02-09 16:06:43 +0100 (Mon, 09 Feb 2009)
 *  Id      OTprivate.hxx 1118 2009-02-09 15:06:43Z dutka
 */

/**
 * @mainpage The Open TURNS project
 *
 * This documentation is intented to be used by experienced programmers.
 * It shows the API of almost all the objects defined by the C++ core library
 * of Open TURNS.
 *
 * There may be undocumented features or objects. This is a mistake. You can help
 * us improve the software by noticing us of these caveats or by documenting them yourself
 * as a friendly user.
 *
 * As visible either in the directory structure or in the namespace naming,
 * the library is divided in two main pieces:
 *  - the Base part
 *  - the Uncertainty part
 *
 * \section Base
 * The Base part of the library provides containers and algorithms of common usage, such
 * as multidimensional points (\link OT::NumericalPoint NumericalPoint\endlink),
 * aka vectors, samples
 * (\link OT::NumericalSample NumericalSample\endlink)
 * or functions (\link OT::NumericalMathFunction NumericalMathFunction\endlink).
 * Every concept in Open TURNS is multidimensional unless clearly expressed or documented.
 *
 * \section Uncertainty
 * The Uncertainty part of the library provides objects and algorithms designed to achieve
 * probabilistic analyses. Here you will find things like distributions (\link OT::Distribution
 * Distribution\endlink or \link OT::Normal Normal\endlink),
 * random vectors (\link OT::RandomVector RandomVector\endlink), algorithms
 * (\link OT::FORM FORM\endlink or \link OT::MonteCarlo
 * MonteCarlo\endlink) or statistical tests (\link OT::FittingTest FittingTest\endlink).
 *
 * As you will probably see, a particular attention was paid to precisely name classes and objects
 * according to their usage. We hope this will help you enter the library.
 *
 * The Open TURNS core team.
 */
#ifndef OPENTURNS_OTPRIVATE_HXX
#define OPENTURNS_OTPRIVATE_HXX

/*
 * This include defines the debug level in all source code.
 */
#include "OTdebug.h"

/*
 * The OTtypes.hxx header file contains all type declarations used by
 * the Open TURNS platform.
 */
#include "OTtypes.hxx"

/*
 * The OTmemory.hxx header file declares some debugging memory functions
 */
#include "OTmemory.hxx"

/*
 * Assertions are needed to ensure invariant respect
 */
#include <cassert>

/*
 * Some templates to ease C memory allocation
 */
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

template <class T>
T * newC(size_t count = 1)
{
  T * ptr = static_cast<T *>( calloc( count, sizeof(T) ) );
  return ptr;
}

template <class T>
void deleteC(const T * ptr)
{
  free( const_cast<T *>( ptr ) );
}

#endif /* OPENTURNS_OTPRIVATE_HXX */
