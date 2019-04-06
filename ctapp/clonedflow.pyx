# cython: embedsignature=True
# distutils: language = c++

cdef class ClonedFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow as a new class
    """

    def __cinit__(self, ct._SolutionBase thermo, *args, **kwargs):
        gas = ct.getIdealGasPhase(thermo)
        self.flow = new ct.CxxStFlow(gas, thermo.n_species, 2)
