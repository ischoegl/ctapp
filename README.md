# ctapp

This is a simple example for a cantera derived application that adds new C++ and cython cdef'd classes (illustrated using a clone of IdealGasFlow defined in cython, and a clone of IdealGasConstPressureReactor defined in C++).

__Notes:__ this example requires a *complete* cantera installation (including headers), which typically requires compilation from source. 

 * Objects for `ClonedFlow`, `ClonedReactor` work with stock cantera, and were tested for the `v2.4` branch and the current development branch (`v2.5.0ra2`) on ubuntu 18.04.
 * The object `NewFlow` adds equations, and requires a patched version of `StFlow.h` / `boundaries1D.cpp`.

## Usage

Compile cython extension

```
# python setup.py build_ext --inplace
```

Install package

```
# pip install -e .
```

Open python console, verify that the package loads without errors, and that newly defined objects are present.

```
$ python
...
>>> import ctapp
>>> dir(ctapp)
```

Check that everything runs (the shell script `get_examples.sh` downloads and modifies stock cantera examples)

```
$ cd examples
$ sh get_examples.sh
$ python cloned_flame.py
$ python cloned_reactor.py
```
