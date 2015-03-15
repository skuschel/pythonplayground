#!/usr/bin/env python2

# Stephan Kuschel 2015

import examplemodule as em
import numpy as np
import timeit

bins = (1000, 700)
npart = 2e6
print '=== Histogramme  bins: {:6s}, npart: {:.1e}'.format(bins, npart)
datax = np.random.rand(npart)
datay = np.random.rand(npart)
weights = np.random.random(npart)

print '--- Histogram 1D'

#data.sort()

(histnp, _, _) = np.histogram2d(datax, datay, bins=(10, 15), range=((0.1, 0.9), (0.1,0.9)))
(hist, _, _) = em.histogram2d(datax, datay, range=((0.1, 0.9), (0.1,0.9)), bins=(10,15))
#print hist
print 'Histogramme sind gleich (ohne weights): '
print hist - histnp

(histnp, _, _) = np.histogram2d(datax, datay, weights=weights, bins=(10, 10), range=((0.1, 0.9), (0.1,0.9)))
(hist, _, _) = em.histogram2d(datax, datay, weights=weights, range=((0.1, 0.9), (0.1,0.9)), bins=(10,10))
#print hist
print 'Histogramme sind gleich (mit weights): '
print hist - histnp

n = 10
t = timeit.Timer(lambda: np.histogram2d(datax, datay, range=((0.001, 0.999), (0.001,0.999)), bins=bins, weights=weights))
tn = t.timeit(number=1)/1
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: em.histogram2d(datax, datay,range=((0.001, 0.999), (0.001,0.999)), bins=bins, weights=weights))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print 'Histogrammsumme: {:3.3e}'.format(hist.sum())
print 'Faktor {:5.1f} schneller!'.format(tn/tc)
