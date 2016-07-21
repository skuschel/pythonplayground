#!/usr/bin/env python3

#Stephan Kuschel, 2015

from setuptools import setup, Extension

# in order to build and test use
# ./setup.py build_ext --inplace

# in order to install in development mode:
# ./setup.py develop --user
# rerun this command to recompile as well

exmodule = Extension('examplemodule',
                     sources=['examplemodule.c'])


setup(name='examplemodule',
      ext_modules=[exmodule])
