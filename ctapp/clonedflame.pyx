# cython: embedsignature=True
# distutils: language = c++

cdef class ClonedFlow(ct._FlowBase):
    """
    An (almost exact) copy of IdealGasFlow as a new class
    """

    def __cinit__(self, ct._SolutionBase thermo, *args, **kwargs):
        gas = ct.getIdealGasPhase(thermo)
        self.flow = new ct.CxxStFlow(gas, thermo.n_species, 2)


class ClonedFlame(ct.BurnerFlame):
    """An (almost exact) clone of BurnerFlame, using ClonedFlow from above."""
    __slots__ = ('burner', 'flame', 'outlet')

    def __init__(self, gas, grid=None, width=None):
        """
        see BurnerFlame
        """
        self.burner = ct.Inlet1D(name='burner', phase=gas)
        self.outlet = ct.Outlet1D(name='outlet', phase=gas)
        if not hasattr(self, 'flame'):
            self.flame = ClonedFlow(gas, name='flame')
            self.flame.set_axisymmetric_flow()

        if width is not None:
            grid = np.array([0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]) * width

        super(ct.BurnerFlame, self).__init__(
            (self.burner, self.flame, self.outlet), gas, grid)

        self.burner.T = gas.T
        self.burner.X = gas.X
