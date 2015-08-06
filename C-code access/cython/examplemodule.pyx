cimport cython


cpdef double sum(double x, double y):
    return x + y


# http://docs.cython.org/src/tutorial/numpy.html

import numpy as np
cimport numpy as np

ctypedef np.double_t DTYPE_t

def sumarray(np.ndarray[DTYPE_t, ndim=1] data):
    cdef DTYPE_t ret = 0.0
    cdef int n = len(data)

    for i in xrange(n):
        ret += data[i]

    return ret


def hist1d(np.ndarray[DTYPE_t, ndim=1] data, double min, double max, int bins=20, np.ndarray[DTYPE_t, ndim=1] weights=None, int order=0):
    cdef np.ndarray[DTYPE_t, ndim=1] ret = np.zeros(bins, dtype=np.double);
    cdef int n = len(data)
    cdef double tmp = 1.0 / (max - min) * bins
    cdef double x
    cdef int xr
    if order == 0:
        # normal Histogram
        for i in xrange(n):
            x = (data[i] - min) * tmp;
            if x > 0.0 and x < bins:
                if weights is None:
                    ret[<int>x] += 1.0
                else:
                    ret[<int>x] += weights[i]
    elif order == 1:
        # Particle shape is spline of order 1 = TopHat
        for i in xrange(n):
            x = (data[i] - min) * tmp;
            xr = <int>(x + 0.5);
            if (xr >= 0.0 and xr < bins):
                if weights is None:
                    ret[xr] += (0.5 + x - xr) * 1.0
                    if (xr > 1.0):
                        ret[xr - 1] += (0.5 - x + xr) * 1.0
                else:
                    ret[xr] += (0.5 + x - xr) * weights[i]
                    if (xr > 1.0):
                        ret[xr - 1] += (0.5 - x + xr) * weights[i]
    return ret




def histogram2d(np.ndarray[np.double_t, ndim=1] datax, np.ndarray[np.double_t, ndim=1] datay,
                np.ndarray[np.double_t, ndim=1] weights=None,
                range=None, bins=(20, 20), int order=0):
    '''
    Mimics numpy.histogram2d.
    Additional Arguments:
        - order = 0:
            sets the order of the particle shapes.
            order = 0 returns a normal histogram.
            order = 1 uses top hat particle shape.
    '''
    cdef int n = len(datax)
    if n != len(datay):
            raise ValueError('datax and datay must be of equal length')
    cdef np.ndarray[np.double_t, ndim=2] ret = np.zeros(bins, dtype=np.double)
    cdef double xmin, xmax, ymin, ymax
    if range is None:
        xmin = np.min(datax)
        xmax = np.max(datax)
        ymin = np.min(datay)
        ymax = np.max(datay)
    else:
        xmin = range[0][0]
        xmax = range[0][1]
        ymin = range[1][0]
        ymax = range[1][1]
    cdef int xbins = bins[0]
    cdef int ybins = bins[1]
    xedges = np.linspace(xmin, xmax, xbins+1)
    yedges = np.linspace(ymin, ymax, ybins+1)
    cdef double xtmp = 1.0 / (xmax - xmin) * xbins
    cdef double ytmp = 1.0 / (ymax - ymin) * ybins
    cdef double x
    cdef double y
    if order == 0:
        # normal Histogram
        for i in xrange(n):
            x = (datax[i] - xmin) * xtmp
            y = (datay[i] - ymin) * ytmp
            if x > 0.0 and y > 0.0 and x < xbins and y < ybins:
                if weights is None:
                    ret[<int>x, <int>y] += 1.0
                else:
                    ret[<int>x, <int>y] += weights[i]
    elif order == 1:
        pass

    return ret, xedges, yedges

