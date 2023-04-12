//! @file NewFlow.h

// the code below builds on StFlow.cpp (Cantera 2.6.1)

#ifndef CT_NEWFLOW_H
#define CT_NEWFLOW_H

#include "cantera/oneD/StFlow.h"
#include "cantera/oneD/DomainFactory.h"

using namespace Cantera;

namespace CanteraApp {

class NewFlow : public StFlow {
public:
    /** This class adds extra equations at the end
     */

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
            ret->setFreeFlow();
            return ret;
        });
}

} // namespace CanteraApp

#endif
