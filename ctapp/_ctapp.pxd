# @file _ctapp.pxd

from cpython.ref cimport PyObject

import cantera as ct
cimport cantera as ct

cdef extern from "cantera/cython/funcWrapper.h":
    cdef int translate_exception()

# new flow class defined via cython
cdef class ClonedFlow(ct._FlowBase):
    pass

# new flow class defined in C++
cdef extern from "NewFlow.h":
    cdef cppclass CxxNewFlow "CanteraApp::NewFlow":
        CxxNewFlow(ct.CxxIdealGasPhase * , int, int, int)

# cython portion
cdef class NewFlow(ct._FlowBase):
    pass

# new reactor class defined in C++
cdef extern from "ClonedReactor.h":
    cdef cppclass CxxClonedReactor "CanteraApp::ClonedReactor":
        CxxClonedReactor()

# new reactor registration in C++
cdef extern from "ClonedReactor.h" namespace "CanteraApp":
    cdef void registerClonedReactor() except +translate_exception

# cython portion
cdef class ClonedReactor(ct.IdealGasConstPressureReactor):
    pass
