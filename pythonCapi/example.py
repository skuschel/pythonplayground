#!/usr/bin/env python2


import examplemodule

print 'multiply'
print examplemodule.multiply(3,4)

print 'sum'
print examplemodule.sum(3,8)


import numpy as np
data = np.arange(1e6)
datav = data[::4]
print datav.sum()
# summiert offensichtlich nur das erste viertel auf, da die Daten nicht zusammenhaengend im
# Speicher liegen
print examplemodule.sumarray(datav)
print data[:1e6/4].sum()
print examplemodule.sumarrayiterator(datav)  # der Iterator macht es wieder richtig

import timeit
n = 100
t = timeit.Timer(lambda: data.sum())
tn = t.timeit(number=n)/n
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: examplemodule.sumarray(data))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print 'Faktor {:5.2f} schneller!'.format(tn/tc)
t = timeit.Timer(lambda: examplemodule.sumarrayiterator(data))
tc = t.timeit(number=n)/n
print 'c (iterator): {:0.4e}'.format(tc)
print 'Faktor {:5.2f} schneller!'.format(tn/tc)

print
bins = 1000000
npart = 5e6
print '=== Histogramme  bins: {:6d}, npart: {:.1e}'.format(bins, npart)
data = np.random.random(npart)
weights = np.random.random(npart)

print '--- Histogram 1D'

#data.sort()

(histnp, _) = np.histogram(data, bins=20, range=(0.1,0.9))
hist = examplemodule.hist1d(data, 0.1, 0.9, 20)
#print hist
print 'Histogramme sind gleich (ohne weights): '
print hist - histnp
(histnp, _) = np.histogram(data, bins=20, range=(0.1,0.9), weights=weights)
hist = examplemodule.hist1d(data, 0.1, 0.9, 20, weights)
#print hist
print 'Histogramme sind gleich (mit weights): '
print hist - histnp

n = 10
tn = 1
t = timeit.Timer(lambda: np.histogram(data, range=(0.001,0.999), bins=bins, weights=weights))
tn = t.timeit(number=1)/1
print 'numpy: {:0.4e}'.format(tn)
t = timeit.Timer(lambda: examplemodule.hist1d(data, 0.001, 0.999, bins, weights))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print 'Histogrammsumme: {:3.3e}'.format(hist.sum())
print 'Faktor {:5.1f} schneller!'.format(tn/tc)


print
print '--- Histogram 1D -- Top Hat shape'
hist = examplemodule.hist1dtophat(data, 0.1, 0.9, 50)
#print hist
n = 10
t = timeit.Timer(lambda: examplemodule.hist1dtophat(data, 0.001, 0.999, bins, weights))
tc = t.timeit(number=n)/n
print 'c: {:0.4e}'.format(tc)
print 'Histogrammsumme: {:3.3e}'.format(hist.sum())
print 'Faktor {:5.1f} schneller!'.format(tn/tc)



