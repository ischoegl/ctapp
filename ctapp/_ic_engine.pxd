     cdef cppclass CxxCylinder "Cantera::Cylinder" (CxxReactor):
         CxxCylinder()
         void install(CxxPistonBase&) except +translate_exception
         void setBoreDiameter(double) except +translate_exception
         double getBoreDiameter() except +translate_exception
         void setCompressionRatio(double) except +translate_exception
         double getCompressionRatio() except +translate_exception
         void setDisplacement(double) except +translate_exception
         void setProfile(vector[double]&, vector[double]&) except +translate_exception
         double getDisplacement() except +translate_exception
         double getVMin() except +translate_exception
         double getVMax() except +translate_exception

     cdef cppclass CxxPistonBase "Cantera::PistonBase" (CxxWallBase):
         CxxPistonBase()
         cbool check()
         void setZMin() except +translate_exception
         double getZMin()
         void setZMax() except +translate_exception
         double getZMax()
         double getStrokeLength()
         double position(double) except +translate_exception
         double velocity(double) except +translate_exception

     cdef cppclass CxxFloatingPiston "Cantera::FloatingPiston" (CxxPistonBase):
         CxxFloatingPiston()

     cdef cppclass CxxDrivenPiston "Cantera::DrivenPiston" (CxxPistonBase):
         CxxDrivenPiston()
         void setStrokeLength(double)

     cdef cppclass CxxReciprocatingPiston "Cantera::ReciprocatingPiston" (CxxPistonBase):
         CxxReciprocatingPiston()
         void setRPM(double)
         double getRPM()
         double cycle()
         void setOmega(double)
         double getOmega()
         void setStrokeNumber(int)
         int getStrokeNumber()
         void setRodRadiusRatio(double)
         double getRodRadiusRatio()
         double crankAngle(double)

     cdef cppclass CxxSolenoid "Cantera::Solenoid" (CxxFlowDevice):
         CxxSolenoid()
         void setMaster(CxxWallBase*)
         cbool hasCycle()
         double timer(double)
         cbool isOpen(double)
         double getOpenDuration()
         void setPressureCoeff(double)
         double getPressureCoeff()
         void setAddedMass(double)
         double getAddedMass()
         void setOpenTiming(double)
         double getOpenTiming()
         void setCloseTiming(double)
         double getCloseTiming()

cdef class Cylinder(IdealGasReactor):
     pass

cdef class PistonBase(WallBase):
     cdef CxxPistonBase* piston

cdef class Solenoid(FlowDevice):
     pass


