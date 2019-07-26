//! @file Piston.cpp

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#include "Piston.h"
#include "cantera/zeroD/Reactor.h"

using namespace Cantera;

namespace CanteraApp {

double PistonBase::position()
{
    if (ready()) {
        Cylinder* cyl = (Cylinder*) m_left;
        return cyl->volume() / m_area;
    } else {
        throw CanteraError("PistonBase::position",
                           "Not ready.");
    }
}

double FloatingPiston::position(double t)
{
    return PistonBase::position();
}

double FloatingPiston::velocity(double t)
{
    double z = position(t);
    double v = m_k * (m_left->pressure() - m_right->pressure());

    if ( z <= m_zmin && v<0. ) {
        v *= 0.;
    } else if (z >= m_zmax && v>0.) {
        v *= 0.;
    }
    return v;
}

void DrivenPiston::setStrokeLength(double s)
{
    double vmin = *min_element(m_vprof.begin(), m_vprof.end());
    double vmax = *max_element(m_vprof.begin(), m_vprof.end());

    m_area = (vmax - vmin) / s;
    m_zmin = vmin / m_area;
    m_zmax = vmax / m_area;
}

void DrivenPiston::setProfile(std::vector<double>& vprofile, std::vector<double>& tprofile)
{
    m_vprof = vprofile;
    m_tprof = tprofile;
    m_zmin = *min_element(m_vprof.begin(), m_vprof.end()) / m_area;
    m_zmax = *max_element(m_vprof.begin(), m_vprof.end()) / m_area;
    m_ix = 0;
}

double DrivenPiston::position(const double t)
{
    int siz = m_tprof.size();
    if (siz>1) {
        if (t <= m_tprof[0]) {
            return m_vprof[0];
        } else if (t >= m_tprof[siz-1]) {
            return m_vprof[siz-1];
        } else {
            // linearInterp(zz, m_zfix, m_tfix);
            if (t<m_tprof[m_ix]) m_ix = 0;
            while (t >= m_tprof[m_ix+1]) ++m_ix;
            double dz = (m_vprof[m_ix+1] - m_vprof[m_ix]);
            dz /= (m_tprof[m_ix+1] - m_tprof[m_ix]) * m_area;
            dz *= t - m_tprof[m_ix];
            return m_vprof[m_ix] + dz;
        }
    } else {
        return 0.;
    }
}

double DrivenPiston::velocity(const double t)
{
    int siz = m_tprof.size();
    if (siz>1 && t >= m_tprof[0] && t < m_tprof[siz-1]) {
        if (t<m_tprof[m_ix]) m_ix = 0;
        while (t >= m_tprof[m_ix+1]) ++m_ix;
        double dz = m_vprof[m_ix+1] - m_vprof[m_ix];
        dz /= (m_tprof[m_ix+1] - m_tprof[m_ix]) * m_area;
        return dz;
    } else {
        return 0.;
    }
}

double ReciprocatingPiston::crankAngle(const double t)
{
    double cum = 2. / (double)m_strokes * m_freq * t;
    int cyc = (int)cum;
    return cycle() * (cum - (double)cyc);
}

double ReciprocatingPiston::position(const double t)
{
    double a = crankAngle(t);
    double r_sin_a = m_r * sin(a);
    double dz = m_r*cos(a);
    if (m_l > m_r) {
        dz += sqrt(m_l*m_l - r_sin_a*r_sin_a);
    }
    return m_zmin + m_r - dz;
}

double ReciprocatingPiston::velocity(const double t)
{
    double a = crankAngle(t);
    double r_sin_a = m_r * sin(a);
    double dxda = 1.;

    // actual crank mechanism requires properly set rod length
    if (m_l > m_r) {
        dxda += m_r*cos(a) / sqrt(m_l*m_l - r_sin_a*r_sin_a);
    }
    dxda *= r_sin_a;

    return 2. * pi * m_freq * dxda;
}

}
