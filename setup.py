#!/usr/bin/env python2

#Stephan Kuschel, 2015

from setuptools import setup, Extension

# in order to build and test use
# ./setup.py build --build-lib .

exmodule = Extension('examplemodule',
                     sources=['examplemodule.c'])


setup(name='examplemodule',
      ext_modules=[exmodule])
