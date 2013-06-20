import multiprocessing
import time
import numpy as np
import itertools
import logging

def compute_delta_characteristic_function_on_3d_grid(distribution, b, N, process_local_id):
    """
    The interest is to compute one of the following quantities:

    if process_local_id = 0
    \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
    if process_local_id = 1
    \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
    \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
    if process_local_id = 2
    \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
    \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
    if process_local_id = 3
    \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    if process_local_id = 4
    \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
    \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
    if process_local_id = 5
    \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    if process_local_id = 6
    \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    if process_local_id = 7
    \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
    \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
    if process_local_id = 8
    \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
    \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
    if process_local_id = 9
    \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
    \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
    if process_local_id = 10
    \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
    \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
    if process_local_id = 11
    \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0,(k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
    \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0,-(k2+1)hy,0) E_{m1,m2,m3}(0,-(k2+1),0)
    if process_local_id = 12
    \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0,0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
    \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0,0,-(k3+1)hz) E_{m1,m2,m3}(0,0,-(k3+1)

    When fixing a process_local_id, the two computations are done and their sum is returned

    The final purpose of this "private" function is to be called seuqentially or concurrenctly and compute :
    S_{m1,m2,m3} = \frac{hx hy hz}{8\pi^3}\sum_{k1=-N}^{N}\sum_{k2=-N}^{N}\sum_{k3=-N}^{N}\delta(k1 h_x,k2 h_y k3 h_z) E_{m1,m2,m3}(k1,k2,k3)
    Here :
      E_{m1,m2,m3}(k1,k2,k3)=e^{-i\sum_{j=1}^{3} k_jh_j (\mu_j+a (\frac{2m_j+1}{M}-1)\sigma_j)}
    Thus,
    S_{m1,m2,m3}=\frac{hx hy hz}{4\pi^2} \sum_{s1,s2,s3 \in [0,-,+]} \Sigma_{m1,m2,m3}^{s1 s2 s3}

    Parameters
    ----------
    distribution : PythonMultivariateRandomMixture
        MultivariateRandomMixture distribution for which the pdf is evaluated in links with its characteristic function

    b : positive float
        The number of marginal standard deviations beyond which the density is evaluated

    N : positive integer, preference of form N = 2^k
        The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

    process_local_id : positive integer
        Fix which part of calculations is wondered

    Returns
    -------

    s : complex-ndarray of shape (N,N,N)
        The sigma values fixed by the user

    Example
    -------
    >>> import openturns as ot
    >>> import MultivariateRandomMixture as MV
    >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0), ot.Uniform(2.0, 5.0)])
    >>> matrix = ot.Matrix([[1,2, 4], [3,4,5], [6,0,1]])
    >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix)
    >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
    >>> N = 128 # 128x128x128 points for the 3D grid
    >>> [grid_values, pdf_values] = dist.computePDFOn3DGrid(b, N)
    >>> xgrid_values, ygrid_values, zgrid_values = tuple(grid_values)
    >>> f = open("out.csv", "w")
    >>> f.write("x;y;z;pdf\n")
    >>> for i in xrange(len(xgrid_values)):
    ...     for j in xrange(len(ygrid_values)):
    ...         for k in xrange(len(zgrid_values)):
    ...             f.write("{0:.16g};{1:.16g};{2:.16g};{3:.16g}\n".format(xgrid_values[i], ygrid_values[j], zgrid_values[k], pdf_values[i][j][k]))
    ... f.close()

    """

    assert (float(b) > 0)
    pi = np.pi
    two_pi = 2.0 * pi
    mu_x, mu_y, mu_z = tuple(distribution.getMean())
    sigma_x, sigma_y, sigma_z = tuple(distribution.getStandardDeviation())
    b_sigma_x, b_sigma_y , b_sigma_z = sigma_x * b, sigma_y * b, sigma_z * b
    tau_x, tau_y, tau_z = mu_x / b_sigma_x, mu_y / b_sigma_y, mu_z / b_sigma_z
    h_x, h_y, h_z = pi / b_sigma_x, pi / b_sigma_y, pi / b_sigma_z

    #  These arrays are used below
    f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(N,1,1)
    f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,N,1)
    f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,1,N)
    z_exp_m3 = np.exp(-two_pi* 1j * np.arange(N) / N)
    z_exp_m1 = z_exp_m3.reshape(N,1,1)
    z_exp_m2 = z_exp_m3.reshape(1,N,1)

    if process_local_id == 0:
        logging.debug("Process 0: compute \Sigma_+++ and compute \Sigma_---")
        # 1) compute \Sigma_+++
        # \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{+++} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k+1)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * f2 * f3
        yk_hat = np.fft.fftn(yk)
        sigma_plus_plus_plus = yk_hat * z_exp_m1 * z_exp_m2 * z_exp_m3

        # 2) compute \Sigma_---
        # \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{---} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        sigma_minus_minus_minus = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        logging.debug("End of Process 0")
        return sigma_plus_plus_plus + sigma_minus_minus_minus

    if process_local_id == 1:
        logging.debug("Process 1: compute \Sigma_++- and compute \Sigma_--+")
        # 3) compute \Sigma_++-
        # \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
        # \Sigma_{m1,m2,m3}^{++-} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * f2 * np.conjugate(f3[:,:,::-1])
        yk_hat = np.fft.fftn(yk)
        sigma_plus_plus_minus = yk_hat * z_exp_m1 * z_exp_m2

        # 4) compute \Sigma_--+
        # \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
        # \Sigma_{m1,m2,m3}^{--+} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3+1) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3/N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_minus_plus = yk_hat * z_exp_m3
        logging.debug("End of Process 1")
        return sigma_plus_plus_minus + sigma_minus_minus_plus

    if process_local_id == 2:
        logging.debug("Process 2: compute \Sigma_+-+ and compute \Sigma_-+-")
        # 5) compute \Sigma_+-+
        # \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
        # \Sigma_{m1,m2,m3}^{+-+} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, (k+1)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * f3
        yk_hat = np.fft.fftn(yk)
        sigma_plus_minus_plus = yk_hat * z_exp_m1 * z_exp_m3

        # 6) compute \Sigma_-+-
        # \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{-+-} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[(k1-N) hx, (k2+1) hy, (k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2/N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_plus_minus = yk_hat * z_exp_m2
        logging.debug("End of Process 2")
        return sigma_plus_minus_plus + sigma_minus_plus_minus

    if process_local_id == 3:
        logging.debug("Process 3: compute \Sigma_+-- and compute \Sigma_-++")
        # 7) compute \Sigma_+--
        # \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{+--} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * np.conjugate(f3[:,:,::-1])
        yk_hat = np.fft.fftn(yk)
        sigma_plus_minus_minus = yk_hat * z_exp_m1

        # 8) compute \Sigma_-++
        # \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{-++} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_plus_plus = yk_hat * z_exp_m2 * z_exp_m3
        logging.debug("End of Process 3")
        return sigma_plus_minus_minus + sigma_minus_plus_plus

    #----------------------------------------#
    #----------------- FFT2D ----------------#
    #--- In this section, we apply 2D FFT ---#
    #--- and we propagate in 3 dimensions ---#
    #----------------------------------------#
    f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1))
    f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1))
    f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1))
    z_exp_m2 = np.exp(-two_pi* 1j * np.arange(N) / N)
    z_exp_m1 = z_exp_m2.reshape(N,1)

    if process_local_id == 4:
        logging.debug("Process 4: compute \Sigma_++0 and compute \Sigma_--0")
        # 9) compute \Sigma_++0
        # \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
        # \Sigma_{m1,m2,m3}^{++0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx, (k2+1) hy, 0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1) * h_x, (j+1) * h_y, 0]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * f2
        yk_hat = np.fft.fft2(yk)
        sigma_plus_plus_0 = yk_hat * z_exp_m1 * z_exp_m2

        # 10) compute \Sigma_--0
        # \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
        # \Sigma_{m1,m2,m3}^{--0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[-(k1-N) hx, -(k2-N) hy, 0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_minus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
        logging.debug("End of Process 4")
        return sigma_plus_plus_0 + sigma_minus_minus_0

    if process_local_id == 5:
        logging.debug("Process 5: compute \Sigma_0++ and compute \Sigma_0--")
        # 11) compute \Sigma_0++
        # \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{0++} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,(k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j+1)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f2.reshape(N,1) * f3
        yk_hat = np.fft.fft2(yk)
        sigma_0_plus_plus = yk_hat * z_exp_m1 * z_exp_m2

        # 12) compute \Sigma_0--
        # \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{0-} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,-(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_0_minus_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
        logging.debug("End of Process 5")
        return sigma_0_plus_plus + sigma_0_minus_minus

    if process_local_id == 6:
        logging.debug("Process 6: compute \Sigma_+0+ and compute \Sigma_-0-")
        # 13) compute \Sigma_+0+
        # \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        # \Sigma_{m1,m2,m3}^{+0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i + 1) * h_x, 0, (j + 1) * h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * f3
        yk_hat = np.fft.fft2(yk)
        sigma_plus_0_plus = yk_hat * z_exp_m1 * z_exp_m2

        # 14) compute \Sigma_-0-
        # \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        # \Sigma_{m1,m2,m3}^{-0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[-(k1-N) hx,0,-(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_0_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
        logging.debug("End of Process 6")
        return sigma_plus_0_plus + sigma_minus_0_minus

    if process_local_id == 7:
        logging.debug("Process 7: compute \Sigma_+-0 and compute \Sigma_-+0")
        # 15) compute \Sigma_+-0
        # \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
        # \Sigma_{m1,m2,m3}^{+-0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k2-N) hy,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, 0]) for j in xrange(N)] for i in xrange(N)])
        yk = dcf * f1.reshape(N,1) * np.conjugate(f2[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_plus_minus_0 = yk_hat * z_exp_m1

        # 16) compute \Sigma_-+0
        # \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
        # \Sigma_{m1,m2,m3}^{-+0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[(k1-N) hx,0,(k2+1) hy,0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_plus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
        logging.debug("End of Process 7")
        return sigma_plus_minus_0 + sigma_minus_plus_0

    if process_local_id == 8:
        logging.debug("Process 8: compute \Sigma_+0- and compute \Sigma_-0+")
        # 17) compute \Sigma_+0-
        # \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
        # \Sigma_{m1,m2,m3}^{+0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, 0, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * np.conjugate(f3[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_plus_0_minus = yk_hat * z_exp_m1

        # 18) compute \Sigma_-0+
        # \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
        # \Sigma_{m1,m2,m3}^{-0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m3/N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_0_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
        logging.debug("End of Process 8")
        return sigma_plus_0_minus + sigma_minus_0_plus

    if process_local_id == 9:
        logging.debug("Process 9: compute \Sigma_0+- and compute \Sigma_0-+")
        # 19) compute \Sigma_0+-
        # \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
        # \Sigma_{m1,m2,m3}^{0+-} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[distribution.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f2.reshape(N,1) * np.conjugate(f3[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_0_plus_minus = yk_hat * z_exp_m1

        # 20) compute \Sigma_0-+
        # \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
        # \Sigma_{m1,m2,m3}^{0-+} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_0_minus_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
        logging.debug("End of Process 9")
        return sigma_0_plus_minus + sigma_0_minus_plus

    #----------------------------------------#
    #----------------- FFT1D ----------------#
    #--- In this section, we apply 1D FFT ---#
    #--- and we propagate in 3 dimensions ---#
    #----------------------------------------#
    z_exp_m1 = np.exp(-two_pi* 1j * np.arange(N) / N)

    if process_local_id == 10:
        logging.debug("Process 10: compute \Sigma_+00 and compute \Sigma_-00")
        # 21) compute \Sigma_+00
        # \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
        # \Sigma_{m1,m2,m3}^{+00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N)
        # forall k1,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([distribution.computeDeltaCharacteristicFunction([(i+1)*h_x,0,0]) for i in xrange(N)])
        yk = dcf * f1
        yk_hat = np.fft.fft(yk)
        sigma_plus_0_0 = yk_hat * z_exp_m1

        # 22) compute \Sigma_-00
        # \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
        # \Sigma_{m1,m2,m3}^{-00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1)
        # forall k1,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_minus_0_0 = np.fft.fft(np.conjugate(yk[::-1]))
        logging.debug("End of Process 10")
        return sigma_plus_0_0 + sigma_minus_0_0

    if process_local_id == 11:
        logging.debug("Process 11: compute \Sigma_0+0 and compute \Sigma_0-0")
        # 23) compute \Sigma_0+0
        # \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
        # \Sigma_{m1,m2,m3}^{0+0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,(k2+1)hy,0] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N)
        # forall k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([distribution.computeDeltaCharacteristicFunction([0,(i+1)*h_y,0]) for i in xrange(N)])
        yk = dcf * f2
        yk_hat = np.fft.fft(yk)
        sigma_0_plus_0 = yk_hat * z_exp_m1

        # 24) compute \Sigma_0-0
        # \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
        # \Sigma_{m1,m2,m3}^{0-0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,-(k2-N)hy,0]) * exp(- pi* 1j * (k2+1) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2)
        # forall k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_0_minus_0 = np.fft.fft(np.conjugate(yk[::-1]))
        logging.debug("End of Process 11")
        return sigma_0_plus_0 + sigma_0_minus_0

    if process_local_id == 12:
        logging.debug("Process 12: compute \Sigma_00+ and compute \Sigma_00-")
        # 25) compute \Sigma_00+
        # \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
        # \Sigma_{m1,m2,m3}^{00+} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,0,(k3+1)hz] * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m3 / N)
        # forall k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([distribution.computeDeltaCharacteristicFunction([0,0,(i+1)*h_z]) for i in xrange(N)])
        yk = dcf * f3
        yk_hat = np.fft.fft(yk)
        sigma_0_0_plus = yk_hat * z_exp_m1

        # 26) compute \Sigma_00-
        # \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
        # \Sigma_{m1,m2,m3}^{00-} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,0,-(k3-N)hz]) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m3)
        # forall k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_0_0_minus = np.fft.fft(np.conjugate(yk[::-1]))
        logging.debug("End of Process 12")
        return sigma_0_0_plus + sigma_0_0_minus


def factorizer_compute_delta_function_on_3d_grid(distribution, b, N, nprocs = multiprocessing.cpu_count()):
    """
    Compute PDF on 3 Grid function helper
    This function enables to compute the Delta characteristic parts in parallel
    """
    #nums = [0,12,1,11,2,10,3,9,4,8,5,7,6]
    # Better order for launching processes
    nums = [0,4,1,5,2,6,3,7,8,10,9,11,12]
    def worker(nums,out_q):
        """ The worker function, invoked in a process. 'nums' is a
            list of numbers to factor. The results are placed in
            a dictionary that's pushed to a queue.
        """
        outdict = {}
        for n in nums:
            outdict[n] = compute_delta_characteristic_function_on_3d_grid(distribution, b, N, n)
        out_q.put(outdict)

    # Each process will get 'chunksize' nums and a queue to put his out
    # dict into
    out_q = multiprocessing.Queue()
    chunksize = int(np.ceil(len(nums) / float(nprocs)))
    procs = []

    for i in range(nprocs):
        p = multiprocessing.Process(
                target=worker,
                args=(nums[chunksize * i:chunksize * (i + 1)],
                      out_q))
        procs.append(p)
        p.start()

    # Collect all results into a single result dict. We know how many dicts
    # with results to expect.
    resultdict = {}
    for i in range(nprocs):
        resultdict.update(out_q.get())

    # Wait for all worker processes to finish
    for p in procs:
        p.join()

    pi = np.pi
    two_pi = 2.0 * pi
    mu_x, mu_y, mu_z = tuple(distribution.getMean())
    sigma_x, sigma_y, sigma_z = tuple(distribution.getStandardDeviation())
    b_sigma_x, b_sigma_y , b_sigma_z = sigma_x * b, sigma_y * b, sigma_z * b
    tau_x, tau_y, tau_z = mu_x / b_sigma_x, mu_y / b_sigma_y, mu_z / b_sigma_z
    h_x, h_y, h_z = pi / b_sigma_x, pi / b_sigma_y, pi / b_sigma_z
    # final s_m
    s_m = resultdict[0] + resultdict[1] + resultdict[2] + resultdict[3] \
         + resultdict[4].reshape(N,N,1)  + resultdict[5].reshape(1,N,N) \
         + resultdict[6].reshape(N,1,N)  + resultdict[7].reshape(N,N,1) \
         + resultdict[8].reshape(N,1,N)  + resultdict[9].reshape(1,N,N) \
         + resultdict[10].reshape(N,1,1) + resultdict[11].reshape(1,N,1) \
         + resultdict[12].reshape(1,1,N)
    s_m *= (h_x * h_y * h_z) /(8*pi*pi*pi)
    return s_m

def parallel_compute_delta_function_on_3d_grid(distribution, b, N, nprocs = multiprocessing.cpu_count()):
    """
    This function is used for 3D computations
    It helps to compute gaussian approximations on 3D regular grids
    The function uses parallel processes
    The interest is to compute the density function on a 3D grid of form:
      r \in {1,2,3},\forall m\in\{0,\hdots,N-1\},\:y_{r,m}=\mu_r+b(\frac{2m+1}{N} - 1)\sigma_r

    The density is given by:
      p_{m1,,m2, m3}= Q_{m1,m2,m3}+S_{m1,m2,m3}
    with S_{m1,m2,m3} = \frac{hx hy hz}{8\pi^3}\sum_{k1=-N}^{N}\sum_{k2=-N}^{N}\sum_{k3=-N}^{N}\delta(k1 h_x,k2 h_y k3 h_z) E_{m1,m2,m3}(k1,k2,k3)
    Here :
      E_{m1,m2,m3}(k1,k2,k3)=e^{-i\sum_{j=1}^{3} k_jh_j (\mu_j+a (\frac{2m_j+1}{M}-1)\sigma_j)}
    Using FFT,
    S_{m1,m2,m3}=\frac{hx hy hz}{4\pi^2} \sum_{s1,s2,s3 \in [0,-,+]} \Sigma_{m1,m2,m3}^{s1 s2 s3}

    with
    \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
    \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
    \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
    \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
    \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
    \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
    \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
    \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
    \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
    \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
    \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
    \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
    \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
    \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0,(k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
    \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0,-(k2+1)hy,0) E_{m1,m2,m3}(0,-(k2+1),0)
    \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0,0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
    \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0,0,-(k3+1)hz) E_{m1,m2,m3}(0,0,-(k3+1)

    Parameters
    ----------
    b : positive float
        The number of marginal standard deviations beyond which the density is evaluated

    N : positive integer, preference of form N = 2^k
        The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

    Returns
    -------
    grid_values : ndarray of shape (N, 3)
                  3D Grid on which the probability density function has been evaluated

    pdf_values : ndarray of shape (N,N,N)
                The probability density function values on the grid

    Example
    -------
    >>> import openturns as ot
    >>> import MultivariateRandomMixture as MV
    >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0), ot.Uniform(2.0, 5.0)])
    >>> matrix = ot.Matrix([[1,2, 4], [3,4,5], [6,0,1]])
    >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix)
    >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
    >>> N = 128 # 128x128x128 points for the 3D grid
    >>> [grid_values, pdf_values] = dist.computePDFOn3DGrid(b, N)
    >>> xgrid_values, ygrid_values, zgrid_values = tuple(grid_values)
    >>> f = open("out.csv", "w")
    >>> f.write("x;y;z;pdf\n")
    >>> for i in xrange(len(xgrid_values)):
    ...     for j in xrange(len(ygrid_values)):
    ...         for k in xrange(len(zgrid_values)):
    ...             f.write("{0:.16g};{1:.16g};{2:.16g};{3:.16g}\n".format(xgrid_values[i], ygrid_values[j], zgrid_values[k], pdf_values[i][j][k]))
    ... f.close()

    """
    global _compute_delta_characterstic_function_3d
    def _compute_delta_characterstic_function_3d(args):
        return distribution.computeDeltaCharacteristicFunction(args)
    pi = np.pi
    two_pi = 2.0 * pi
    mu_x, mu_y, mu_z = tuple(distribution.getMean())
    sigma_x, sigma_y, sigma_z = tuple(distribution.getStandardDeviation())
    b_sigma_x, b_sigma_y , b_sigma_z = sigma_x * b, sigma_y * b, sigma_z * b
    tau_x, tau_y, tau_z = mu_x / b_sigma_x, mu_y / b_sigma_y, mu_z / b_sigma_z
    h_x, h_y, h_z = pi / b_sigma_x, pi / b_sigma_y, pi / b_sigma_z

    #  These arrays are used below
    f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(N,1,1)
    f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,N,1)
    f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,1,N)
    z_exp_m3 = np.exp(-two_pi* 1j * np.arange(N) / N)
    z_exp_m1 = z_exp_m3.reshape(N,1,1)
    z_exp_m2 = z_exp_m3.reshape(1,N,1)

    logging.debug("Process: compute \Sigma_+++ and compute \Sigma_---")
    # compute dcf
    x = np.arange(1,N+1) * h_x
    y = np.arange(1,N+1) * h_y
    z = np.arange(1,N+1) * h_z
    grid = itertools.product(x,y,z)
    pool = multiprocessing.Pool(nprocs)
    #dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k+1)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
    dcf = pool.map(_compute_delta_characterstic_function_3d, grid, N*N)
    dcf = np.array(dcf).reshape(N,N,N)

    # 1) compute \Sigma_+++
    # \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
    # \Sigma_{m1,m2,m3}^{+++} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    yk = dcf * f1 * f2 * f3
    del dcf
    yk_hat = np.fft.fftn(yk)
    sigma_plus_plus_plus = yk_hat * z_exp_m1 * z_exp_m2 * z_exp_m3

    # 2) compute \Sigma_---
    # \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
    # \Sigma_{m1,m2,m3}^{---} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    sigma_minus_minus_minus = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
    logging.debug("End of Process")

    # 3) compute \Sigma_++-
    # \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
    # \Sigma_{m1,m2,m3}^{++-} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1

    # compute dcf
    logging.debug("Process: compute \Sigma_++- and compute \Sigma_--+")
    x = np.arange(1,N+1) * h_x
    y = np.arange(1,N+1) * h_y
    z = np.arange(-N,0,1) * h_z
    grid = itertools.product(x,y,z)
    #dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
    dcf = pool.map(_compute_delta_characterstic_function_3d, grid, N*N)
    dcf = np.array(dcf).reshape(N,N,N)

    yk = dcf * f1 * f2 * np.conjugate(f3[:,:,::-1])
    del dcf
    yk_hat = np.fft.fftn(yk)
    sigma_plus_plus_minus = yk_hat * z_exp_m1 * z_exp_m2

    # 4) compute \Sigma_--+
    # \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
    # \Sigma_{m1,m2,m3}^{--+} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3+1) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3/N)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
    sigma_minus_minus_plus = yk_hat * z_exp_m3
    logging.debug("End of Process")

    # 5) compute \Sigma_+-+
    # \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
    # \Sigma_{m1,m2,m3}^{+-+} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3 / N)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    # compute dcf
    logging.debug("Process: compute \Sigma_+-+ and compute \Sigma_-+-")
    x = np.arange(1,N+1) * h_x
    y = np.arange(-N,0,1) * h_y
    z = np.arange(1,N+1) * h_z
    grid = itertools.product(x,y,z)
    dcf = pool.map(_compute_delta_characterstic_function_3d, grid, N*N)
    dcf = np.array(dcf).reshape(N,N,N)

    yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * f3
    del dcf
    yk_hat = np.fft.fftn(yk)
    sigma_plus_minus_plus = yk_hat * z_exp_m1 * z_exp_m3

    # 6) compute \Sigma_-+-
    # \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
    # \Sigma_{m1,m2,m3}^{-+-} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = conj(\delta[(k1-N) hx, (k2+1) hy, (k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2/N) * exp(2.0 * pi* 1j * m3)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
    sigma_minus_plus_minus = yk_hat * z_exp_m2
    logging.debug("End of Process")

    # 7) compute \Sigma_+--
    # \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    # \Sigma_{m1,m2,m3}^{+--} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    # compute dcf
    logging.debug("Process: compute \Sigma_+-- and compute \Sigma_-++")
    x = np.arange(1,N+1) * h_x
    y = np.arange(-N,0,1) * h_y
    z = np.arange(-N,0,1) * h_z
    grid = itertools.product(x,y,z)
    #dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
    dcf = pool.map(_compute_delta_characterstic_function_3d, grid, N*N)
    dcf = np.array(dcf).reshape(N,N,N)

    yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * np.conjugate(f3[:,:,::-1])
    del dcf
    yk_hat = np.fft.fftn(yk)
    sigma_plus_minus_minus = yk_hat * z_exp_m1

    # 8) compute \Sigma_-++
    # \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    # \Sigma_{m1,m2,m3}^{-++} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
    # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
    yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
    sigma_minus_plus_plus = yk_hat * z_exp_m2 * z_exp_m3
    logging.debug("End of Process")

    #----------------------------------------#
    #----------------- FFT2D ----------------#
    #--- In this section, we apply 2D FFT ---#
    #--- and we propagate in 3 dimensions ---#
    #----------------------------------------#
    f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1))
    f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1))
    f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1))
    z_exp_m2 = np.exp(-two_pi* 1j * np.arange(N) / N)
    z_exp_m1 = z_exp_m2.reshape(N,1)

    # 9) compute \Sigma_++0
    # \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
    # \Sigma_{m1,m2,m3}^{++0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1) hx, (k2+1) hy, 0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N)
    # forall k1,k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_++0 and compute \Sigma_--0")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1) * h_x, (j+1) * h_y, 0]) for j in xrange(N)] for i in xrange(N)] )

    yk = dcf * f1.reshape(N,1) * f2
    yk_hat = np.fft.fft2(yk)
    sigma_plus_plus_0 = yk_hat * z_exp_m1 * z_exp_m2

    # 10) compute \Sigma_--0
    # \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
    # \Sigma_{m1,m2,m3}^{--0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = conj(\delta[-(k1-N) hx, -(k2-N) hy, 0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2)
    # forall k1,k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_minus_minus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
    logging.debug("End of Process")

    # 11) compute \Sigma_+-0
    # \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
    # \Sigma_{m1,m2,m3}^{+-0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k2-N) hy,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
    # forall k1,k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_+-0 and compute \Sigma_-+0")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, 0]) for j in xrange(N)] for i in xrange(N)])

    yk = dcf * f1.reshape(N,1) * np.conjugate(f2[::-1])
    yk_hat = np.fft.fft2(yk)
    sigma_plus_minus_0 = yk_hat * z_exp_m1

    # 12) compute \Sigma_-+0
    # \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
    # \Sigma_{m1,m2,m3}^{-+0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = conj(\delta[(k1-N) hx,0,(k2+1) hy,0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
    # forall k1,k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_minus_plus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
    logging.debug("End of Process")

    # 13) compute \Sigma_0++
    # \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    # \Sigma_{m1,m2,m3}^{0++} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[0,(k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
    # forall k2,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_0++ and compute \Sigma_0--")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j+1)*h_z]) for j in xrange(N)] for i in xrange(N)] )

    yk = dcf * f2.reshape(N,1) * f3
    yk_hat = np.fft.fft2(yk)
    sigma_0_plus_plus = yk_hat * z_exp_m1 * z_exp_m2

    # 14) compute \Sigma_0--
    # \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    # \Sigma_{m1,m2,m3}^{0-} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = conj(\delta[0,-(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3)
    # forall k2,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_0_minus_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
    logging.debug("End of Process")

    # 15) compute \Sigma_0+-
    # \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
    # \Sigma_{m1,m2,m3}^{0+-} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
    # forall k2,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_0+- and compute \Sigma_0-+")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )

    yk = dcf * f2.reshape(N,1) * np.conjugate(f3[::-1])
    yk_hat = np.fft.fft2(yk)
    sigma_0_plus_minus = yk_hat * z_exp_m1

    # 16) compute \Sigma_0-+
    # \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
    # \Sigma_{m1,m2,m3}^{0-+} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
    # forall k2,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_0_minus_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
    logging.debug("End of Process")

    # 17) compute \Sigma_+0+
    # \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    # \Sigma_{m1,m2,m3}^{+0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
    # forall k1,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_+0+ and compute \Sigma_-0-")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i + 1) * h_x, 0, (j + 1) * h_z]) for j in xrange(N)] for i in xrange(N)] )

    yk = dcf * f1.reshape(N,1) * f3
    yk_hat = np.fft.fft2(yk)
    sigma_plus_0_plus = yk_hat * z_exp_m1 * z_exp_m2

    # 18) compute \Sigma_-0-
    # \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    # \Sigma_{m1,m2,m3}^{-0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[-(k1-N) hx,0,-(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
    # forall k1,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_minus_0_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))
    logging.debug("End of Process")

    # 19) compute \Sigma_+0-
    # \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
    # \Sigma_{m1,m2,m3}^{+0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m3)
    # forall k1,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    logging.debug("Process: compute \Sigma_+0- and compute \Sigma_-0+")
    dcf = np.array([[distribution.computeDeltaCharacteristicFunction([(i+1)*h_x, 0, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )

    yk = dcf * f1.reshape(N,1) * np.conjugate(f3[::-1])
    yk_hat = np.fft.fft2(yk)
    sigma_plus_0_minus = yk_hat * z_exp_m1

    # 20) compute \Sigma_-0+
    # \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
    # \Sigma_{m1,m2,m3}^{-0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
    # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m3/N)
    # forall k1,k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 2, FFT should be of dimension 2
    sigma_minus_0_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2
    logging.debug("End of Process")

    #----------------------------------------#
    #----------------- FFT1D ----------------#
    #--- In this section, we apply 1D FFT ---#
    #--- and we propagate in 3 dimensions ---#
    #----------------------------------------#
    z_exp_m1 = np.exp(-two_pi* 1j * np.arange(N) / N)

    # 21) compute \Sigma_+00
    # \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
    # \Sigma_{m1,m2,m3}^{+00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N)
    # forall k1,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    logging.debug("Process: compute \Sigma_+00 and compute \Sigma_-00")
    dcf = np.array([distribution.computeDeltaCharacteristicFunction([(i+1)*h_x,0,0]) for i in xrange(N)])

    yk = dcf * f1
    yk_hat = np.fft.fft(yk)
    sigma_plus_0_0 = yk_hat * z_exp_m1

    # 22) compute \Sigma_-00
    # \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
    # \Sigma_{m1,m2,m3}^{-00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1)
    # forall k1,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    sigma_minus_0_0 = np.fft.fft(np.conjugate(yk[::-1]))
    logging.debug("End of Process")

    # 23) compute \Sigma_0+0
    # \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
    # \Sigma_{m1,m2,m3}^{0+0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[0,(k2+1)hy,0] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N)
    # forall k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    logging.debug("Process: compute \Sigma_0+0 and compute \Sigma_0-0")
    dcf = np.array([distribution.computeDeltaCharacteristicFunction([0,(i+1)*h_y,0]) for i in xrange(N)])

    yk = dcf * f2
    yk_hat = np.fft.fft(yk)
    sigma_0_plus_0 = yk_hat * z_exp_m1
    logging.debug("End of Process")

    # 24) compute \Sigma_0-0
    # \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
    # \Sigma_{m1,m2,m3}^{0-0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = conj(\delta[0,-(k2-N)hy,0]) * exp(- pi* 1j * (k2+1) * (tau_x - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2)
    # forall k2,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    sigma_0_minus_0 = np.fft.fft(np.conjugate(yk[::-1]))

    # 25) compute \Sigma_00+
    # \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
    # \Sigma_{m1,m2,m3}^{00+} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = \delta[0,0,(k3+1)hz] * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m3 / N)
    # forall k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    logging.debug("Process: compute \Sigma_00+ and compute \Sigma_00-")
    dcf = np.array([distribution.computeDeltaCharacteristicFunction([0,0,(i+1)*h_z]) for i in xrange(N)])

    yk = dcf * f3
    yk_hat = np.fft.fft(yk)
    sigma_0_0_plus = yk_hat * z_exp_m1

    # 26) compute \Sigma_00-
    # \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
    # \Sigma_{m1,m2,m3}^{00-} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
    # y_{k1,k2,0} = conj(\delta[0,0,-(k3-N)hz]) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
    # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m3)
    # forall k3,m1,m2,m3=0,1,...,N-1
    # Care components here are of dimension 1, FFT should be of dimension 1
    sigma_0_0_minus = np.fft.fft(np.conjugate(yk[::-1]))
    logging.debug("End of Process")

    s_m = sigma_plus_plus_plus + sigma_minus_minus_minus + sigma_plus_plus_minus + sigma_minus_minus_plus \
        + sigma_plus_minus_plus + sigma_minus_plus_minus + sigma_plus_minus_minus + sigma_minus_plus_plus \
        + sigma_plus_plus_0.reshape(N,N,1)  + sigma_minus_minus_0.reshape(N,N,1) \
        + sigma_plus_minus_0.reshape(N,N,1) + sigma_minus_plus_0.reshape(N,N,1) \
        + sigma_plus_0_plus.reshape(N,1,N)  + sigma_minus_0_minus.reshape(N,1,N) \
        + sigma_plus_0_minus.reshape(N,1,N) + sigma_minus_0_plus.reshape(N,1,N) \
        + sigma_0_plus_plus.reshape(1,N,N)  + sigma_0_minus_minus.reshape(1,N,N) \
        + sigma_0_plus_minus.reshape(1,N,N) + sigma_0_minus_plus.reshape(1,N,N) \
        + sigma_0_0_plus.reshape(1,1,N)     + sigma_0_0_minus.reshape(1,1,N) \
        + sigma_0_plus_0.reshape(1,N,1)     + sigma_0_minus_0.reshape(1,N,1) \
        + sigma_plus_0_0.reshape(N,1,1)     + sigma_minus_0_0.reshape(N,1,1)

    s_m *= (h_x * h_y * h_z) /(8*pi*pi*pi)
    pool.terminate()
    pool.close()
    return s_m

def compute_3d_grid(distribution, b, N):
    """
    This function is used for 3D computations
    It helps to generate the 3D grids and mesh
    """
    pi = np.pi
    two_pi = 2.0 * pi
    mu_x, mu_y, mu_z = tuple(distribution.getMean())
    sigma_x, sigma_y, sigma_z = tuple(distribution.getStandardDeviation())
    b_sigma_x, b_sigma_y , b_sigma_z = sigma_x * b, sigma_y * b, sigma_z * b
    tau_x, tau_y, tau_z = mu_x / b_sigma_x, mu_y / b_sigma_y, mu_z / b_sigma_z
    h_x, h_y, h_z = pi / b_sigma_x, pi / b_sigma_y, pi / b_sigma_z
    # compute the gaussian pdf on y_m + 2(k+1)b \sigma, k in 0,..,N-1, m in 0,..,N-1
    x_grid = mu_x + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_x
    y_grid = mu_y + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_y
    z_grid = mu_z + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_z
    alt_grid_mesher = distribution.temp_mesh_grid_.clone([2.0 * b_sigma_x, 2.0 * b_sigma_y, 2.0 * b_sigma_z])
    return [x_grid, y_grid, z_grid, alt_grid_mesher]

def compute_gaussian_equivalent_parallel(distribution, b, N, nprocs = multiprocessing.cpu_count()):
    """
    This function is used for 3D computations
    It helps to compute gaussian approximations on 3D regular grids
    The function uses parallel processes
    """
    global _compute_equivalent_normal_pdf_sum
    def _compute_equivalent_normal_pdf_sum(args):
        return distribution.computeEquivalentNormalPDFSum(*args)
    pool = multiprocessing.Pool(nprocs)
    [x_grid, y_grid, z_grid, alt_grid_mesher] = compute_3d_grid(distribution, b,N)
    imax = distribution.getEquivalentNormalPDFSumLevelMax(distribution.getMean(), alt_grid_mesher)
    grid = itertools.product(x_grid, y_grid, z_grid)
    x = itertools.izip(grid, itertools.cycle([alt_grid_mesher]), itertools.cycle([imax]))
    pdf = pool.map(_compute_equivalent_normal_pdf_sum, x)
    pdf = np.array(pdf)
    pdf = pdf.reshape(N,N,N)
    pool.terminate()
    pool.close()
    return pdf

def compute_analytical_pdf_parallel(distribution, b, N, nprocs = multiprocessing.cpu_count()):
    """
    This function is used for 3D computations
    It helps to compute the analytical PDF on a 3D grid using parallel processes
    """
    global _compute_pdf
    def _compute_pdf(y):
        return distribution.computePDF(y)
    pool = multiprocessing.Pool(nprocs)
    [x_grid, y_grid, z_grid, alt_grid_mesher] = compute_3d_grid(distribution,b,N)
    grid = itertools.product(x_grid, y_grid, z_grid)
    pdf = pool.map(_compute_pdf, grid)
    pdf = np.array(pdf)
    pdf = pdf.reshape(N,N,N)
    pool.terminate()
    pool.close()
    return pdf


def compute_pdf_on_3d_grid(distribution, b, N, nprocs=multiprocessing.cpu_count()):
    """
    The interest is to compute the density function on a 3D grid of form:
      r \in {1,2,3},\forall m\in\{0,\hdots,N-1\},\:y_{r,m}=\mu_r+b(\frac{2m+1}{N} - 1)\sigma_r

    The density is given by:
      p_{m1,,m2, m3}= Q_{m1,m2,m3}+S_{m1,m2,m3}
    with S_{m1,m2,m3} = \frac{hx hy hz}{8\pi^3}\sum_{k1=-N}^{N}\sum_{k2=-N}^{N}\sum_{k3=-N}^{N}\delta(k1 h_x,k2 h_y k3 h_z) E_{m1,m2,m3}(k1,k2,k3)
    Here :
      E_{m1,m2,m3}(k1,k2,k3)=e^{-i\sum_{j=1}^{3} k_jh_j (\mu_j+a (\frac{2m_j+1}{M}-1)\sigma_j)}
    Using FFT,
    S_{m1,m2,m3}=\frac{hx hy hz}{4\pi^2} \sum_{s1,s2,s3 \in [0,-,+]} \Sigma_{m1,m2,m3}^{s1 s2 s3}

    with
    \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
    \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
    \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
    \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
    \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
    \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
    \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
    \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
    \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
    \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
    \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
    \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
    \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
    \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
    \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
    \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
    \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0,(k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
    \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0,-(k2+1)hy,0) E_{m1,m2,m3}(0,-(k2+1),0)
    \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0,0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
    \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0,0,-(k3+1)hz) E_{m1,m2,m3}(0,0,-(k3+1)

    Parameters
    ----------
    b : positive float
        The number of marginal standard deviations beyond which the density is evaluated

    N : positive integer, preference of form N = 2^k
        The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

    Returns
    -------
    grid_values : ndarray of shape (N, 3)
                  3D Grid on which the probability density function has been evaluated

    pdf_values : ndarray of shape (N,N,N)
                The probability density function values on the grid

    Example
    -------
    >>> import openturns as ot
    >>> import MultivariateRandomMixture as MV
    >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0), ot.Uniform(2.0, 5.0)])
    >>> matrix = ot.Matrix([[1,2, 4], [3,4,5], [6,0,1]])
    >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix)
    >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
    >>> N = 128 # 128x128x128 points for the 3D grid
    >>> [grid_values, pdf_values] = dist.computePDFOn3DGrid(b, N)
    >>> xgrid_values, ygrid_values, zgrid_values = tuple(grid_values)
    >>> f = open("out.csv", "w")
    >>> f.write("x;y;z;pdf\n")
    >>> for i in xrange(len(xgrid_values)):
    ...     for j in xrange(len(ygrid_values)):
    ...         for k in xrange(len(zgrid_values)):
    ...             f.write("{0:.16g};{1:.16g};{2:.16g};{3:.16g}\n".format(xgrid_values[i], ygrid_values[j], zgrid_values[k], pdf_values[i][j][k]))
    ... f.close()

    """

    if distribution.dimension_ != 3:
        raise ValueError("Dimension of distribution should be 3")
    # get the grid
    logging.info("Starting evaluation of PDF on the 3D Grid...")
    [x_grid, y_grid, z_grid, alt_grid_mesher] = compute_3d_grid(distribution, b, N)
    logging.info("Number of parallel process : %d " %nprocs)
    if distribution.isAnalyticPDF_:
        logging.info("Analytical evaluation of the pdf")
        tic = time.time()
        pdf = compute_analytical_pdf_parallel(distribution, b, N, nprocs)
        toc = time.time()
        logging.info("End of analytical evaluation")
        logging.debug("It tooks %s second(s) to evaluate the analytical pdf" %(toc-tic))
    else:
        logging.info("Numerical approximation...")
        logging.info("Computing gaussian approximation")
        tic = time.time()
        pdf = compute_gaussian_equivalent_parallel(distribution, b, N, nprocs)
        toc = time.time()
        logging.info("End of gaussian approximation")
        logging.debug("It tooks %s second(s) to evaluate the gaussian approximation on the grid" %(toc-tic))
        # Compute the delta part
        logging.info("Delta characteristic function approximation")
        tic = time.time()
        #s_m = factorizer_compute_delta_function_on_3d_grid(distribution, b, N, nprocs)
        s_m = parallel_compute_delta_function_on_3d_grid(distribution, b, N, nprocs)
        toc = time.time()
        logging.info("End of Delta characteristic function evaluation")
        logging.debug("It tooks %s second(s) to evaluate the delta function on the grid" %(toc-tic))
        pdf += s_m.real
        pdf *= pdf > 0
    logging.info("End of computations")
    return [[x_grid, y_grid, z_grid],pdf]

if __name__== "__main__":
    # Log parameters
    import openturns as ot
    import MultivariateRandomMixture as MV
    logging.basicConfig(level=logging.DEBUG, format="%(levelname)s:%(asctime)s: %(message)s")
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.DEBUG)
    b = 6
    N = 128
    # Define the distribution
    collection = ot.DistributionCollection([ot.Uniform(), ot.Uniform(), ot.Uniform()])
    matrix = ot.Matrix([[1, 4, 5], [3, 2, -1], [0,0,1]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    # Analytical evaluation
    [grid, pdf_ana] = compute_pdf_on_3d_grid(distribution, b , N)
    tic = time.time()
    [g, pdf_values_ana] = distribution.computePDFOn3DGrid(b,N)
    toc = time.time()
    print "It tooks %s second(s) to evaluate the analytical pdf on the grid using internal method" %(toc-tic)
    # distribution set as non analytical
    distribution.isAnalyticPDF_ = False
    # Show message debug
    [grid, pdf_app] = compute_pdf_on_3d_grid(distribution, b , N)
    tic = time.time()
    [g, pdf_values] = distribution.computePDFOn3DGrid(b,N)
    toc = time.time()
    print "It tooks %s second(s) to evaluate the numerical pdf on the grid using internal method" %(toc-tic)
