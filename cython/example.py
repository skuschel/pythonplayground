#!/usr/bin/env python2

# Stephan Kuschel 2015

import examplemodule as em

print em.sum(7.1, 3.33)
print em.sum(4,1)

import numpy as np

data = np.random.rand(2e6)
datav = data[::2]
print 'numpy summe:    {:.17e}'.format(datav.sum())
print 'cython summe:   {:.17e}'.format(em.sumarray(datav))

import timeit
n = 100
t = timeit.Timer(lambda: datav.sum())
tn = t.timeit(number=n)/n
print 'numpy:  {:0.4e} s'.format(tn)
t = timeit.Timer(lambda: em.sumarray(datav))
tc = t.timeit(number=n)/n
print 'cython: {:0.4e} s'.format(tc)
print 'Faktor {:5.4f} schneller!'.format(tn/tc)


print
bins = 1000000
npart = 1e6
print '=== Histogramme  bins: {:6d}, npart: {:.1e}'.format(bins, npart)
data = np.random.random(npart)
weights = np.random.random(npart)

print '--- Histogram 1D'

#data.sort()

(histnp, _) = np.histogram(data, bins=20, range=(0.1,0.9))
hist = em.hist1d(data, 0.1, 0.9, 20)
#print hist
print 'Histogramme sind gleich (ohne weights): '
print hist - histnp

#(histnp, _) = np.histogram(data, bins=20, range=(0.1,0.9), weights=weights)
#hist = examplemodule.hist1d(data, 0.1, 0.9, 20, weights)
#print hist
#print 'Histogramme sind gleich (mit weights): '
#print hist - histnp

n = 10
tn = 1
t = timeit.Timer(lambda: np.histogram(data, range=(0.001,0.999), bins=bins))
tn = t.timeit(number=1)/1
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: em.hist1d(data, 0.001, 0.999, bins))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print 'Histogrammsumme: {:3.3e}'.format(hist.sum())
print 'Faktor {:5.1f} schneller!'.format(tn/tc)






