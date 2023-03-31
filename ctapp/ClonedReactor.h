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
public:
    virtual std::string type() const { return "cloned-reactor"; }
    // inherit everything else
};

inline void registerReactors() {
    ReactorFactory::factory()->reg("cloned-reactor", []() {
        return new ClonedReactor();
    });
}

} // namespace CanteraApp

#endif
