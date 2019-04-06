# @file ClonedReactor.h

# This file is part of the add-on package ctapp.
# See License.txt in the top-level directory

cdef extern from "cantera/cython/funcWrapper.h":
    cdef int translate_exception()

cdef extern from "ClonedReactor.h" namespace "CanteraApp":
    cdef void registerClonedReactor() except +translate_exception

cdef extern from "ClonedReactor.h":
    cdef cppclass CxxClonedReactor "CanteraApp::ClonedReactor":
        CxxClonedReactor()
