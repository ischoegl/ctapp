//! @file Solenoid.cpp

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#include "cantera/zeroD/Piston.h"
#include "cantera/zeroD/Solenoid.h"

namespace Cantera
{

void Solenoid::setMaster(WallBase* master)
{
    m_master = master;
    if ( (master) && (master->type() == ReciprocatingPistonType) ) {
        m_master = master;
	m_cycle = ((ReciprocatingPiston*)m_master)->cycle();
	m_duration = calcOpenDuration();
    }
}


double Solenoid::timer(double t)
{
    if ( hasCycle() ) {
        return ((ReciprocatingPiston*)m_master)->crankAngle(t);
    } else {
        return t;
    }
}

bool Solenoid::isOpen(double curr_timer)
{
    double ca = curr_timer;
    if ( m_open < m_close ) {
        if (ca > m_close) ca -= m_cycle;
	else if (ca + m_cycle <= m_close) ca += m_cycle;
	return (ca >= m_open) && (ca < m_close);
    } else {
        if (ca > m_open) ca -= m_cycle;
	else if (ca + m_cycle <= m_open) ca += m_cycle;
	return !( (ca >= m_close) && (ca < m_open) );
    }
}

void Solenoid::updateMassFlowRate(double time)
{
    if (!ready()) {
        throw CanteraError("Valve::updateMassFlowRate",
			   "Device is not ready; some parameters have not been set.");
    }
    m_mdot = 0.;
    if ( isOpen(timer(time)) ) {
        if ( (m_mass>0.) && (m_duration>0.) ) {
	    m_mdot += m_mass / m_duration;
	}
	if ( hasCycle() ) {
	    m_mdot *= ((ReciprocatingPiston*)m_master)->getOmega();
	}
	if (m_coeffs.size() == 1) {
	    double delta_P = in().pressure() - out().pressure();
	    m_mdot += m_coeffs[0]*delta_P;
	}
    }
    m_mdot = std::max(m_mdot, 0.0);
}

double Solenoid::calcOpenDuration()
{
    if (m_open < m_close) {
        return m_close - m_open;
    } else {
        return m_close + m_cycle - m_open;
    }
}

}
