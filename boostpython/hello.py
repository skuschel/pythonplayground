#!/usr/bin/env python2

#  Stephan Kuschel, 2015

import hello_ext
print hello_ext.greet()

print hello_ext.sum(3.7,4.1)
print hello_ext.sum(3,4)

print hello_ext.sumarray([4,5,6, 7.7])

import numpy as np

data = np.random.rand(1e6)
datalist = list(data)
print 'numpy result:          {:.17f}'.format(data.sum())
print 'boost result:          {:.17f}'.format(hello_ext.sumarray(datalist))
print 'boost numeric result:  {:.17f}'.format(hello_ext.sumarraynp(data))

import timeit
n = 20
t = timeit.Timer(lambda: data.sum())
tn = t.timeit(number=n)/n
print 'numpy: {:0.4e} s'.format(tn)
t = timeit.Timer(lambda: hello_ext.sumarray(datalist))
tb = t.timeit(number=n)/n
print 'boost: {:0.4e} s'.format(tb)
print 'Faktor {:5.3f} schneller! (naja also boost ist deutlich langsamer)'.format(tn/tb)
t = timeit.Timer(lambda: hello_ext.sumarraynp(data))
tb = t.timeit(number=n)/n
print 'boost numeric: {:0.4e} s'.format(tb)
print 'Faktor {:5.3f} schneller! (naja also boost ist deutlich langsamer)'.format(tn/tb)
