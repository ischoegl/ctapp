class TestCylinder(utilities.CanteraTest):
     reactorClass = ct.Cylinder

     def make_cylinder(self, T=300, P=101325, X='O2:1.0'):

         self.net = ct.ReactorNet()

         self.gas = ct.Solution('h2o2.xml')
         self.gas.TPX = T, P, X
         self.cyl = self.reactorClass(self.gas)

         self.net.add_reactor(self.cyl)
         self.gas.TPX = T, P, X
         self.amb = ct.Reservoir(self.gas)

     def ic_piston(self, omega=None, **kwargs):
         piston = ct.ReciprocatingPiston(self.cyl, self.amb, **kwargs)
         if omega is not None:
             piston.omega = omega
         return piston

     def add_solenoid(self, a, b, **kwargs):
         return ct.Solenoid(a, b, **kwargs)

     def test_cylinder1(self):
         displacement = .5e-3
         epsilon =  12.
         bore = .083
         self.make_cylinder()
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Function requires attached piston.'):
             self.cyl.displacement = displacement
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Function requires attached piston.'):
             self.cyl.compression_ratio = epsilon
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Function requires attached piston.'):
             self.cyl.bore = bore

     def test_cylinder2(self):
         displacement = .5e-3
         epsilon =  12.
         bore = .083
         self.make_cylinder()
         piston = ct.FloatingPiston(self.cyl, self.amb)
         self.cyl.install(piston)
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Only implemented for ReciprocatingPiston.'):
             self.cyl.displacement = displacement
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Only implemented for ReciprocatingPiston.'):
             self.cyl.compression_ratio = epsilon
         piston = ct.DrivenPiston(self.cyl, self.amb)
         self.cyl.install(piston)
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Only implemented for ReciprocatingPiston.'):
             self.cyl.displacement = displacement
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Only implemented for ReciprocatingPiston.'):
             self.cyl.compression_ratio = epsilon

     def test_cylinder3(self):
         displacement = .5e-3
         epsilon =  12.
         bore = .083
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)

         # initial setup works as expected
         self.cyl.displacement = displacement
         self.assertEqual(self.cyl.displacement, displacement)
         self.cyl.bore = bore
         self.assertEqual(self.cyl.displacement, displacement)
         self.assertEqual(self.cyl.bore, bore)
         self.cyl.compression_ratio = epsilon
         self.assertEqual(self.cyl.displacement, displacement)
         self.assertEqual(self.cyl.bore, bore)
         self.assertEqual(self.cyl.compression_ratio, epsilon)

         # everything is set up correctly
         self.assertNear(piston.area, .25 * np.pi * bore**2)
         self.assertNear(piston.stroke_length, displacement / piston.area)
         self.assertNear(self.cyl.volume_min, displacement / (epsilon - 1.))

         # geometry can be changed independently
         self.cyl.displacement = .5 * displacement
         self.assertEqual(self.cyl.displacement, .5 * displacement)
         self.assertEqual(self.cyl.bore, bore)
         self.assertEqual(self.cyl.compression_ratio, epsilon)
         self.cyl.bore = .5 * bore
         self.assertEqual(self.cyl.displacement, .5 * displacement)
         self.assertEqual(self.cyl.bore, .5 * bore)
         self.assertEqual(self.cyl.compression_ratio, epsilon)
         self.cyl.compression_ratio = .5 * epsilon
         self.assertEqual(self.cyl.displacement, .5 * displacement)
         self.assertEqual(self.cyl.bore, .5 * bore)
         self.assertEqual(self.cyl.compression_ratio, .5 * epsilon)

     def test_install(self):
         self.make_cylinder()
         piston1 = ct.FloatingPiston(self.amb, self.cyl)
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Piston is not set up correctly: '
                                     'Cylinder needs to be on left.'):
             self.cyl.install(piston1)

     def test_reciprocating_piston1(self):
         f = 50.
         r = 1.5
         self.make_cylinder()
         piston = self.ic_piston(omega=2.*np.pi*f, rod_radius_ratio=r)
         self.cyl.install(piston)
         self.assertEqual(piston.omega, 2. * np.pi * f)
         self.assertEqual(piston.rev_per_minute, f * 60)
         self.assertEqual(piston.rod_radius_ratio, r)
         self.assertEqual(piston.stroke_number, 4)
         self.assertNear(piston.cycle, 4.*np.pi)
         piston.stroke_number = 2
         self.assertEqual(piston.stroke_number, 2)
         self.assertNear(piston.cycle, 2.*np.pi)

     def test_reciprocating_piston2(self):
         displacement = .5e-3
         epsilon =  12.
         bore = .083
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)
         self.cyl.displacement = displacement
         self.cyl.bore = bore
         self.cyl.compression_ratio = epsilon

         net = ct.ReactorNet()
         net.add_reactor(self.cyl)
         net.set_max_time_step(1.e-3)

         for t in [.1, .5, 1.]:
             net.advance(t)
             self.assertNear(self.cyl.volume, piston.position(t) * piston.area,
                             rtol=1.e-6)

     def test_driven_piston1(self):
         vs = np.array([0.1, 0.5, 1.0])
         ts = np.array([0.05, 0.1, 0.2])
         self.make_cylinder()
         piston = self.ic_piston()
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Function requires attached piston.'):
             self.cyl.set_profile(vs, ts)
         self.cyl.install(piston)
         with self.assertRaisesRegex(ct.CanteraError,
                                     'Only implemented for DrivenPiston.'):
             self.cyl.set_profile(vs, ts)

         piston = ct.DrivenPiston(self.cyl, self.amb)
         self.cyl.install(piston)
         self.cyl.set_profile(vs, ts)
         self.assertEqual(piston.position(0.), 0.1)
         self.assertEqual(piston.position(0.1), .5)
         self.assertEqual(piston.position(0.2), 1.)

         v = (vs[2] - vs[1]) / (ts[2] - ts[1])
         self.assertEqual(piston.velocity(0.), 0.)
         self.assertEqual(piston.velocity(0.1), v)
         self.assertEqual(piston.velocity(0.2), 0.)

     def test_driven_piston2(self):
         vs = np.array([0.1, 0.5, 1.0])
         ts = np.array([0.05, 0.1, 0.2])
         self.make_cylinder()
         piston = ct.DrivenPiston(self.cyl, self.amb)
         self.cyl.install(piston)
         self.cyl.set_profile(vs, ts)

         self.assertEqual(piston.position(0.), 0.1)
         self.assertEqual(piston.position(0.1), .5)
         self.assertEqual(piston.position(0.2), 1.)

         v = (vs[2] - vs[1]) / (ts[2] - ts[1])
         self.assertEqual(piston.velocity(0.), 0.)
         self.assertEqual(piston.velocity(0.1), v)
         self.assertEqual(piston.velocity(0.2), 0.)

         net = ct.ReactorNet()
         net.add_reactor(self.cyl)
         net.set_max_time_step(1.e-4)

         for t in ts:
             net.advance(t)
             self.assertNear(self.cyl.volume, piston.position(t) * piston.area,
                             rtol=1.e-6)

     def test_floating_piston1(self):

         self.make_cylinder()
         piston = ct.FloatingPiston(self.cyl, self.amb)
         net = ct.ReactorNet()
         net.add_reactor(self.cyl)

         with self.assertRaisesRegex(ct.CanteraError,
                                     'Function requires attached piston.'):
             net.advance(.1)

     def test_floating_piston2(self):

         self.make_cylinder()
         piston = ct.FloatingPiston(self.cyl, self.amb)
         self.cyl.install(piston)
         net1 = ct.ReactorNet()
         net1.add_reactor(self.cyl)

         r = ct.Reactor(self.gas)
         self.assertEqual(self.cyl.thermo.P, r.thermo.P)
         self.assertEqual(self.cyl.thermo.T, r.thermo.T)
         wall = ct.Wall(r, self.amb)
         net2 = ct.ReactorNet()
         net2.add_reactor(r)

         for t in [1.e-4, 1.e-3, 1.e-2, 1.e-1, 1.]:
             net1.advance(t)
             net2.advance(t)
             self.assertNear(self.cyl.volume, r.volume,
                             rtol=1.e-6)

     def test_master(self):
         coeff = 1.e-6
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)
         inlet = self.add_solenoid(self.amb, self.cyl)

         # untethered
         t = .1
         self.assertFalse(inlet.has_cycle)
         self.assertEqual(inlet.timer(t), t)

         # synchronized
         inlet.set_master(piston)
         self.assertTrue(inlet.has_cycle)
         self.assertEqual(inlet.timer(t), piston.crank_angle(t))

     def test_solenoid1(self):
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)
         self.injector = self.add_solenoid(self.amb, self.cyl)

         def tester(obj, t0, t1, eps=1.e-3):
             obj.assertFalse(obj.injector.is_open(t0 - eps))
             obj.assertTrue(obj.injector.is_open(t0 + eps))
             obj.assertTrue(obj.injector.is_open(t1 - eps))
             obj.assertFalse(obj.injector.is_open(t1 + eps))

         # testing without synchronizing solenoid to valve
         timing = 0.1, 0.2
         self.injector.timing = timing
         self.assertEqual(timing, self.injector.timing)
         tester(self, *timing)

         # synchronize solenoid to piston
         self.injector.set_master(piston)

         # 0 < open < close < cycle
         self.injector.timing = np.pi, 3. * np.pi
         tester(self, np.pi, 3 * np.pi)

         # 0 < close < open < cycle
         self.injector.timing = 3. * np.pi, np.pi
         tester(self, 3. * np.pi, np.pi)

         # open < 0 < close < cycle
         self.injector.timing = -np.pi, np.pi
         tester(self, 3. * np.pi, np.pi)

         # close < 0 < open < cycle
         self.injector.timing = np.pi, -np.pi
         tester(self, np.pi, 3. * np.pi)

         # 0 < open < cycle < close
         self.injector.timing = 3. * np.pi, 5. * np.pi
         tester(self, 3. * np.pi, np.pi)

         # 0 < close < cycle < open
         self.injector.timing = 5. * np.pi, 3. * np.pi
         tester(self, np.pi, 3. * np.pi)

     def test_solenoid2(self):
         coeff = 1.e-6
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)
         inlet = self.add_solenoid(self.amb, self.cyl, coeff=coeff)
         self.assertEqual(inlet.pressure_coeff, coeff)

         timing = 0.1, 0.2
         inlet.timing = timing
         mdot = coeff * (self.amb.thermo.P - self.cyl.thermo.P)
         self.assertNear(inlet.mdot(0.), 0.)
         self.assertNear(inlet.mdot(0.15), mdot)
         self.assertNear(inlet.mdot(0.25), 0.)

     def test_solenoid3(self):
         mass = 3.2e-5
         self.make_cylinder()
         piston = self.ic_piston()
         self.cyl.install(piston)
         injector = self.add_solenoid(self.amb, self.cyl, mass=mass)
         self.assertEqual(injector.added_mass, mass)

         timing = 0.1, 0.2
         injector.timing = timing
         self.assertNear(injector.duration, 0.1)
         mdot = mass / injector.duration
         self.assertNear(injector.mdot(0.), 0.)
         self.assertNear(injector.mdot(0.15), mdot)
         self.assertNear(injector.mdot(0.25), 0.)

         # synchronize solenoid to piston
         injector.set_master(piston)
         injector.timing = np.pi, 1.1 * np.pi
         tau = np.pi / piston.omega
         eps = 1.e-5
         mdot = piston.omega * mass / injector.duration
         self.assertNear(injector.mdot(tau - eps), 0.)
         self.assertNear(injector.mdot(tau + eps), mdot)
         self.assertNear(injector.mdot(1.1 * tau - eps), mdot)
         self.assertNear(injector.mdot(1.1 * tau + eps), 0.)

         piston.stroke_number = 2
         injector.set_master(piston)
         self.assertNear(injector.mdot(tau - eps), 0.)
         self.assertNear(injector.mdot(tau + eps), mdot)
         self.assertNear(injector.mdot(1.1 * tau - eps), mdot)
         self.assertNear(injector.mdot(1.1 * tau + eps), 0.)
