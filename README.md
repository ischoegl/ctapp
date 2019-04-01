# cantera-mod

This is a simple modification that adds a new version of a cython cdef'd class 
to cantera (adding clones of IdealGasFlow/BurnerFlame classes defined in cython).

Note: this example requires a *complete* cantera installation (including headers), which typically requires compilation from source. The code was tested for the `cantera 2.4` branch.

## Usage

Compile cython extension

```
# python setup.py build_ext --inplace
```

Install package

```
# pip install -e .
```

Open python console and verify that package loads without errors

```
$ python
...
>>> import canteramod as ct
```

Check that everything runs (`new_flame.py` is a version of the  cantera example `burner_flame.py` that uses the cloned object `ClonedFlame`)

```
$ cd examples
$ python new_flame.py
```
