//                                               -*- C++ -*-
/**
 *  @file  TestOptimLinear.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <iostream>
#include <cstdlib>
#include "OTconfig.hxx" // Only needed for test wrappers
#include "WrapperInterface.h"
#include "WrapperCommon.h"
#include "WrapperMacros.h"

namespace WrapperInternals
{
  struct internalState
  {
    long numberOfCalls;
  };

  void internalStateInitialization(struct internalState * p_internalState)
  {
    if (p_internalState) p_internalState->numberOfCalls = 0;
  }

  void internalStateIncrement(struct internalState * p_internalState)
  {
    if (p_internalState) p_internalState->numberOfCalls++;
  }

  long internalStateGetNumberOfCalls(struct internalState * p_internalState)
  {
    return (p_internalState ? p_internalState->numberOfCalls : -1);
  }
} /* namespace WrapperInternals */

#define WRAPPERNAME optim_linear

BEGIN_C_DECLS

WRAPPER_BEGIN

static struct WrapperInformation info_optim_linear = {/* inSize_  = */ 4,
                                                      /* outSize_ = */ 1
};


/* Function */
FUNC_CREATESTATE( WRAPPERNAME ,
                  {
                    *p_p_state = malloc(sizeof(struct WrapperInternals::internalState));
                    internalStateInitialization(static_cast<struct WrapperInternals::internalState *>(*p_p_state));
                  } )

FUNC_DELETESTATE( WRAPPERNAME , { free(p_state); } )

FUNC_INFO( WRAPPERNAME , { *p_info = info_optim_linear; } )

FUNC_INIT( WRAPPERNAME , {} )

FUNC_EXEC( WRAPPERNAME ,
           {
             internalStateIncrement(static_cast<struct WrapperInternals::internalState *>(p_state));
             double x1(inPoint->data_[0]);
             double x2(inPoint->data_[1]);
             double x3(inPoint->data_[2]);
             double x4(inPoint->data_[3]);

             outPoint->data_[0] = x1 + 2 * x2 - 3 * x3 + 4 * x4;
           } )

FUNC_FINALIZE( WRAPPERNAME , {} )


/* Gradient, which is void at this time */
GRAD_CREATESTATE( WRAPPERNAME ,
                  {
                    *p_p_state = malloc(sizeof(struct WrapperInternals::internalState));
                    internalStateInitialization(static_cast<struct WrapperInternals::internalState *>(*p_p_state));
                  } )

GRAD_DELETESTATE( WRAPPERNAME , { free(p_state); } )

GRAD_INFO( WRAPPERNAME , { *p_info = info_optim_linear; } )

GRAD_INIT( WRAPPERNAME , {} )

GRAD_EXEC( WRAPPERNAME , {} )

GRAD_FINALIZE( WRAPPERNAME , {} )

/* Hessian, which is void at this time */
HESS_CREATESTATE( WRAPPERNAME ,
                  {
                    *p_p_state = malloc(sizeof(struct WrapperInternals::internalState));
                    internalStateInitialization(static_cast<struct WrapperInternals::internalState *>(*p_p_state));
                  } )

HESS_DELETESTATE( WRAPPERNAME , { free(p_state); } )

HESS_INFO( WRAPPERNAME , { *p_info = info_optim_linear; } )

HESS_INIT( WRAPPERNAME , {} )

HESS_EXEC( WRAPPERNAME , {} )

HESS_FINALIZE( WRAPPERNAME , {} )

WRAPPER_END

END_C_DECLS
