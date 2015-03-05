#!/usr/bin/env python2

#  Copyright Joel de Guzman 2002-2007. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#  Hello World Example from the tutorial

import hello_ext
print hello_ext.greet()

print hello_ext.sum(3.7,4.1)
print hello_ext.sum(3,4)

print hello_ext.sumarray([4,5,6, 7.7])

import numpy as np

data = np.random.rand(1e6)
datalist = list(data)
print 'boost result:  {:.17f}'.format(hello_ext.sumarray(datalist))
print 'numpy result:  {:.17f}'.format(data.sum())

import timeit
n = 100
t = timeit.Timer(lambda: data.sum())
tn = t.timeit(number=n)/n
print 'numpy: {:0.4e} s'.format(tn)
t = timeit.Timer(lambda: hello_ext.sumarray(datalist))
tb = t.timeit(number=n)/n
print 'boost: {:0.4e} s'.format(tb)
print 'Faktor {:5.3f} schneller! (naja also boost ist deutlich langsamer)'.format(tn/tb)
