# see https://stackoverflow.com/questions/32528560/using-setuptools-to-create-a-cython-package-calling-an-external-c-library

from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "canteramod._newflow",
        ["canteramod/_newflow.pyx"],
        libraries=['cantera'],
    ),
]

setup(
    name="canteramod",
    packages=find_packages(),
    ext_modules=cythonize(extensions))
