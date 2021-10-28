//! @file NewFlow.cpp

// the code below builds on StFlow.cpp (Cantera 2.6.1)

#include "NewFlow.h"

using namespace std;

using namespace Cantera;

namespace CanteraApp {

NewFlow::NewFlow(IdealGasPhase *ph, size_t nsp, size_t nextra, size_t points)
    : StFlow(ph, nsp, points), m_nextra(nextra)
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
    size_t idx;
    idx = StFlow::componentIndex(name);
    if (idx == npos) {
        for (size_t n = c_offset_Y + m_nsp; n < m_nv; n++) {
            if (componentName(n) == name) {
                return n;
            }
        }
        return npos;
    } else {
        return idx;
    }
}

} // namespace CanteraApp
