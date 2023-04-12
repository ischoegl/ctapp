# @file _ctapp.pxd

from cpython.ref cimport PyObject

import cantera as ct
cimport cantera as ct

cdef extern from "cantera/cython/funcWrapper.h":
    cdef int translate_exception()

# new domain registration in C++
cdef extern from "NewFlow.h" namespace "CanteraApp":
    cdef void registerDomains() except +translate_exception

# cython portion
cdef class NewFlow(ct._FlowBase):
    pass

# new reactor registration in C++
cdef extern from "ClonedReactor.h" namespace "CanteraApp":
    cdef void registerReactors() except +translate_exception

# cython portion
cdef class ClonedReactor(ct.IdealGasConstPressureReactor):
    pass
