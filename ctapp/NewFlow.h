//! @file NewFlow.h

// This file is part of ctapp. See LICENSE file in the top-level directory
// for license and copyright information.

// the code extends StFlow.cpp (Cantera 3.0.0a5)

#ifndef CT_NEWFLOW_H
#define CT_NEWFLOW_H

#include "cantera/oneD/StFlow.h"
#include "cantera/oneD/DomainFactory.h"

using namespace Cantera;

namespace CanteraApp {

/** This class inherits from StFlow but adds extra equations at the end. */
class NewFlow : public StFlow {
public:

    //! Create a new flow domain.
    //! @param sol  Solution object used to evaluate all thermodynamic, kinetic, and
    //!     transport properties
    //! @param id  name of flow domain
    //! @param points  initial number of grid points
    NewFlow(shared_ptr<Solution> sol, const std::string& id="", size_t points=1);

    virtual std::string type() const { return "new-flow"; }

    virtual std::string componentName(size_t n) const;
    virtual size_t componentIndex(const std::string &name) const;

protected:
    //! Evaluate the residual function. This function is called in eval
    //! after updateProperties is called.
    virtual void evalResidual(double *x, double *rsd, int *diag, double rdt,
                              size_t jmin, size_t jmax);

    size_t m_nextra = 1;
};

inline void registerDomains() {
    DomainFactory::factory()->reg("new-flow",
        [](shared_ptr<Solution> solution, const string& id) {
            StFlow* ret = new NewFlow(solution, id);
            ret->setUnstrainedFlow();
            return ret;
        });
}

} // namespace CanteraApp

#endif
