# ctapp

This is a simple example for a cantera derived application that is compiled
against cantera source code.

This example adds new C++ and cython cdef'd classes (illustrated using clones of
`IdealGasFlow` and `IdealGasConstPressureReactor` defined in C++).

__Notes:__
 * the package depends on `cantera` and `libcantera-devel`, both of which
are now available as conda packages.
 * at the moment, the package requires a Linux installation

## Installation

### Set up a conda environment

Create and activate a conda environment

```
$ conda create -n cantera-dev scons numpy cython ruamel_yaml libboost git ipython
$ conda activate cantera-dev
```

Install cantera and libcantera-devel

```
$ conda install -c cantera/label/dev cantera
$ conda install -c cantera/label/dev libcantera-devel
```

### Install ctapp

Clone the repository

```
$ git clone https://github.com/ischoegl/ctapp.git
$ cd ctapp
```

Optional step: to shows compilation progress prior to installation, run

```
# python setup.py build_ext --inplace
```

Run the installer for the local project path

```
# pip install -e .
```

Note: the `-e` (`--editable`) flag creates a version that is linked and will
automatically update.

## Usage

Open a python console, verify that the package loads without errors, and that
newly defined objects are present.

```
$ python
...
>>> import ctapp
>>> dir(ctapp)
```

Check that everything runs (the shell script `get_examples.sh` downloads and
modifies stock cantera examples)

```
$ cd examples
$ sh get_examples.sh
$ python cloned_flame.py
$ python cloned_reactor.py
```
