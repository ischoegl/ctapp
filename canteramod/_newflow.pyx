# cython: embedsignature=True
# distutils: language = c++

#import csv
#from cantera import interrupts
# cimport cantera
from cantera cimport CxxStFlow
from cantera cimport _SolutionBase
from cantera cimport _FlowBase
from cantera cimport getIdealGasPhase

# implementation file

from cantera import *

__all__ = ['ClonedFlow', 'ClonedFlame']

cdef class ClonedFlow(_FlowBase):
    """
    An (almost exact) copy of IdealGasFlow as a new class
    """

    def __cinit__(self, _SolutionBase thermo, *args, **kwargs):
        gas = getIdealGasPhase(thermo)
        self.flow = new CxxStFlow(gas, thermo.n_species, 2)


class ClonedFlame(FlameBase):
    """An (almost exact) copy of BurnerFlame, using ClonedFlow from above."""
    __slots__ = ('burner', 'flame', 'outlet')

    def __init__(self, gas, grid=None, width=None):
        """
        :param gas:
            `Solution` (using the IdealGas thermodynamic model) used to
            evaluate all gas properties and reaction rates.
        :param grid:
            A list of points to be used as the initial grid. Not recommended
            unless solving only on a fixed grid; Use the `width` parameter
            instead.
        :param width:
            Defines a grid on the interval [0, width] with internal points
            determined automatically by the solver.

        A domain of class `IdealGasFlow` named ``flame`` will be created to
        represent the flame and set to axisymmetric stagnation flow. The three
        domains comprising the stack are stored as ``self.burner``,
        ``self.flame``, and ``self.outlet``.
        """
        self.burner = Inlet1D(name='burner', phase=gas)
        self.outlet = Outlet1D(name='outlet', phase=gas)
        if not hasattr(self, 'flame'):
            # Create flame domain if not already instantiated by a child class
            self.flame = ClonedFlow(gas, name='flame')
            self.flame.set_axisymmetric_flow()

        if width is not None:
            grid = np.array([0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]) * width

        super().__init__((self.burner, self.flame, self.outlet), gas, grid)

        # Setting X needs to be deferred until linked to the flow domain
        self.burner.T = gas.T
        self.burner.X = gas.X

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
        u0 = self.burner.mdot / self.gas.density
        T0 = self.burner.T

        # get adiabatic flame temperature and composition
        self.gas.equilibrate('HP')
        Teq = self.gas.T
        Yeq = self.gas.Y
        u1 = self.burner.mdot / self.gas.density

        locs = [0.0, 0.2, 1.0]
        self.set_profile('u', locs, [u0, u1, u1])
        self.set_profile('T', locs, [T0, Teq, Teq])
        for n in range(self.gas.n_species):
            self.set_profile(self.gas.species_name(n),
                             locs, [Y0[n], Yeq[n], Yeq[n]])

    def solve(self, loglevel=1, refine_grid=True, auto=False):
        """
        Solve the problem.

        :param loglevel:
            integer flag controlling the amount of diagnostic output. Zero
            suppresses all output, and 5 produces very verbose output.
        :param refine_grid:
            if True, enable grid refinement.
        :param auto: if True, sequentially execute the different solution stages
            and attempt to automatically recover from errors. Attempts to first
            solve on the initial grid with energy enabled. If that does not
            succeed, a fixed-temperature solution will be tried followed by
            enabling the energy equation, and then with grid refinement enabled.
            If non-default tolerances have been specified or multicomponent
            transport is enabled, an additional solution using these options
            will be calculated.
        """

        # Use a callback function to check that the flame has not been blown off
        # the burner surface. If the user provided a callback, store this so it
        # can called in addition to our callback, and restored at the end.
        original_callback = self._steady_callback

        class FlameBlowoff(Exception):
            pass

        if auto:
            def check_blowoff(t):
                T = self.T
                n = max(3, len(self.T) // 5)

                # Near-zero temperature gradient at burner indicates blowoff
                if abs(T[n] - T[0]) / (T[-1] - T[0]) < 1e-6:
                    raise FlameBlowoff()

                if original_callback:
                    return original_callback(t)
                else:
                    return 0.0

            self.set_steady_callback(check_blowoff)

        try:
            return super().solve(loglevel, refine_grid, auto)
        except FlameBlowoff:
            # The eventual solution for a blown off flame is the non-reacting
            # solution, so just set the state to this now
            self.set_flat_profile(self.flame, 'T', self.T[0])
            for k, spec in enumerate(self.gas.species_names):
                self.set_flat_profile(self.flame, spec, self.burner.Y[k])

            self.set_steady_callback(original_callback)
            super().solve(loglevel, False, False)
            if loglevel > 0:
                print('Flame has blown off of burner (non-reacting solution)')

        self.set_steady_callback(original_callback)
