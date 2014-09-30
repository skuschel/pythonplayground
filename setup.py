#!/usr/bin/env python2

from distutils.core import setup, Extension

exmodule = Extension('examplemodule',
                     sources=['examplemodule.c'])


setup(name='examplemodule',
      ext_modules=[exmodule])
