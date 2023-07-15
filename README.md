# ctapp

This is a simple example for a Cantera derived application that is compiled against
Cantera source code.

This example adds Cython versions of new C++ classes, which are accessed from the
Python package `ctapp`:

* `Sqrt1` functor (new C++ `Func1` class) ... Python: `ct.Func1.cxx_functor("sqrt")`
* `ClonedReactor` (based on C++ `IdealGasConstPressureReactor`) ... Python: `ctapp.ClonedReactor`
* `NewFlow` (based on C++ `StFlow`) ... Python: `ctapp.NewFlame`

## Installation

### Set up a conda environment

> **Warning**
> This branch compiles against Cantera 3.0.0b1, which is still under development. Conda
> packages may not be up to date, so compilation from source is recommended.


Create and activate a conda environment

```
$ conda create -n ctapp -c main scons numpy cython boost-cpp eigen ruamel.yaml git ipython
$ conda activate ctapp
```

The package depends on `cantera` and `libcantera-devel`, both of which are available as
conda packages.

Install `cantera` and `libcantera-devel`

```
$ conda install -c cantera/label/dev cantera libcantera-devel
```

> **Note**
> As an alternative, Cantera can be installed from source in the same environment.

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

Run the functor example:
```
$ cd examples
$ python new_functor.py
```

**Linux/macOS only:** Run modified Cantera samples (the shell script `get_examples.sh`
downloads and modifies stock Cantera examples). Utilities `wget` and `sed` need to be
installed.

```
$ cd examples
$ sh get_examples.sh
$ python new_flame.py
$ python cloned_reactor.py
```

## Building Documentation

Build `doxygen` documentation using `doxygen-awesome-css` styling
(see [instructions](https://github.com/jothepro/doxygen-awesome-css#installation)):

```
$ doxygen source/doxygen/Doxyfile
```

Build `Sphinx` documentation (requires `sphinx-autodoc-typehints`,
`pydata-sphinx-theme`, `breathe`):

```
$ make html
```
