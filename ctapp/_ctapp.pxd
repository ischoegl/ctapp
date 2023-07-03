# @file _ctapp.pxd

from cpython.ref cimport PyObject

import cantera as ct
cimport cantera as ct
from cantera cimport translate_exception

# new domain registration in C++
cdef extern from "NewFlow.h" namespace "CanteraApp":
    cdef void registerDomains() except +translate_exception

# new reactor registration in C++
cdef extern from "ClonedReactor.h" namespace "CanteraApp":
    cdef void registerReactors() except +translate_exception

# new functor registration in C++
cdef extern from "NewFunc1.h" namespace "CanteraApp":
    cdef void registerFunctors() except +translate_exception
