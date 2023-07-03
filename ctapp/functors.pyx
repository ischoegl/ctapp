# This file is part of ctapp. See LICENSE file in the top-level directory
# for license and copyright information.

cdef __register_functors():
    registerFunctors()

__register_functors()


cdef class ClonedFunc1(ct.Func1):
    pass
