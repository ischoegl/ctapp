# @file _ctapp.pxd

# This file is part of the add-on package ctapp.
# See License.txt in the top-level directory

import cantera as ct
cimport cantera as ct

cdef extern from "cantera/cython/funcWrapper.h":
    cdef int translate_exception()

cdef extern from "ClonedReactor.h" namespace "CanteraApp":
    cdef void registerClonedReactor() except +translate_exception

cdef extern from "ClonedReactor.h":
    cdef cppclass CxxClonedReactor "CanteraApp::ClonedReactor":
        CxxClonedReactor()

cdef class ClonedFlow(ct._FlowBase):
    pass

cdef class ClonedReactor(ct.IdealGasConstPressureReactor):
    pass
