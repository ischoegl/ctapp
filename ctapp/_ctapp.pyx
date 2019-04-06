# cython: embedsignature=True
# distutils: language = c++

import numpy as np
cimport numpy as np

import cantera as ct
cimport cantera as ct

# from cython.operator cimport dereference as deref, preincrement as inc

from cantera import interrupts

__all__ = ['ClonedFlow', 'ClonedFlame', 'ClonedReactor']

include "clonedflame.pyx"
include "clonedreactor.pyx"
