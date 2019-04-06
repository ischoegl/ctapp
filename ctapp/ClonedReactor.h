//! @file ClonedReactor.h

// This file is part of the add-on package ctapp.
// See License.txt in the top-level directory

#ifndef CT_CLONEDREACTOR_H
#define CT_CLONEDREACTOR_H

#include "cantera/zeroD/IdealGasConstPressureReactor.h"
#include "cantera/zeroD/ReactorFactory.h"

using namespace Cantera;

namespace CanteraApp {

const int ClonedReactorType = 7;

class ClonedReactor : public IdealGasConstPressureReactor {
  virtual int type() const { return ClonedReactorType; }
  // inherit everything else
};

inline void registerClonedReactor() {
  ReactorFactory::factory()->reg("ClonedReactor",
                                 []() { return new ClonedReactor(); });
}

} // namespace CanteraApp

#endif
