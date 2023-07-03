"""
Example illustrating use of a new functor defined in ctapp.
"""
import ctapp
import math

# access ctapp functors (new functor is 'sqrt')
f1 = ctapp.ClonedFunc1.cxx_functor("sqrt")
f2 = ctapp.ClonedFunc1.cxx_functor("sqrt", 2.)

# evaluate functors
x = .5
print(f"x = {x}")
print(f"sqrt(x) = {f1(x)} (check: {math.sqrt(x)})")
print(f"sqrt(2*x) = {f2(x)} (check: {math.sqrt(2 * x)})")

# note that original Cantera functors are also available
f3 = ctapp.ClonedFunc1.cxx_functor("exp")
