#!/usr/bin/env python
from sys import platform
import pkg_resources
from pathlib import Path
import numpy as np
from setuptools import setup, Extension, find_packages
from sysconfig import get_paths
from Cython.Build import cythonize

__PATHS = get_paths()
__INCLUDE = Path(__PATHS["include"]).parent

__CANTERA_LIBS = ["cantera_shared"] # dynamic libraries

# cantera library
if platform == "win32":
    __CANTERA_LIBS = [f"{lib}.lib" for lib in __CANTERA_LIBS]
    extra_comp_args = ["/EHsc /std:c++17"]
    __CONDA = __INCLUDE / "Library"
    extra_link_args = [f"/LIBPATH:{__CONDA / 'lib'}"]
else:
    __CANTERA_LIBS = [f"-l{lib}" for lib in __CANTERA_LIBS]
    extra_comp_args = ["-std=c++17"]
    __CONDA = __INCLUDE.parent / "lib"
    extra_link_args = []
if platform == "darwin":
    extra_link_args.extend(["-framework", "Accelerate"])
extra_comp_args.append("-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION")

__CANTERA_DEP = pkg_resources.resource_filename('cantera', 'interrupts.py')
__DEPENDS = [__CANTERA_DEP]

def readme():
    with open('README.md') as f:
        return f.read()

with Path("ctapp/_version.py").open() as version_file:
    exec(version_file.read())


extensions = [
    Extension(
        "ctapp._ctapp",
        ["ctapp/_ctapp.pyx",
         "ctapp/NewFlow.cpp"],
        include_dirs=[np.get_include(), str(__INCLUDE)],
        extra_objects=__CANTERA_LIBS,
        extra_compile_args=extra_comp_args,
        extra_link_args=extra_link_args,
        depends=__DEPENDS,
        language='c++17',
    ),
]

setup(
    name="ctapp",
    version=__version__,
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
    author=__author__,
    author_email='ischoegl@lsu.edu',
    license='MIT',
    packages=find_packages(),
    install_requires=['cantera>=3.0.0a5'],
    ext_modules=cythonize(extensions, compiler_directives={'language_level' : "3"}))
