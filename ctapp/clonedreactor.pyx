# cython: embedsignature=True
# distutils: language = c++

cdef __register():
    registerClonedReactor()

__register()

cdef class ClonedReactor(ct.IdealGasPressureReactor):
    """
    a reactor cloned from IdealGasPressureReactor
    rest is done by inheritance
    """
    reactor_type = "ClonedReactor"
