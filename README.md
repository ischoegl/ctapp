# cantera-mod

This is a simple modification that adds a new version of a cython cdef'd class 
to cantera (adding a clone of the AxisymmetricStagnationFlow class defined in cython).

Note: this example requires a *complete* cantera installation (including headers), which typically requires compilation from source. The code was tested for `cantera v2.4.0` (the cloned object is deprecated and is slated to be removed in later releases).

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

Check that everything runs (`new\_flame.py` is a version of the  cantera example `burner\_flame.py` that uses the cloned object `NewFlame`)

```
$ cd examples
$ python new_flame.py
```
