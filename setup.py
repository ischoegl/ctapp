#!/usr/bin/env python
from sys import platform
import glob
import pkg_resources
import os
from pathlib import Path
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
    __PYCANTERA_OBJ = pkg_resources.resource_filename("cantera", "_cantera*.pyd")
    __PYCANTERA_OBJ = glob.glob(__PYCANTERA_OBJ)[0]

    __CANTERA_OBJ = Path(__PYCANTERA_OBJ).parents[3] / "Library" / "lib" / "cantera.lib"
    if not __CANTERA_OBJ.is_file:
        raise Exception("Cannot locate Cantera installation")
    __YAML_OBJ = __CANTERA_OBJ.parent / "yaml-cpp.lib"
    if not __YAML_OBJ.is_file:
        raise Exception("Cannot locate yaml-cpp installation")
    __EXTRA_OBJ = [str(__CANTERA_OBJ), str(__YAML_OBJ)]

    __INCLUDE = Path(__INCLUDE) / "Library" / "include"
    __INCLUDE = str(__INCLUDE)

    __CANTERA_DEP = pkg_resources.resource_filename('cantera', 'interrupts.py')
    __DEPENDS = [__CANTERA_DEP] # str(__PYCANTERA_OBJ)
else:
    __CANTERA_OBJ = pkg_resources.resource_filename('cantera', '_cantera.*so')
    __CANTERA_OBJ = glob.glob(__CANTERA_OBJ)[0]
    __EXTRA_OBJ = [str(__CANTERA_OBJ)]

    __CANTERA_DEP = pkg_resources.resource_filename('cantera', 'interrupts.py')
    __DEPENDS = [__CANTERA_DEP]


def readme():
    with open('README.md') as f:
        return f.read()


extensions = [
    Extension(
        "ctapp._ctapp",
        ["ctapp/_ctapp.pyx",
         "ctapp/NewFlow.cpp"],
        include_dirs=[np.get_include(), __INCLUDE],
        extra_objects=__EXTRA_OBJ,
        depends=__DEPENDS,
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
