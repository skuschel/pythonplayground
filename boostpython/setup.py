#!/usr/bin/env python2

#Stephan Kuschel, 2015

from setuptools import setup, Extension

# in order to build and test use
# ./setup.py build_ext --inplace

# in order to install in development mode:
# ./setup.py develop --user
# rerun this command to recompile as well

# Very helpful on how to use boost:


exmodule = Extension('hello_ext',
                     sources=['hello.cpp'],
                     libraries=['boost_python'])


setup(name='examplemodule',
      ext_modules=[exmodule])


