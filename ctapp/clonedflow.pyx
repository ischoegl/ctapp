# cython: embedsignature=True
# distutils: language = c++

cdef __register_domains():
    registerDomains()

__register_domains()

cdef class NewFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow with an extra equation
    """
    _domain_type = "new-flow"
