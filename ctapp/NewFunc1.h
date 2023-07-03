//! @file NewFunc1.h

// This file is part of ctapp. See LICENSE file in the top-level directory
// for license and copyright information.

#ifndef CT_NEWFUNC1_H
#define CT_NEWFUNC1_H

#include "cantera/numerics/Func1.h"
#include "cantera/numerics/Func1Factory.h"

using namespace Cantera;

namespace CanteraApp {


//! Implements the \c sqrt() function.
/*!
 * The functor class with type \c "sqrt" returns \f$ f(x) = \sqrt(a x) \f$.
 * @param a  Factor (default=1.0)
 */
class Sqrt1 : public Func1
{
public:
    Sqrt1(double a=1.0) {
        m_c = a;
    }

    //! Constructor uses single parameter (factor)
    Sqrt1(const vector<double>& params);

    virtual string type() const {
        return "sqrt";
    }

    virtual double eval(double t) const {
        return sqrt(m_c * t);
    }

    virtual string write(const string& arg) const;
};


inline void registerFunctors() {
    Func1Factory::factory()->reg("sqrt", [](const vector<double>& params) {
        return new Sqrt1(params);
    });
}

} // namespace CanteraApp

#endif
