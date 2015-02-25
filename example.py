#!/usr/bin/env python2


import examplemodule

print 'multiply'
print examplemodule.multiply(3,4)

print 'sum'
print examplemodule.sum(3,8)


import numpy as np
data = np.random.random(2000)
print data.sum()
print examplemodule.sumarray(data)

import timeit
n = 10000
t = timeit.Timer(lambda: data.sum())
print 'numpy: {:0.4e}'.format(t.timeit(number=n)/n)
t = timeit.Timer(lambda: examplemodule.sumarray(data))
print 'c: {:0.4e}'.format(t.timeit(number=n)/n)

