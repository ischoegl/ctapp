# cython: embedsignature=True
# distutils: language = c++

cdef __register_domains():
    registerDomains()

__register_domains()

cdef class ClonedFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow
    """
    _domain_type = "cloned-flow"

cdef class NewFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow with minor updates
    """
    _domain_type = "new-flow"
