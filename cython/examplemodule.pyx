cimport cython


cpdef double sum(double x, double y):
    return x + y


# http://docs.cython.org/src/tutorial/numpy.html

import numpy as np
cimport numpy as np

ctypedef np.double_t DTYPE_t

def sumarray(np.ndarray data):
    cdef DTYPE_t ret = 0.0
    cdef int n = len(data)

    for i in xrange(n):
        ret += data[i]

    return ret
