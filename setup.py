#!/usr/bin/env python
from sys import platform
import glob
import pkg_resources
import os
import numpy as np
from setuptools import setup, Extension, find_packages
from sysconfig import get_paths
from Cython.Build import cythonize
from Cython.Compiler import Options

Options.language_level = 3

__INCLUDE = get_paths()['include'].split(os.sep)[:-1]
__INCLUDE = (os.sep).join(__INCLUDE)

# cantera library
if platform == "win32":
    __CANTERA_OBJ = pkg_resources.resource_filename('cantera', '_cantera.*lib')
else:
    __CANTERA_OBJ = pkg_resources.resource_filename('cantera', '_cantera.*so')
__CANTERA_OBJ = glob.glob(__CANTERA_OBJ)[0]

__CANTERA_DEP = pkg_resources.resource_filename('cantera', 'interrupts.py')


def readme():
    with open('README.md') as f:
        return f.read()


extensions = [
    Extension(
        "ctapp._ctapp",
        ["ctapp/_ctapp.pyx",
         "ctapp/NewFlow.cpp"],
        include_dirs=[np.get_include(), __INCLUDE],
        extra_objects=[__CANTERA_OBJ],
        depends=[__CANTERA_DEP],
        language='c++11',
    ),
]

setup(
    name="ctapp",
    description='Example for compilation of custom classes against stock cantera',
    long_description=readme(),
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'License :: MIT',
        'Programming Language :: Python :: 3',
    ],
    keywords='cantera',
    url='https://github.com/ischoegl/ctapp.git',
    author='Ingmar Schoegl',
    author_email='ischoegl@lsu.edu',
    license='MIT',
    packages=find_packages(),
    install_requires=['cantera>=2.6.0a1'],
    ext_modules=cythonize(extensions))
