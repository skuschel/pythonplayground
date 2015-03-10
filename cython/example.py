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
