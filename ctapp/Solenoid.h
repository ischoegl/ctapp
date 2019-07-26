//! @file Solenoid.h

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_SOLENOID_H
#define CT_SOLENOID_H

#include "cantera/zeroD/FlowDevice.h"
#include "cantera/zeroD/Wall.h"

using namespace Cantera;

namespace CanteraApp {

const int SolenoidType = 4;

class Cantera::Wall;

//! Supply a mass flow rate that is a function of the pressure drop across the
//! valve.
/*!
 * The default behavior is a linearly proportional to the pressure difference.
 * Note that real valves do not have this behavior, so this class does not
 * model real, physical valves.
 */
class Solenoid : public FlowDevice
{
public:
    Solenoid() : FlowDevice(),
                 m_open(0.), m_close(1.), m_duration(-1.),
                 m_mass(0.), m_cycle(0.), m_master(0) {
        m_type = SolenoidType;
    }

    virtual bool ready() {
        return FlowDevice::ready() && (m_coeffs.size() == 1 || m_mass>0.);
    }

    //! Set pressure coefficient [kg/s/Pa]
    virtual void setPressureCoeff(double c) { m_coeffs = {c}; }

    //! Get pressure coefficient [kg/s/Pa]
    virtual double getPressureCoeff() const { return m_coeffs[0]; }

    //! Set added mass [kg]
    virtual void setAddedMass(double m) { m_mass = m; }

    //! Get added mass [kg]
    virtual double getAddedMass() const { return m_mass; }

    //! Set object solenoid uses for synchronization
    virtual void setMaster(Wall* master);

    //! Specify whether solenoid is synchronized
    virtual bool hasCycle() const {  return m_cycle>0.; }

    //! Get timing value
    virtual double timer(double t);

    //! Get valve state
    virtual bool isOpen(double curr_timer);

    /// Compute the currrent mass flow rate
    virtual void updateMassFlowRate(double time);

    //! Set solenoid open timing
    virtual void setOpenTiming(double open) {
        m_open = open;
        m_duration = calcOpenDuration();
    }

    //! Get solenoid open timing
    virtual double getOpenTiming() { return m_open; }

    //! Set solenoid close timing
    virtual void setCloseTiming(double close) {
        m_close = close;
        m_duration = calcOpenDuration();
    }

    //! Get solenoid close timing
    virtual double getCloseTiming() { return m_close; }

    //! Get solenoid open duration
    virtual double getOpenDuration() { return m_duration; }

protected:

    double calcOpenDuration();

    double m_open, m_close, m_duration, m_mass, m_cycle;
    Wall* m_master;
};

}
#endif
