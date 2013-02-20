//                                               -*- C -*-
/**
 * @file  WrapperCommon_static.h
 * @brief This file provides definitions for exported wrapper functions
 *
 * Copyright (C) 2005-2013 EDF
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author schueller
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#ifndef OPENTURNS_WRAPPERCOMMON_STATIC_H
#define OPENTURNS_WRAPPERCOMMON_STATIC_H

#include <WrapperInterface.h>
#include <WrapperCommon_macros.h>

#undef BIND_ACTION
#define BIND_ACTION(rtype, name, args) extern rtype (* name) args;
BIND_METHODS;

#endif /* OPENTURNS_WRAPPERCOMMON_STATIC_H */
