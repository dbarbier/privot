//                                               -*- C++ -*-
/**
 *  @file  Lapack.hxx
 *  @brief This file includes all of the Lapack functions used in the platform
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
 *  Id      Lapack.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LAPACK_HXX
#define OPENTURNS_LAPACK_HXX

#include "OTconfig.hxx"
#ifdef HAVE_FORTRANCINTERFACE
#  include "FortranCInterface.h"
#endif /* HAVE_FORTRANCINTERFACE */

BEGIN_C_DECLS

/** Function daxpy is to be used to compute the sum or the difference of two matrices
    y := alpha*x + y */
#define DAXPY_F77 F77_FUNC(daxpy,DAXPY)
void DAXPY_F77(int *n, double *alpha, double *x, int *incx, double *y, int *incy);

/** Function dscal is to be used to compute the product of a matrix by a numerical scalar
    x := alpha*x */
#define DSCAL_F77 F77_FUNC(dscal,DSCAL)
void DSCAL_F77(int *n, double *alpha, double *x, int *incx);

/** Function dgemv is to be used to compute the product of a matrix with a vector (numerical point)
    y := alpha*A(trans)*x + beta*y */
#define DGEMV_F77 F77_FUNC(dgemv,DGEMV)
void DGEMV_F77(char *trans, int *m, int *n, double *alpha, double *a, int *lda, double *x, int *incx, double *beta, double *y, int *incy, int *ltrans);

/** Function dsymv is to be used to compute the product of a matrix with a vector (numerical point);  optimization for symmetric matrices
    y := alpha*A(trans)*x + beta*y */
#define DSYMV_F77 F77_FUNC(dsymv,DSYMV)
void DSYMV_F77(char *uplo, int *n, double *alpha, double *a, int *lda, double *x, int *incx, double *beta, double *y, int *incy, int *luplo);

/** Function dspmv is to be used to compute the product of a matrix with a vector (numerical point);  optimization for positive definite matrices
    y := alpha*A*x + beta*y */
#define DSPMV_F77 F77_FUNC(dspmv,DSPMV)
void DSPMV_F77(char *uplo, int *n, double *alpha, double *a, double *x, int *incx, double *beta, double *y, int *incy, int *luplo);

/** Function dgemm is to be used to compute the product of two matrices
    c := alpha*a(trans)*b(trans) + beta*c */
#define DGEMM_F77 F77_FUNC(dgemm,DGEMM)
void DGEMM_F77(char *transa, char *transb, int *m, int *n, int *k, double *alpha, double *a, int *lda, double *b, int *ldb, double *beta, double *c, int *ldc, int *ltransa, int *ltransb);

/** Function dsymm is to be used to compute the product of two matrices;  optimization for symmetric matrices
    c := alpha*a*b + beta*c if side is l or alpha*b*a + beta*c if side is r */
#define DSYMM_F77 F77_FUNC(dsymm,DSYMM)
void DSYMM_F77(char *side, char *uplo, int *m, int *n, double *alpha, double *a, int *lda, double *b, int *ldb, double *beta, double *c, int *ldc, int *lside, int *luplo);

/** Function dgeev is used to compute the eigenvalues of a square matrix */
#define DGEEV_F77 F77_FUNC(dgeev,DGEEV)
void DGEEV_F77(char *jobvl, char *jobvr, int *n, double *a, int *lda, double *wr, double *wi,
               double *vl, int *ldvl, double *vr, int *ldvr, double *work, int *lwork, int *info, int *ljobvl, int *ljobvr);

/** Function dsyev is used to compute the eigenvalues of a symmetric matrix */
#define DSYEV_F77 F77_FUNC(dsyev,DSYEV)
void DSYEV_F77(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info, int *ljobz, int *luplo);

/** Function dgelsy is used to solve the linear system corresponding to a matrix */
#define DGELSY_F77 F77_FUNC(dgelsy,DGELSY)
void DGELSY_F77(int *m, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb, int *jpvt, double *rcond, int *rank, double *work, int *lwork, int *info);

/** Function dgesv is used to solve the linear system corresponding to a square matrix */
#define DGESV_F77 F77_FUNC(dgesv,DGESV)
void DGESV_F77(int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb, int *info);

/** Function dsysv is used to solve the linear system corresponding to a symmetric matrix */
#define DSYSV_F77 F77_FUNC(dsysv,DSYSV)
void DSYSV_F77(char *uplo, int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb, double *work, int *lwork, int *info, int *luplo);

/** Function dpotrf is used to check if a matrix is positive definite */
#define DPOTRF_F77 F77_FUNC(dpotrf,DPOTRF)
void DPOTRF_F77(char *uplo, int *n, double *a, int *lda, int *info, int *luplo);

/** Function dgetrf is used to make the LU factorisation of a matrix */
#define DGETRF_F77 F77_FUNC(dgetrf,DGETRF)
void DGETRF_F77(int *m, int *n, double *a, int *lda, int *ipiv, int *info);

/** Function dsytrf is used to make the LU factorisation of a symmetric matrix */
#define DSYTRF_F77 F77_FUNC(dsytrf,DSYTRF)
void DSYTRF_F77(char *uplo, int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info, int *luplo);

/** Function dger is used to make the Kronecker product of two vectors */
#define DGER_F77 F77_FUNC(dger,DGER)
void DGER_F77(int *m, int *n, double *alpha, double *x, int *incx, double *y, int *incy, double *a, int *lda);

/** Function dstev computes the eigenvalues and eigenvectors of a symmetric tridiagonal matrix */
#define DSTEV_F77 F77_FUNC(dstev,DSTEV)
void DSTEV_F77(char *jobz, int *n, double *d, double *e, double *z, int *ldz, double *work, int *info, int *ljobz);

/** Function dsyr performs a rank one update on a square matrix A -> A + alpha * x . x' */
#define DSYR_F77 F77_FUNC(dsyr,DSYR)
void DSYR_F77(char *uplo, int *n, double *alpha, double *x, int *incx, double *a, int *lda, int *luplo);

/** Function dtrsm solves a triangular linear system A.X = alpha B, where A can be transposed or not */
#define DTRSM_F77 F77_FUNC(dtrsm,DTRSM)
void DTRSM_F77(char *side, char *uplo, char *transa, char *diag, int *m, int *n, double *alpha, double *A, int *lda, double *B, int *ldb, int *lside, int *luplo, int *ltransa, int *ldiag);

/** Function dgesdd computes singular values by using a divide and conquer strategy.
    The number of singular values is the min of the column dimension and the row dimension. */
#define DGESDD_F77 F77_FUNC(dgesdd,DGESDD)
void DGESDD_F77(char *jobz, int *m, int *n, double *A, int *lda, double *S, double *U, int *ldu, double *VT, int *ldvt, double *work, int *lwork, int *iwork, int *info, int *ljobz);

/** Function dtrmv is to be used to compute the product of a triangular
    (upper or lower) matrix with a vector (numerical point) */
#define DTRMV_F77 F77_FUNC(dtrmv,DTRMV)
void DTRMV_F77(char *uplo, char *trans, char *diag, int *n, double *A, int *lda, double *X, int *incx, int *luplo, int *ltrans, int *ldiag);

/** BLAS routines for complex values */

/** Function zaxpy is to be used to compute the sum or the difference of two complex matrices
    y := alpha*x + y */
#define ZAXPY_F77 F77_FUNC(zaxpy,ZAXPY)
void ZAXPY_F77(int *n, std::complex<double> *alpha, std::complex<double> *x, int *incx, std::complex<double> *y, int *incy);

/** Function zscal is to be used to compute the product of a complex matrix by a numerical complex
    x := alpha*x */
#define ZSCAL_F77 F77_FUNC(zscal,ZSCAL)
void ZSCAL_F77(int *n, std::complex<double> *alpha, std::complex<double> *x, int *incx);

/** Function zgemm is to be used to compute the product of two complex matrices
    c := alpha*a(trans)*b(trans) + beta*c */
#define ZGEMM_F77 F77_FUNC(zgemm,ZGEMM)
void ZGEMM_F77(char *transa, char *transb, int *m, int *n, int *k, std::complex<double> *alpha, std::complex<double> *a, int *lda,
               std::complex<double> *b, int *ldb, std::complex<double> *beta, std::complex<double> *c, int *ldc, int *ltransa, int *ltransb);

/** Function zsymm is to be used to compute the product of two complex matrices
 *  optimization for symmetric matrices
 c := alpha*a*b + beta*c if side is l or alpha*b*a + beta*c if side is r */
#define ZSYMM_F77 F77_FUNC(zsymm,ZSYMM)
void ZSYMM_F77(char *side, char *uplo, int *m, int *n, std::complex<double> *alpha, std::complex<double> *a, int *lda,
               std::complex<double> *b, int *ldb, std::complex<double> *beta, std::complex<double> *c, int *ldc, int *lside, int *luplo);

/** Function zhemm is to be used to compute the product of two complex matrices
 *  optimization for hermitian matrices
 c := alpha*a*b + beta*c if side is l or alpha*b*a + beta*c if side is r */
#define ZHEMM_F77 F77_FUNC(zhemm,ZHEMM)
void ZHEMM_F77(char *side, char *uplo, int *m, int *n, std::complex<double> *alpha, std::complex<double> *a, int *lda,
               std::complex<double> *b, int *ldb, std::complex<double> *beta, std::complex<double> *c, int *ldc, int *lside, int *luplo);

/** Function zpotrf is used to computes the Cholesky factorization of a complex hermitian positive definite matrix  */
#define ZPOTRF_F77 F77_FUNC(zpotrf,ZPOTRF)
void ZPOTRF_F77(char *uplo, int *n, std::complex<double> *a, int *lda, int *info, int *luplo);

/** Function zgemv is to be used to compute the product of a complex matrix with a complex vector
    y := alpha*A(trans)*x + beta*y */
#define ZGEMV_F77 F77_FUNC(zgemv,ZGEMV)
void ZGEMV_F77(char *trans, int *m, int *n, std::complex<double> *alpha, std::complex<double> *a, int *lda, std::complex<double> *x, int *incx,
               std::complex<double> *beta, std::complex<double> *y, int *incy, int *ltrans);

/** Function zhemv is to be used to compute the product of a complex matrix with a vector ;  optimization for hermitian matrices
    y := alpha*A(trans)*x + beta*y */
#define ZHEMV_F77 F77_FUNC(zhemv,ZHEMV)
void ZHEMV_F77(char *uplo, int *n, std::complex<double> *alpha, std::complex<double> *a, int *lda, std::complex<double> *x, int *incx,
               std::complex<double> *beta, std::complex<double> *y, int *incy, int *luplo);

/** Function zcopy is to be used to copy a vector x to a vector y */
#define ZCOPY_F77 F77_FUNC(zcopy,ZCOPY)
void ZCOPY_F77(int *n, std::complex<double> *x, int *incx, std::complex<double> *y, int *incy);

/** Function ztrmv is to be used to compute the product of a triangular (upper or lower) matrix with a vector  */
#define ZTRMV_F77 F77_FUNC(ztrmv,ZTRMV)
void ZTRMV_F77(char *uplo, char *trans, char *diag, int *n, std::complex<double> *A, int *lda, std::complex<double> *X, int *incx, int *luplo, int *ltrans, int *ldiag);

/** Function ztrmm is to be used to compute the product of a triangular (upper or lower) matrix with another matrix  */
#define ZTRMM_F77 F77_FUNC(ztrmm,ZTRMM)
void ZTRMM_F77(char *side, char *uplo, char *trans, char *diag, int *m, int *n, std::complex<double> *alpha , std::complex<double> *A, int *lda, std::complex<double> *B, int *ldb,
               int *lside , int *luplo, int *ltrans, int *ldiag);

/** Function dtrmv is to be used to compute the product of a triangular (upper or lower) matrix with a vector  */
void DTRMV_F77(char *uplo, char *trans, char *diag, int *n, double *A, int *lda, double *X, int *incx, int *luplo, int *ltrans, int *ldiag);

/** Function dposv is to be used to solve a linear system with a symmetric definite positive matrix */
#define DPOSV_F77 F77_FUNC(dposv,DPOSV)
void DPOSV_F77(char *uplo, int *n, int *nrhs, double *A, int *lda, double *B, int *ldb, int *info, int *luplo);

END_C_DECLS

#endif /* OPENTURNS_LAPACK_HXX */
