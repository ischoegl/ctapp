//! @file NewFlow.cpp

// the code below builds on StFlow.cpp (Cantera 2.5.0a2)

#include "NewFlow.h"
// #include "cantera/base/ctml.h"
// #include "cantera/transport/TransportBase.h"
// #include "cantera/numerics/funcs.h"

using namespace std;

using namespace Cantera;

namespace CanteraApp {

NewFlow::NewFlow(IdealGasPhase *ph, size_t nsp, size_t points)
    : StFlow(ph, nsp, points) {

  m_nv += 3;
  Domain1D::resize(m_nv, points);
}

} // namespace
