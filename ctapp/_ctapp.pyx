# cython: embedsignature=True
# distutils: language = c++

# This file is part of ctapp. See LICENSE file in the top-level directory
# for license and copyright information.

import cantera as ct
cimport cantera as ct

from cantera import interrupts

class CanteraError(RuntimeError):
    pass

cdef public PyObject* pyCanteraError = <PyObject*>CanteraError

__all__ = ['ClonedReactor', 'NewFlow', 'ClonedFunc1']

include "clonedflow.pyx"
include "clonedreactor.pyx"
include "functors.pyx"
