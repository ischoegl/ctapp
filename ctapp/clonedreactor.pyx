# This file is part of ctapp. See LICENSE file in the top-level directory
# for license and copyright information.

cdef __register_reactors():
    registerReactors()

__register_reactors()


cdef class ClonedReactor(ct.IdealGasConstPressureReactor):
    """
    a reactor cloned from IdealGasConstPressureReactor
    rest is done by inheritance
    """
    reactor_type = "cloned-reactor"
