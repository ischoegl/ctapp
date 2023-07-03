//! @file NewFunc1.cpp

// This file is part of ctapp. See LICENSE file in the top-level directory
// for license and copyright information.

#include "NewFunc1.h"
#include "cantera/base/ctexceptions.h"

using namespace std;

using namespace Cantera;

namespace CanteraApp {

Sqrt1::Sqrt1(const vector<double>& params)
{
    if (params.size() != 1) {
        throw CanteraError("Sqrt1::Sqrt1",
            "Constructor needs exactly one parameter (factor).");
    }
    m_c = params[0];
}

string Sqrt1::write(const string& arg) const
{
    if (m_c == 1.0) {
        return fmt::format("\\sqrt({})", arg);
    }
    return fmt::format("\\sqrt({}{})", m_c, arg);
}

} // namespace CanteraApp
