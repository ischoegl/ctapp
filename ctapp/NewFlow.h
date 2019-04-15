//! @file NewFlow.h

// the code below builds on StFlow.h (Cantera 2.5.0a2)

#ifndef CT_NEWFLOW_H
#define CT_NEWFLOW_H

#include "cantera/oneD/StFlow.h"

using namespace Cantera;

namespace CanteraApp {

//------------------------------------------
//   constants
//------------------------------------------

class NewFlow : public StFlow
{
public:
    //--------------------------------
    // construction and destruction
    //--------------------------------

    //! Create a new flow domain.
    //! @param ph Object representing the gas phase. This object will be used
    //!     to evaluate all thermodynamic, kinetic, and transport properties.
    //! @param nsp Number of species.
    //! @param points Initial number of grid points
    NewFlow(IdealGasPhase* ph = 0, size_t nsp = 1, size_t points = 1);

};

}

#endif
