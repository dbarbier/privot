//                                               -*- C++ -*-
/**
 *  @file  Rfunctions.hxx
 *  @brief Graph implements graphic devices for plotting through R
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      Rfunctions.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_RFUNCTIONS_HXX
#define OPENTURNS_RFUNCTIONS_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS


static const String R_LEGEND = "legend <-function(x, y = NULL, legend, fill=NULL, col = par(\"col\"), lty, lwd, pch, angle = 45, density = NULL, bty = \"o\", bg = par(\"bg\"), pt.bg = NA, cex = 2, pt.cex = cex, pt.lwd = lwd, xjust = 0, yjust = 1, x.intersp = 1, y.intersp = 1, adj = c(0, 0.5), text.width = NULL, text.col = par(\"col\"), merge = do.lines && has.pch, trace = FALSE, plot = TRUE, ncol = 1, horiz = FALSE, title = NULL, inset = 0) \n { \n   if(missing(legend) && !missing(y) && (is.character(y) || is.expression(y))) \n   { \n     legend <- y \n     y <- NULL \n   } \n   mfill <- !missing(fill) || !missing(density) \n  \n   if(length(title) > 1) stop(\"invalid title\") \n   n.leg <- if(is.call(legend)) 1 else length(legend) \n   if(n.leg == 0) stop(\"legend is of length 0\") \n   auto <- \n     if (is.character(x)) \n       match.arg(x, c(\"bottomright\", \"bottom\", \"bottomleft\", \"left\", \"topleft\", \"top\", \"topright\", \"right\", \"center\")) \n     else NA \n   if (is.na(auto)) \n   { \n     xy <- xy.coords(x, y); x <- xy$x; y <- xy$y \n     nx <- length(x) \n     if (nx < 1 || nx > 2) stop(\"invalid coordinate lengths\") \n   } \n   else nx <- 0 \n  \n   xlog <- par(\"xlog\") \n   ylog <- par(\"ylog\") \n  \n   rect2 <- function(left, top, dx, dy, density = NULL, angle, ...) \n   { \n     r <- left + dx; if(xlog) { left <- 10^left; r <- 10^r } \n     b <- top  - dy; if(ylog) {  top <- 10^top;  b <- 10^b } \n     rect(left, top, r, b, angle = angle, density = density, ...) \n   } \n  \n   segments2 <- function(x1, y1, dx, dy, ...) \n   { \n     x2 <- x1 + dx; if(xlog) { x1 <- 10^x1; x2 <- 10^x2 } \n     y2 <- y1 + dy; if(ylog) { y1 <- 10^y1; y2 <- 10^y2 } \n     segments(x1, y1, x2, y2, ...) \n   } \n  \n   points2 <- function(x, y, ...) \n   { \n     if(xlog) x <- 10^x \n     if(ylog) y <- 10^y \n     points(x, y, ...) \n   } \n  \n   text2 <- function(x, y, ...) \n   { \n     if(xlog) x <- 10^x \n     if(ylog) y <- 10^y \n     text(x, y, ...) \n   } \n   if(trace) \n  \n     catn <- function(...) \n     { \n       do.call(\"cat\", c(lapply(list(...),formatC), list(\"\"))) \n     } \n  \n   cin <- par(\"cin\") \n   Cex <- cex * par(\"cex\") \n  \n   if(is.null(text.width)) \n     text.width <- max(strwidth(legend, units=\"user\", cex=cex)) \n   else if(!is.numeric(text.width) || text.width < 0) \n     stop(\"text.width must be numeric, >= 0\") \n  \n   xc <- Cex * xinch(cin[1], warn.log=FALSE) \n   yc <- Cex * yinch(cin[2], warn.log=FALSE) \n  \n   xchar  <- xc \n   xextra <- 0 \n   yextra <- yc * (y.intersp - 1) \n   ymax   <- max(yc, strheight(legend, units=\"user\", cex=cex)) \n   ychar <- yextra + ymax \n   if(trace) catn(\"  xchar=\", xchar, \"; (yextra,ychar)=\", c(yextra,ychar)) \n  \n   if(mfill) \n   { \n     xbox <- xc * 0.8 \n     ybox <- yc * 0.5 \n     dx.fill <- xbox \n   } \n   do.lines <- (!missing(lty) && (is.character(lty) || any(lty > 0))) || !missing(lwd) \n   if(is.na(do.lines)) do.lines <- FALSE \n  \n   n.legpercol <- \n     if(horiz) \n     { \n       if(ncol != 1) warning(\"horizontal specification overrides: Number of columns := \", n.leg) \n       ncol <- n.leg \n       1 \n     } else ceiling(n.leg / ncol) \n    \n   if(has.pch <- !missing(pch) && length(pch) > 0) \n   { \n     if(is.character(pch) && !is.na(pch[1]) && nchar(pch[1], type=\"c\") > 1) \n     { \n       if(length(pch) > 1) warning(\"not using pch[2..] since pch[1] has multiple chars\") \n       np <- nchar(pch[1], type=\"c\") \n       pch <- substr(rep.int(pch[1], np), 1:np, 1:np) \n     } \n     if(!merge) dx.pch <- x.intersp/2 * xchar \n   } \n   x.off <- if(merge) -0.7 else 0 \n    \n   if (is.na(auto)) \n   { \n     if (xlog) x <- log10(x) \n     if (ylog) y <- log10(y) \n   } \n   if(nx == 2) \n   { \n     x <- sort(x) \n     y <- sort(y) \n     left <- x[1] \n     top  <- y[2] \n     w <- diff(x) \n     h <- diff(y) \n     w0 <- w/ncol \n  \n     x <- mean(x) \n     y <- mean(y) \n     if(missing(xjust)) xjust <- 0.5 \n     if(missing(yjust)) yjust <- 0.5 \n   } \n   else \n   { \n     h <- (n.legpercol + !is.null(title)) * ychar + yc \n     w0 <- text.width + (x.intersp + 1) * xchar \n     if(mfill) w0 <- w0 + dx.fill \n     if(has.pch && !merge) w0 <- w0 + dx.pch \n     if(do.lines)  w0 <- w0 + (2+x.off) * xchar \n     w <- ncol*w0 + .5* xchar \n     if (!is.null(title) && (tw <- strwidth(title, units=\"user\", cex=cex) + 0.5*xchar) > w) \n     { \n       xextra <- (tw - w)/2 \n       w <- tw \n     } \n  \n     if (is.na(auto)) \n     { \n       left <- x - xjust * w \n       top  <- y + (1 - yjust) * h \n     } \n     else \n     { \n       usr <- par(\"usr\") \n       inset <- rep(inset, length.out = 2) \n       insetx <- inset[1]*(usr[2] - usr[1]) \n       left <- switch(auto, \"bottomright\"=, \"topright\"=, \"right\" = usr[2] - w - insetx, \"bottomleft\"=, \"left\"=, \"topleft\"= usr[1] + insetx, \"bottom\"=, \"top\"=, \"center\"= (usr[1] + usr[2] - w)/2) \n       insety <- inset[2]*(usr[4] - usr[3]) \n       top <- switch(auto, \"bottomright\"=, \"bottom\"=, \"bottomleft\"= usr[3] + h + insety, \"topleft\"=, \"top\"=, \"topright\" = usr[4] - insety, \"left\"=, \"right\"=, \"center\" = (usr[3] + usr[4] + h)/2) \n     } \n   } \n  \n   if (plot && bty != \"n\") \n   { \n     if(trace) catn(\"  rect2(\",left,\",\",top,\", w=\",w,\", h=\",h,\", ...)\",sep=\"\") \n     rect2(left, top, dx = w, dy = h, col = bg, density = NULL) \n   } \n  \n   xt <- left + xchar + xextra + (w0 * rep.int(0:(ncol-1), rep.int(n.legpercol,ncol)))[1:n.leg] \n   yt <- top - 0.5 * yextra - ymax - (rep.int(1:n.legpercol,ncol)[1:n.leg] - 1 + !is.null(title)) * ychar \n  \n   if(plot && (has.pch || do.lines || mfill)) col <- rep(col, length.out = n.leg) \n    \n   if (mfill) \n   { \n     if(plot) \n     { \n       fill <- rep(fill, length.out = n.leg) \n       for (boxcount in 1:length(xt)) \n       { \n         if (!is.na(fill[boxcount])) \n         { \n           dens <- NA \n           if(fill[boxcount] == \"solid\") dens <- NA \n           if(fill[boxcount] == \"shaded\") dens <- density \n           rect2(.9*xbox + xt[boxcount], yt[boxcount] + 1.2*ybox/2, dx = xbox, dy = 1.2*ybox/2, col = col[boxcount],density=dens,angle=angle,border=col[boxcount]) \n         } \n       } \n     } \n     xt <- xt + dx.fill \n   } \n   seg.len <- 2 \n   if(missing(lwd)) lwd <- par(\"lwd\") \n   if (do.lines) \n   { \n     if(missing(lty)) lty <- 1 \n     lty <- rep(lty, length.out = n.leg) \n     lwd <- rep(lwd, length.out = n.leg) \n     ok.l <- !is.na(lty) & (is.character(lty) | lty > 0) \n     if(trace) catn(\"  segments2(\",xt[ok.l] + x.off*xchar, \",\", yt[ok.l], \", dx=\", seg.len*xchar, \", dy=0, ...)\") \n     if(plot) segments2(xt[ok.l] + x.off*xchar, yt[ok.l], dx= seg.len*xchar, dy=0, lty = lty[ok.l], lwd = lwd[ok.l], col = col[ok.l]) \n     xt <- xt + (seg.len+x.off) * xchar \n   } \n   if (has.pch) \n   { \n     pch   <- rep(pch, length.out = n.leg) \n     pt.bg <- rep(pt.bg, length.out = n.leg) \n     pt.cex<- rep(pt.cex, length.out = n.leg) \n     pt.lwd<- rep(pt.lwd, length.out = n.leg) \n     ok <- !is.na(pch) & (is.character(pch) | pch >= 0) \n     x1 <- (if(merge) xt-(seg.len/2)*xchar else xt)[ok] \n     y1 <- yt[ok] \n     if(trace) catn(\"  points2(\", x1,\",\", y1,\", pch=\", pch[ok],\", ...)\") \n     if(plot) points2(x1, y1, pch = pch[ok], col = col[ok], cex = pt.cex[ok], bg = pt.bg[ok], lwd = pt.lwd[ok]) \n     if (!merge) xt <- xt + dx.pch \n   } \n  \n   xt <- xt + x.intersp * xchar \n   if(plot) \n   { \n     if (!is.null(title)) text2(left + w/2, top - ymax, labels = title, adj = c(0.5, 0), cex = cex, col = text.col) \n     text2(xt, yt, labels = legend, adj = adj, cex = cex, col = text.col) \n   } \n   invisible(list(rect = list(w = w, h = h, left = left, top = top), text = list(x = xt, y = yt))) \n }";

static const String R_PIE = "pie <- function (x,center=c(0,0), labels = names(x), edges = 200, radius = 0.8, clockwise = FALSE, init.angle = if (clockwise) 90 else 0, density = NULL, angle = 45, col = NULL, border = NULL, lty = NULL, main = NULL, ...) \n { \n   if (!is.numeric(x) || any(is.na(x) | x < 0)) stop(\"'x' values must be positive.\") \n   if (is.null(labels)) labels <- as.character(1:length(x)) \n   x <- c(0, cumsum(x)/sum(x)) \n   dx <- diff(x) \n   nx <- length(dx) \n  \n   pin <- par(\"pin\") \n   usr <- par(\"usr\") \n    \n   rhoy <- (usr[4]-usr[3])/(usr[2]-usr[1])*pin[1]/pin[2] \n   rhox <- 1 \n   if(rhoy>1) \n   { \n     rhox <- 1/rhoy \n     rhoy <- 1 \n   } \n    \n   if (is.null(col)) \n     col <- if (is.null(density)) c(\"white\", \"lightblue\", \"mistyrose\", \"lightcyan\", \"lavender\", \"cornsilk\") \n            else par(\"fg\") \n     col <- rep(col, length.out = nx) \n     border <- rep(border, length.out = nx) \n     lty <- rep(lty, length.out = nx) \n     angle <- rep(angle, length.out = nx) \n     density <- rep(density, length.out = nx) \n     twopi <- if (clockwise) -2 * pi else 2 * pi\n     t2xy <- function(t) \n             { \n               t2p <- twopi * t + init.angle * pi/180 \n               list(x = radius * cos(t2p), y = radius * sin(t2p)) \n             } \n                                        \n   for (i in 1:nx) \n   { \n     n <- max(2, floor(edges * dx[i])) \n     P <- t2xy(seq(x[i], x[i + 1], length = n)) \n     P$y <- P$y*rhoy \n     P$x <- P$x*rhox \n     polygon(c(P$x, 0)+center[1], c(P$y, 0)+center[2], density = density[i], angle = angle[i],border = border[i], col = col[i], lty = lty[i]) \n     P <- t2xy(mean(x[i + 0:1])) \n     P$y <- P$y*rhoy \n     P$x <- P$x*rhox \n     if (!is.na(lab <- labels[i]) && lab != \"\") \n     { \n       lines(c(1,1.05)*P$x+center[1], c(1,1.05)*P$y+center[2]) \n       text(1.1 * P$x + center[1], 1.1 * P$y + center[2], lab, cex=2, xpd = TRUE, adj = ifelse(P$x <0, 1, 0), ...) \n     } \n   } \n   invisible(NULL) \n }";


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RFUNCTIONS_HXX */
