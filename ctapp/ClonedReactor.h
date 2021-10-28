//! @file ClonedReactor.h

// This file is part of the add-on package ctapp.
// See License.txt in the top-level directory

// the code below builds on StFlow.cpp (Cantera 2.6.1)

#ifndef CT_CLONEDREACTOR_H
#define CT_CLONEDREACTOR_H

#include "cantera/zeroD/IdealGasConstPressureReactor.h"
#include "cantera/zeroD/ReactorFactory.h"

using namespace Cantera;

namespace CanteraApp {

class ClonedReactor : public IdealGasConstPressureReactor {
    virtual std::string type() const { return "ClonedReactor"; }
    // inherit everything else
};

inline void registerClonedReactor() {
      ReactorFactory::factory()->reg("ClonedReactor",
                                     []() { return new ClonedReactor(); });
}

} // namespace CanteraApp

#endif
