import cmath
import openturns as ot

def MGCF(distribution, u):
    """
    Multivariate gaussian characteristic function

    Parameters
    ----------
    distribution : Distribution
                   An OpenTURNS Distribution

    matrix : Matrix
             Either OpenTURNS matrix or Numpy matrix
    u : 1D array-like
        Either a python list, an OpenTURNS NumericalPoint or a Numpy 1D-array
        Its length should be equal to the dimension of the distribution   

    Returns
    ----------
    out : a complex
          The characteristic function of dist evaluated on the u point

    Example
    -------
    >>> import openturns as ot
    >>> cf = MGCF(ot.Normal(2), [4,4])

    """
    dist = ot.Distribution(distribution)
    if dist.getImplementation().getClassName() != "Normal" :
        raise ValueError("Only gaussian distributions are allowed")
    if (len(u) != dist.getDimension()):
        raise ValueError ("Dist of dimension %d whereas the input vector is of dimension %d" %(dist.getDimension(), len(u)))
    # Computation
    somme = 0
    mu = dist.getMean()
    sigma = dist.getCovariance()
    simga_u = sigma * u
    for k in xrange(dist.getDimension()):
        somme += (u[k] * mu[k]* 1j) - 0.5 * u[k] * simga_u[k]
    return cmath.exp(somme)

if __name__ == "__main__":
    # Validation on distributions
    x = range(101)
    dx = 0.01
    y = range(101)
    dy = 0.01
    for ix in x:
        for iy in y:
	    cf = MGCF(ot.Normal(2), [dx * ix, dy * iy])
	    print "x=%s y=%s val=%s" %(dx*ix, dy*iy, cf)