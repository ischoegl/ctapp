# cython: embedsignature=True
# distutils: language = c++

import cantera as ct
cimport cantera as ct

from cantera import interrupts

class CanteraError(RuntimeError):
    pass

cdef public PyObject* pyCanteraError = <PyObject*>CanteraError

__all__ = ['ClonedFlow', 'ClonedReactor', 'NewFlow']

include "clonedflow.pyx"
include "clonedreactor.pyx"
