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
    sigma_u = sigma * u
    somme = complex(-0.5 * ot.dot(u, sigma_u), ot.dot(u, mu))
    return cmath.exp(somme)

if __name__ == "__main__":
    # Validation on distributions
    x = range(101)
    dx = 0.01
    y = range(101)
    dy = 0.01
    for ix in x:
        for iy in y:
            dist = ot.Normal([0.3, 0.9], ot.CovarianceMatrix([[2.0, -1.0], [-1.0, 3.0]]))
            cf = MGCF(dist, [dx * ix, dy * iy])
            print "x=%s y=%s val=%s" %(dx*ix, dy*iy, cf)
            try :
                value = dist.computeCharacteristicFunction([dx * ix, dy * iy])
                print "ot implementation: %s, diff=%s" %(value, value - cf)
            except ValueError:
                pass