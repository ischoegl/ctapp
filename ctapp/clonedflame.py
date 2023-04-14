#!/usr/bin/env python

import cantera as ct
import numpy as np
from ._ctapp import NewFlow

__all__ = ['NewFlame']


class NewFlame(ct.BurnerFlame):
    """An (almost exact) clone of BurnerFlame, using NewFlow (C++ defined)."""
    __slots__ = ('burner', 'flame', 'outlet')

    def __init__(self, gas, nextra=10, grid=None, width=None):
        """
        see BurnerFlame
        """
        self.burner = ct.Inlet1D(name='burner', phase=gas)
        self.outlet = ct.Outlet1D(name='outlet', phase=gas)
        if not hasattr(self, 'flame'):
            self.flame = NewFlow(gas, name='flame', nextra=nextra)

        if width is not None:
            grid = np.array([0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]) * width

        super(ct.BurnerFlame, self).__init__(
            (self.burner, self.flame, self.outlet), gas, grid)

        self.burner.T = gas.T
        self.burner.X = gas.X
