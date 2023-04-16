//! @file ClonedReactor.h

// This file is part of ctapp. See LICENSE file in the top-level directory
// for license and copyright information.

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
