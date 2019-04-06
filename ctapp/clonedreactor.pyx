# cython: embedsignature=True
# distutils: language = c++

cdef __register():
    registerClonedReactor()

__register()

cdef class ClonedReactor(ct.IdealGasConstPressureReactor):
    """
    a reactor cloned from IdealGasConstPressureReactor
    rest is done by inheritance
    """
    reactor_type = "ClonedReactor"
