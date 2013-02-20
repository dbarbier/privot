double dlamc3_(double *a, double *b)
{
  /*  -- LAPACK auxiliary routine (version 3.0) --
      Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
      Courant Institute, Argonne National Lab, and Rice University
      October 31, 1992


      Purpose
      =======

      DLAMC3  is intended to force  A  and  B  to be stored prior to doing

      the addition of  A  and  B ,  for use in situations where optimizers

      might hold one of these in a register.

      Arguments
      =========

      A, B    (input) DOUBLE PRECISION
      The values A and B.

      =====================================================================
  */
  /* >>Start of File<<
     System generated locals */
  volatile double ret_val;



  ret_val = *a + *b;

  return ret_val;

  /*     End of DLAMC3 */

} /* dlamc3_ */
