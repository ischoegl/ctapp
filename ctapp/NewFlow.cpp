//! @file NewFlow.cpp

// This file is part of ctapp. See LICENSE file in the top-level directory
// for license and copyright information.

// the code extends StFlow.cpp (Cantera 3.0.0a5)

#include "NewFlow.h"

using namespace std;

using namespace Cantera;

namespace CanteraApp {

NewFlow::NewFlow(shared_ptr<Solution> sol, const std::string& id, size_t points)
    : StFlow(sol, id, points)
{
    // update number of equations and resize state vectors
    m_nv += m_nextra;
    Domain1D::resize(m_nv, points);

    // extra equation bounds
    for (size_t n = m_nsp + c_offset_Y; n < m_nv; n++) {
        setBounds(n, -1.e-7, 1.0e5);
    }
}

void NewFlow::evalResidual(double *x, double *rsd, int *diag, double rdt,
                           size_t jmin, size_t jmax)
{
    // main equations are not affected
    StFlow::evalResidual(x, rsd, diag, rdt, jmin, jmax);

    // create simple exponential decay for remaining components
    // (this class serves for illustration purposes only)
    for (size_t j = jmin; j <= jmax; j++) {

        // loop over extra equations
        for (size_t n = m_nsp + c_offset_Y; n < m_nv; n++) {

            // keep the usual structure (if/else *could* be simplified)
            if (j == 0) {
                // left boundary: force unity
                rsd[index(n, j)] = 1. - x[index(n, j)];
            } else if (j == m_points - 1) {
                // right boundary: zero gradient
                double dz = z(j) - z(j - 1);
                double df = x[index(n, j)] - x[index(n, j - 1)];
                rsd[index(n, j)] = df / dz;
            } else {
                // interior point: simple exponential decay
                double dz = z(j) - z(j - 1);
                double df = x[index(n, j)] - x[index(n, j - 1)];
                rsd[index(n, j)] = -x[index(n, j)] - 1./double(n) * df / dz;
                diag[index(n, j)] = false;
            }
        }
    }
}

string NewFlow::componentName(size_t n) const
{
    if (n < c_offset_Y + m_nsp) {
        return StFlow::componentName(n);
    } else if (n < m_nv) {
        return "xi" + to_string(n - (m_nv - m_nextra));
    } else {
        return "<unknown>";
    }
}

size_t NewFlow::componentIndex(const string &name) const
{
    for (size_t n = c_offset_Y + m_nsp; n < m_nv; n++) {
        if (componentName(n) == name) {
            return n;
        }
    }
    return StFlow::componentIndex(name);
}

} // namespace CanteraApp
