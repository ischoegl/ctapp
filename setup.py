#!/usr/bin/env python
from setuptools import setup, Extension, find_packages
from Cython.Build import cythonize
import pkg_resources
import glob

__CANTERA_OBJ = pkg_resources.resource_filename('cantera', '_cantera.*so')
__CANTERA_OBJ = glob.glob(__CANTERA_OBJ)[0]

__CANTERA_DEP = pkg_resources.resource_filename('cantera', 'interrupts.py')


def readme():
    with open('README.md') as f:
        return f.read()

extensions = [
    Extension(
        "ctapp._ctapp",
        ["ctapp/_ctapp.pyx"],
        language='c++',
        extra_objects=[__CANTERA_OBJ],
        depends=[__CANTERA_DEP],
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
    install_requires=['cantera'],
    packages=find_packages(),
    install_requires=['cantera>=2.4.0'],
    ext_modules=cythonize(extensions))
