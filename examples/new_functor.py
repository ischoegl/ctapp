"""
Example illustrating use of a new functor defined in ctapp.
"""
import cantera as ct
import ctapp # not referenced directly, but needed to load custom functor
import math

# access custom functor (new functor is 'sqrt')
f1 = ct.Func1.cxx_functor("sqrt")
f2 = ct.Func1.cxx_functor("sqrt", 2.)

# evaluate functors
x = .5
print(f"x = {x}")
print(f"sqrt(x) = {f1(x)} (check: {math.sqrt(x)})")
print(f"sqrt(2*x) = {f2(x)} (check: {math.sqrt(2 * x)})")

# access original Cantera functor
f3 = ct.Func1.cxx_functor("exp")
