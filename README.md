# ctapp

This is a simple example for a Cantera derived application that is compiled against Cantera source
code.

This example adds Cython versions of new C++ classes (illustrated using clones of
`IdealGasFlow` and `IdealGasConstPressureReactor` defined in C++).

> **Warning**
> This branch compiles against Cantera 3.0.0ab1, which is still under development. Conda
> packages may not be up to date, so compilation from source is recommended.

> **Note**
> The package is untested on Windows.

## Installation

### Set up a conda environment

Create and activate a conda environment

```
$ conda create -n ctapp -c conda-forge scons numpy cython boost-cpp eigen ruamel.yaml git ipython
$ conda activate ctapp
```

The package depends on `cantera` and `libcantera-devel`, both of which are available as conda
packages.

Install `cantera` and `libcantera-devel`

```
$ conda install -c conda-forge cantera libcantera-devel
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

Check that everything runs (the shell script `get_examples.sh` downloads and
modifies stock cantera examples). Utilities `wget` and `sed` need to be installed.

```
$ cd examples
$ sh get_examples.sh
$ python new_flame.py
$ python cloned_reactor.py
```

> **Note**
> On recent conda versions, there may be a mismatch of `libstdc++` between
> the conda environment and the system `g++` (see
> [workaround](https://github.com/stan-dev/pystan/issues/294#issuecomment-878292636))
