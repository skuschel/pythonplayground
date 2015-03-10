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


def hist1d(np.ndarray[DTYPE_t, ndim=1] data, double min, double max, int bins=20):
    cdef np.ndarray[DTYPE_t, ndim=1] ret = np.zeros(bins, dtype=np.double);
    cdef int n = len(data)
    cdef double tmp = 1.0 / (max - min) * bins
    cdef double x
    for i in xrange(n):
        x = (data[i] - min) * tmp;
        if x > 0.0 and x < bins:
            ret[np.floor(x)] += 1.0
    return ret
