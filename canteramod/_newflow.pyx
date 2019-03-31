#cython: embedsignature=True
#distutils: language = c++

#import csv
from cantera import interrupts
#cimport cantera
#from cantera cimport CxxIdealGasPhase
from cantera cimport CxxStFlow
from cantera cimport CxxAxiStagnFlow
from cantera cimport _SolutionBase
from cantera cimport _FlowBase
from cantera cimport getIdealGasPhase

# implementation file

from cantera import *

cdef class NewFlow(_FlowBase):
    """
    An (almost exact) copy of AxisymmetricStagnationFlow as a new class
    ... skip getIdealGasPhase
    """
    def __cinit__(self, _SolutionBase thermo, *args, **kwargs):
        gas = getIdealGasPhase(thermo)
        #gas = <CxxIdealGasPhase*>(thermo.thermo)
        self.flow = <CxxStFlow*>(new CxxAxiStagnFlow(gas, thermo.n_species, 2))


class NewFlame(FlameBase):
    """An (almost exact) copy of BurnerFlame, using NewFlow from above."""

    def __init__(self, gas, grid=None):
        """
        :param gas:
            `Solution` (using the IdealGas thermodynamic model) used to
            evaluate all gas properties and reaction rates.
        :param grid:
            Array of initial grid points

        A domain of class `AxisymmetricStagnationFlow` named ``flame`` will
        be created to represent the flame. The three domains comprising the
        stack are stored as ``self.burner``, ``self.flame``, and
        ``self.outlet``.
        """
        self.burner = Inlet1D(name='burner', phase=gas)
        self.burner.T = gas.T
        self.outlet = Outlet1D(name='outlet', phase=gas)
        self.flame = NewFlow(gas, name='flame')

        super().__init__((self.burner, self.flame, self.outlet), gas, grid)

    def set_initial_guess(self):
        """
        Set the initial guess for the solution. The adiabatic flame
        temperature and equilibrium composition are computed for the burner
        gas composition. The temperature profile rises linearly in the first
        20% of the flame to Tad, then is flat. The mass fraction profiles are
        set similarly.
        """
        super().set_initial_guess()

        self.gas.TPY = self.burner.T, self.P, self.burner.Y
        Y0 = self.burner.Y
        u0 = self.burner.mdot/self.gas.density
        T0 = self.burner.T

        # get adiabatic flame temperature and composition
        self.gas.equilibrate('HP')
        Teq = self.gas.T
        Yeq = self.gas.Y
        u1 = self.burner.mdot/self.gas.density

        locs = [0.0, 0.2, 1.0]
        self.set_profile('u', locs, [u0, u1, u1])
        self.set_profile('T', locs, [T0, Teq, Teq])
        for n in range(self.gas.n_species):
            self.set_profile(self.gas.species_name(n),
                             locs, [Y0[n], Yeq[n], Yeq[n]])

