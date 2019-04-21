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

class NewFlow : public StFlow {
public:
  /** This class adds extra equations at the end 
   */

  //! Create a new flow domain.
  //! @param ph Object representing the gas phase. This object will be used
  //!     to evaluate all thermodynamic, kinetic, and transport properties.
  //! @param nsp Number of species.
  //! @param nextra number of extra equations
  //! @param points Initial number of grid points
  NewFlow(IdealGasPhase *ph = 0, size_t nsp = 1, size_t nextra = 0,
          size_t points = 1);

  //! Evaluate the residual function. This function is called in eval
  //! after updateProperties is called.
  virtual void evalResidual(double *x, double *rsd, int *diag, double rdt,
                            size_t jmin, size_t jmax);

  virtual std::string componentName(size_t n) const;
  virtual size_t componentIndex(const std::string &name) const;

protected:
  size_t m_nextra;
};

} // namespace CanteraApp

#endif
