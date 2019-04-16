# cython: embedsignature=True
# distutils: language = c++

cdef class ClonedFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow as a new class (cython only)
    """

    def __cinit__(self, ct._SolutionBase thermo, *args, **kwargs):
        gas = ct.getIdealGasPhase(thermo)
        self.flow = new ct.CxxStFlow(gas, thermo.n_species, 2)

cdef class NewFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow as a new class (inherited in C++)
    """

    def __cinit__(self, ct._SolutionBase thermo, *args, nextra=10, **kwargs):
        gas = ct.getIdealGasPhase(thermo)
        self.flow = <ct.CxxStFlow * >(new CxxNewFlow(gas, thermo.n_species, nextra, 2))
