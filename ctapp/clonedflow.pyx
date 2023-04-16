# cython: embedsignature=True
# distutils: language = c++

# This file is part of ctapp. See LICENSE file in the top-level directory
# for license and copyright information.

cdef __register_domains():
    registerDomains()

__register_domains()

cdef class NewFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow with an extra equation
    """
    _domain_type = "new-flow"
