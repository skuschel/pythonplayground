#!/usr/bin/env python2

# Stephan Kuschel 2015

#compile in place with
#./setup.py build_ext --inplace

# taken vom cython page

from distutils.core import setup
from Cython.Build import cythonize

setup(name='test',
    ext_modules = cythonize("examplemodule.pyx")
)
