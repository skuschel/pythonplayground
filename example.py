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
tn = t.timeit(number=n)/n
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: examplemodule.sumarray(data))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print '=== Faktor {:5.1f} schneller! ==='.format(tn/tc)


print
print 'Histogram 1D'
data = np.random.random(2e6)
(hist2, _) = np.histogram(data, bins=50, range=(0.1,0.9))
hist = examplemodule.hist1d(data, 0.1, 0.9, 50)
print hist
print 'Histogramme sind gleich: '
print hist - hist2
n = 10
t = timeit.Timer(lambda: np.histogram(data, range=(0.001,0.999), bins=500))
tn = t.timeit(number=n)/n
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: examplemodule.hist1d(data, 0.001, 0.999, 500))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print '=== Faktor {:5.1f} schneller! ==='.format(tn/tc)
