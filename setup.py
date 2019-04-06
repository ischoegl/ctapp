#!/usr/bin/env python
from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize
import pkg_resources
import glob

__CANTERA_OBJ = pkg_resources.resource_filename('cantera', '_cantera.*so')
__CANTERA_OBJ = glob.glob(__CANTERA_OBJ)[0]

extensions = [
    Extension(
        "ctapp._ctapp",
        ["canteramod/_ctapp.pyx"],
        extra_objects=[__CANTERA_OBJ],
    ),
]

setup(
    name="ctapp",
    description='Example for compilation of custom cython code against stock cantera',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'License :: MIT',
        'Programming Language :: Python :: 3',
    ],
    install_requires=['cantera'],
    packages=find_packages(),
    ext_modules=cythonize(extensions))
