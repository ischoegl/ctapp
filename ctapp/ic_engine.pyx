cdef class Cylinder(IdealGasReactor):
     """
     An engine cylinder (IdealGasReactor with engine related utility functions).
     """
     reactor_type = "Cylinder"

     def __init__(self, contents=None, *, name=None, **kwargs):
         """
         Arguments follow :class:`IdealGasReactor`. An examples for object
         creation is shown below.

         >>> gas = Solution('gri30.xml')
         >>> r1 = Cylinder(gas)
         """
         super().__init__(contents, name=name, **kwargs)

     def install(self, PistonBase piston):
         """ Install piston """
         (<CxxCylinder*>self.reactor).install(deref(piston.piston))

     property bore:
         """ Cylinder bore diameter [m] """
         def __get__(self):
             return (<CxxCylinder*>self.reactor).getBoreDiameter()
         def __set__(self, bore):
             (<CxxCylinder*>self.reactor).setBoreDiameter(bore)

     property compression_ratio:
         """ Compression ratio [-] """
         def __get__(self):
             return (<CxxCylinder*>self.reactor).getCompressionRatio()
         def __set__(self, cr):
             (<CxxCylinder*>self.reactor).setCompressionRatio(cr)

     property displacement:
         """ Cylinder displacement [-] """
         def __get__(self):
             return (<CxxCylinder*>self.reactor).getDisplacement()
         def __set__(self, dis):
             (<CxxCylinder*>self.reactor).setDisplacement(dis)

     def set_profile(self, pos, time):
         """Set a cylinder volume profile.

         :param pos:
             array of volumes [m**3]
         :param time:
             array of times [s]

         >>> cyl.set_profile(np.array([0.1, 0.5, 1.0]),
         ...                 np.array([0.05, 0.1, 0.2])
         """
         cdef vector[double] x, y
         for p in pos:
             x.push_back(p)
         for t in time:
             y.push_back(t)
         (<CxxCylinder*>self.reactor).setProfile(x, y)

     property volume_min:
         """ Minimum volume [m^3] """
         def __get__(self):
             return (<CxxCylinder*>self.reactor).getVMin()

     property volume_max:
         """ Maximum volume [m^3] """
         def __get__(self):
             return (<CxxCylinder*>self.reactor).getVMax()




cdef class PistonBase(WallBase):
     """
     Common base class for pistons.
     """

     def __init__(self, *args, **kwargs):
         super().__init__(*args, **kwargs)
         self.piston = <CxxPistonBase*>(self.wall)

     property check:
         """Check if piston is set up correctly"""
         def __get__(self):
             return pybool(self.piston.check())

     property stroke_length:
         """Piston stroke [m]. """
         def __get__(self):
             return self.piston.getStrokeLength()

     def position(self, double t):
         """Piston position in [m]. """
         return self.piston.position(t)

     def velocity(self, double t):
         """Piston velocity in [m/s]. """
         return self.piston.velocity(t)

cdef class FloatingPiston(PistonBase):
     r"""
     """
     wall_type = "FloatingPiston"


cdef class DrivenPiston(PistonBase):
     r"""
     """
     wall_type = "DrivenPiston"

     def __init__(self, left, right, *, name=None, A=None, U=None, Q=None):
         """
         :param left:
             Reactor or reservoir on the left. Required.
         :param right:
             Reactor or reservoir on the right. Required.
         :param name:
             Name string. If omitted, the name is ``'Wall_n'``, where ``'n'``
             is an integer assigned in the order walls are created.
         :param A:
             Wall area [m^2]. Defaults to 1.0 m^2.
         :param U:
             Overall heat transfer coefficient [W/m^2]. Defaults to 0.0
             (adiabatic wall).
         :param Q:
             Heat flux function :math:`q_0(t)` [W/m^2]. Optional. Default:
             :math:`q_0(t) = 0.0`.

         An example for object creation is shown below.

         >>> p = DrivenPiston(left, right)
         """
         super().__init__(left, right, name=name, A=A, U=U, Q=Q)

     property stroke_length:
         """Piston stroke [m]. """
         def __get__(self):
             return self.piston.getStrokeLength()
         def __set__(self, double value):
             (<CxxDrivenPiston*>self.piston).setStrokeLength(value)


cdef class ReciprocatingPiston(PistonBase):
     r"""
     """
     wall_type = "ReciprocatingPiston"

     def __init__(self, left, right, *, name=None, A=None, U=None, Q=None,
                  rev_per_minute=None, stroke_length=None,
                  rod_radius_ratio=None, start_crank_angle=None):
         """
         :param left:
             Reactor or reservoir on the left. Required.
         :param right:
             Reactor or reservoir on the right. Required.
         :param name:
             Name string. If omitted, the name is ``'Wall_n'``, where ``'n'``
             is an integer assigned in the order walls are created.
         :param A:
             Wall area [m^2]. Defaults to 1.0 m^2.
         :param U:
             Overall heat transfer coefficient [W/m^2]. Defaults to 0.0
             (adiabatic wall).
         :param Q:
             Heat flux function :math:`q_0(t)` [W/m^2]. Optional. Default:
             :math:`q_0(t) = 0.0`.
         :param rev_per_minute:
             Revolutions per minute.
         :param rod_radius_ratio:
             Ratio of rod length to stroke length.

         Examples for object creation are shown below.

         >>> p = ReciprocatingPiston(left, right, rev_per_minute=rpm,
                                      rod_radius_ratio=r)

         This is equivalent to:

         >>> p = ReciprocatingPiston(left, right)
         >>> p.rev_per_minute = rpm
         >>> p.rod_radius_ratio = r
         """
         super().__init__(left, right, name=name, A=A, U=U, Q=Q)

         if rev_per_minute is not None:
             self.rev_per_minute = rev_per_minute
         if  rod_radius_ratio is not None:
             self.rod_radius_ratio = rod_radius_ratio

     property cycle:
         """Engine cycle duraction [rad]. """
         def __get__(self):
             return (<CxxReciprocatingPiston*>self.piston).cycle()

     property rev_per_minute:
         """Engine speed in rpm. """
         def __get__(self):
             return (<CxxReciprocatingPiston*>self.piston).getRPM()
         def __set__(self, double value):
             (<CxxReciprocatingPiston*>self.piston).setRPM(value)

     property omega:
         """Engine speed [rad/s]. """
         def __get__(self):
             return (<CxxReciprocatingPiston*>self.piston).getOmega()
         def __set__(self, double value):
             (<CxxReciprocatingPiston*>self.piston).setOmega(value)

     property stroke_number:
         """Number of strokes per full cycle """
         def __get__(self):
             return (<CxxReciprocatingPiston*>self.piston).getStrokeNumber()
         def __set__(self, int value):
             (<CxxReciprocatingPiston*>self.piston).setStrokeNumber(value)

     property rod_radius_ratio:
         """Ratio of rod length to crank radius [-]. """
         def __get__(self):
             return (<CxxReciprocatingPiston*>self.piston).getRodRadiusRatio()
         def __set__(self, double value):
             (<CxxReciprocatingPiston*>self.piston).setRodRadiusRatio(value)

     def crank_angle(self, double t):
         """Crank angle [rad].  """
         return (<CxxReciprocatingPiston*>self.piston).crankAngle(t)

cdef class Solenoid(FlowDevice):
     r"""
     In Cantera, a `Solenoid` is a flow device that opens and closes at specified
     times. If linked to a `ReciprocatingPiston` object, the timing is syncronized
     with the crank angle.

     When open, the mass flow rate is either constant to deliver a given mass

     .. math:: \dot m = m / (t_c - t_o)

     with :math:`m` being the cumulative mass, and :math:`t_o` and :math:`t_c` being
     times when the solenoid opens and closes. Alternatively, the mass flow rate is
     a function of the pressure drop across it. The behavior is linear:

     .. math:: \dot m = K_v*(P_1 - P_2)

     where :math:`K_v` is a constant set by the `set_valve_coeff` method.
     Note that :math:`P_1` must be greater than :math:`P_2`; otherwise,
     :math:`\dot m = 0`.
     """
     flowdevice_type = "Solenoid"

     # The signature of this function causes warnings for Sphinx documentation
     def __init__(self, upstream, downstream, *, name=None, coeff=None, mass=None):
         """
         :param upstream:
             Upstream reactor or reservoir. Required.
         :param downstream:
             Downstream reactor or reservoir. Required.
         :param name:
             Name string. If omitted, a default value is assigned.
         :param coeff:
             Pressure coefficient [kg/s/Pa].
         :param mass:
             Cumulative mass added while the solenoid is open [kg].

         Examples for object creation are shown below.

         >>> s = Solenoid(up, dn, coeff=c)

         This is equivalent to:

         >>> s = Solenoid(up, dn)
         >>> s.pressure_coeff = c
         """
         super().__init__(upstream, downstream, name=name)
         if coeff is not None:
             self.pressure_coeff = coeff
         if mass is not None:
             self.added_mass = mass

     property pressure_coeff:
         """
         Set the relationship between mass flow rate and the pressure drop across
         the valve using a proportionality constant [kg/s/Pa].

         >>> s = Solenoid(res1, reactor1)
         >>> s.pressure_coeff = 1e-4
         """
         def __get__(self):
             return (<CxxSolenoid*>self.dev).getPressureCoeff()
         def __set__(self, k):
             (<CxxSolenoid*>self.dev).setPressureCoeff(k)

     property added_mass:
         """
         Set the cumulative  mass flow [kg] passing through the solenoid.

         >>> s = Solenoid(res1, reactor1)
         >>> s.added_mass = 1e-4
         """
         def __get__(self):
             return (<CxxSolenoid*>self.dev).getAddedMass()
         def __set__(self, m):
             (<CxxSolenoid*>self.dev).setAddedMass(m)

     def set_master(self, WallBase piston):
         """ Set synchronization source. """
         (<CxxSolenoid*>self.dev).setMaster(piston.wall)

     property has_cycle:
         """ Check synchronization source. """
         def __get__(self):
             return pybool((<CxxSolenoid*>self.dev).hasCycle())

     def timer(self, t):
         """ Get timing signal. """
         return (<CxxSolenoid*>self.dev).timer(t)

     def is_open(self, t):
         """ Get timing signal. """
         return pybool((<CxxSolenoid*>self.dev).isOpen(t))

     property _open:
         """ Timing for Solenoid open (time or crank angle). """
         def __get__(self):
             return (<CxxSolenoid*>self.dev).getOpenTiming()
         def __set__(self, t):
             (<CxxSolenoid*>self.dev).setOpenTiming(t)

     property _close:
         """ Timing for Solenoid close (time or crank angle). """
         def __get__(self):
             return (<CxxSolenoid*>self.dev).getCloseTiming()
         def __set__(self, t):
             (<CxxSolenoid*>self.dev).setCloseTiming(t)

     property timing:
         """
         Solenoid open/close timing specified in terms of time [s]
         or crank angle [rad] if linked to a `ReciprocatingPiston` object.

         >>> s = Solenoid(res1, reactor1)
         >>> s.timing = 0.1, 0.2

         or

         >>> p = ReciprocatingPiston(res2, cylinder1)
         >>> s = Solenoid(res1, cylinder1)
         >>> s.set_master(p)
         >>> s.timing = -10. * np.pi/180., 10. * np.pi/180.
         """
         def __get__(self):
             return self._open, self._close
         def __set__(self, timing):
             self._open = timing[0]
             self._close = timing[1]

     property duration:
         """Duration of the open solenoid [s] or [rad] """
         def __get__(self):
             return (<CxxSolenoid*>self.dev).getOpenDuration()

     property timing_deg:
         """Solenoid open/close timing in radians"""
         def __get__(self):
             assert self.has_cycle, 'Solenoid is not synchronized'
             return self._open * 180. / np.pi, self._close * 180. / np.pi
         def __set__(self, timing):
             assert self.has_cycle, 'Solenoid is not synchronized'
             self._open = timing[0] * np.pi / 180.
             self._close = timing[1] * np.pi / 180.
