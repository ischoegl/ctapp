//! @file Piston.h Header file for Piston classes.

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_PISTON_H
#define CT_PISTON_H

#include "cantera/zeroD/Wall.h"

using namespace Cantera;

namespace CanteraApp {

class Cylinder;

const int FloatingPistonType = 2;
const int DrivenPistonType = 3;
const int ReciprocatingPistonType = 4;


//! Represents a wall between between two ReactorBase objects.
/*!
 * PistonBase is a base class that defines additional functions.
 */
class PistonBase : public Wall
{
public:

    PistonBase() : Wall(), m_zmin(1.e-14), m_zmax(1e14) {}

    virtual bool ready() {
        return ( Wall::ready() && check() );
    }

    //! check whether reactors are installed correctly
    virtual bool check() {
        return ( m_left->type()==CylinderType && m_right->type()!=CylinderType);
    }

    //! Set minimum position [m]
    virtual void setZMin(double z) { m_zmin = z; }

    //! Get minimum position [m]
    double getZMin() const { return m_zmin; }

    //! Set maximum position [m]
    virtual void setZMax(double z) { m_zmax = z; }

    //! Get maximum position [m]
    double getZMax() const { return m_zmax; }

    //! Get stroke length [m]
    double getStrokeLength() const { return m_zmax - m_zmin; }

    //! Position from integrator
    double position();

    //! Calculate piston position [m]
    virtual double position(double t) {
        throw CanteraError("PistonBase::position",
                           "Function needs to be overloaded.");
    }

    //! Calculate piston velocity [m/s]
    virtual double velocity(double t) {
        throw CanteraError("PistonBase::velocity",
                           "Function needs to be overloaded.");
    }

    virtual double vdot(double t) { return m_area * velocity(t); }

protected:

    // Piston position at smallest volume [m]
    double m_zmin;

    // Piston position at largest volume [m]
    double m_zmax;
};


//! Represents a wall between between two ReactorBase objects.
/*!
 * FloatingPiston is equivalent to Wall, minus the ability to specify
 * velocities and heat flux using external functions.
 */
class FloatingPiston : public PistonBase
{
public:
    FloatingPiston() : PistonBase() {
        m_type = FloatingPistonType;
    }

    virtual double position(double t);

    virtual double velocity(double t);
};


//! Represents a wall between between two ReactorBase objects.
/*!
 * The DrivenPiston class captures the movement of a wall with a
 * prescribed profile of piston locations
 * (e.g. rapid compression machine).
 */
class DrivenPiston : public PistonBase
{
public:
    DrivenPiston() : PistonBase() {
        m_type = DrivenPistonType;
    }

    virtual void setZMin(double z) {
        throw CanteraError("DrivenPiston::setZMin", "Not implemented.");
    }
    virtual void setZMax(double z) {
        throw CanteraError("DrivenPiston::setZMax", "Not implemented.");
    }

    //! Set piston stroke [m]
    void setStrokeLength(double s);

    virtual double position(const double t);

    virtual double velocity(const double t);

protected:

    friend class Cylinder;

    /// set volume profile (set from Cylinder)
    void setProfile(std::vector<double>& zprofile, std::vector<double>& tprofile);

    // Index: avoids looping over vector
    size_t m_ix;

    // Vector containing volume timing [s]
    std::vector<double> m_tprof;

    // Vector containing volume profile [m^3]
    std::vector<double> m_vprof;
};


//! Represents a wall between between two ReactorBase objects.
/*!
 * A reciprocating piston captures the motion of a piston in an
 * internal combustion engine using C++ code.
 */
class ReciprocatingPiston : public PistonBase
{
public:
    ReciprocatingPiston() : PistonBase(), m_freq(1.0),
                            m_r(1.0), m_l(0.0), m_strokes(4) {
        m_type = ReciprocatingPistonType;
        m_zmin = .1;
        m_zmax = 1.;
    }

    //! Set engine rpm.
    void setRPM(double rpm) { m_freq = rpm / 60.; }

    //! Get engine speed.
    double getRPM() const { return 60. * m_freq; }

    //! Set engine speed [rad/s]
    void setOmega(double freq) { m_freq = .5 * freq / pi; }

    //! Get engine speed [rad/s]
    double getOmega() const { return 2. * pi * m_freq; }

    //! Set minimum position [m]
    virtual void setZMin(double z) {
        m_zmin = z;
        m_r = .5 * (m_zmax - m_zmin);
    }

    //! Set maximum position [m]
    virtual void setZMax(double z) {
        m_zmax = z;
        m_r = .5 * (m_zmax - m_zmin);
    }

    //! Get ratio of connecting rod length to radius [m/m]
    double getCrankRadius() const { return m_r; }

    //! Set ratio of connecting rod length to radius [m/m]
    void setRodRadiusRatio(double rl) { m_l = rl * m_r; }

    //! Get ratio of connecting rod length to radius [m/m]
    double getRodRadiusRatio() const { return m_l / m_r; }

    //! Set number of strokes
    void setStrokeNumber(int n) { m_strokes = n; }

    //! Get number of strokes
    int getStrokeNumber() const { return m_strokes; }

    //! Cycle duration [rad]
    double cycle() const { return pi * (double)m_strokes; }

    //! Calculate crank position [rad]
    double crankAngle(const double t);

    virtual double position(const double t);

    virtual double velocity(const double t);

protected:

    // Frequency (revolutions per second) [1/s]
    double m_freq;

    // Crank radius [m]
    double m_r;

    // Connecting rod length [m]
    double m_l;

    // Number of strokes
    int m_strokes;
};

}
#endif
