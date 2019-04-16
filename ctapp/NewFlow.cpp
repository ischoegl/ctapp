//! @file NewFlow.cpp

// the code below builds on StFlow.cpp (Cantera 2.5.0a2)

#include "NewFlow.h"

using namespace std;

using namespace Cantera;

namespace CanteraApp {

NewFlow::NewFlow(IdealGasPhase *ph, size_t nsp, size_t nextra, size_t points)
    : StFlow(ph, nsp, points), m_nextra(nextra) {

  // update number of equations and resize state vectors
  m_nv += m_nextra;
  Domain1D::resize(m_nv, points);
}

void NewFlow::evalResidual(double *x, double *rsd, int *diag, double rdt,
                           size_t jmin, size_t jmax) {
  
  // main equations are not affected
  StFlow::evalResidual(x, rsd, diag, rdt, jmin, jmax);

  // keep the remaining entries at zero
  // (this class serves for illustration purposes only)
  for (size_t j = jmin; j <= jmax; j++) {

    // loop over extra equations
    for (size_t n = m_nsp + c_offset_Y; n < m_nv; n++) {

      // keep the usual structure (if/else *could* be simplified)
      if (j == 0) {
        // left boundary
        rsd[index(n, j)] = x[index(n, j)];
      } else if (j == m_points - 1) {
        // right boundary
        rsd[index(n, j)] = x[index(n, j)];
      } else {
        // interior point
        rsd[index(n, j)] = x[index(n, j)];
        diag[index(n, j)] = 1;
      }
    }
  }
}

} // namespace CanteraApp
