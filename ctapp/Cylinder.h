//! @file Cylinder.h

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_CYLINDER_H
#define CT_CYLINDER_H

#include "cantera/zeroD/ReactorBase.h"
#include "cantera/zeroD/Piston.h"

namespace Cantera
{

/**
 * Class Cylinder is derived from class IdealGasReactor and adds utility functions
 * that facilitate calculations for IC engines and rapid compression machines.
 * Utility functions pass geometry information between cylinder and an attached
 * piston object.
 */
class Cylinder : public IdealGasReactor
{
public:
    Cylinder(): IdealGasReactor(), m_piston(0) {}

    virtual int type() const {
        return CylinderType;
    }

    //! Install piston
    void install(PistonBase& piston) {
        m_piston = &piston;
        if (!m_piston->check()) {
            throw CanteraError("Cylinder::install",
                               "Piston is not set up correctly: Cylinder needs to be on left.");
        }
        m_vol = m_piston->position(0.) * m_piston->area();
    }

    virtual void initialize(double t0 = 0.0) {
        if (m_piston) {
            IdealGasReactor::initialize(t0);
            if (m_piston->type()!=FloatingPistonType) {
                m_vol = m_piston->position(t0) * m_piston->area();
            }
        } else {
            throw CanteraError("Cylinder::initialize",
                               "Function requires attached piston.");
        }
    }

    //! Set cylinder bore diameter [m]
    virtual void setBoreDiameter(double bore) {
        if (m_piston) {
            double disp = getDisplacement();
            double cr = getCompressionRatio();
            m_piston->setArea(.25 * pi * bore * bore);
            double stroke = disp / m_piston->area();
            m_piston->setZMin(stroke / (cr - 1.) );
            m_piston->setZMax(stroke * cr / (cr - 1.) );
            m_vol = m_piston->position(0.) * m_piston->area();
        } else {
            throw CanteraError("Cylinder::setBoreDiameter",
                               "Function requires attached piston.");
        }
    }

    //! Get cylinder bore diameter [m]
    virtual double getBoreDiameter() const {
        if (m_piston) {
            return sqrt(4. * m_piston->area() / pi);
        } else {
            throw CanteraError("Cylinder::getBoreDiameter",
                               "Function requires attached piston.");
        }
    }

    //! Set volume profile [-]
    virtual void setProfile(std::vector<double>& vprofile, std::vector<double>& tprofile) {
        if (!m_piston) {
            throw CanteraError("Cylinder::setProfile",
                               "Function requires attached piston.");
        } else if (m_piston->type()!=DrivenPistonType) {
            throw CanteraError("Cylinder::setProfile",
                               "Only implemented for DrivenPiston.");
        } else {
            DrivenPiston* p = (DrivenPiston*)m_piston;
            p->setProfile(vprofile, tprofile);
        }
    }

    //! Set compression ratio [-]
    virtual void setCompressionRatio(double cr) {
        if (!m_piston) {
            throw CanteraError("Cylinder::setCompressionRatio",
                               "Function requires attached piston.");
        } else if (m_piston->type()!=ReciprocatingPistonType) {
            throw CanteraError("Cylinder::setCompressionRatio",
                               "Only implemented for ReciprocatingPiston.");
        } else if (cr<=1.) {
            throw CanteraError("Cylinder::setCompressionRatio",
                               "Invalid compression ratio.");
        } else {
            double stroke = m_piston->getStrokeLength();
            m_piston->setZMin(stroke / (cr - 1.) );
            m_piston->setZMax(stroke * cr / (cr - 1.) );
            m_vol = m_piston->position(0.) * m_piston->area();
        }
    }

    //! Get compression ratio [-]
    virtual double getCompressionRatio() const {
        if (m_piston) {
            return m_piston->getZMax() / m_piston->getZMin();
        } else {
            throw CanteraError("Cylinder::getCompressionRatio",
                               "Function requires attached piston.");
        }
    }

    //! Set cylinder displacement [m^3]
    virtual void setDisplacement(double vol) {
        if (!m_piston) {
            throw CanteraError("Cylinder::setDisplacement",
                               "Function requires attached piston.");
        } else if (m_piston->type()!=ReciprocatingPistonType) {
            throw CanteraError("Cylinder::setDisplacement",
                               "Only implemented for ReciprocatingPiston.");
        } else if (vol<=0.) {
            throw CanteraError("Cylinder::setDisplacement",
                               "Invalid displacement.");
        } else {
            double cr = getCompressionRatio();
            double stroke = vol / m_piston->area();
            m_piston->setZMin(stroke / (cr - 1.) );
            m_piston->setZMax(stroke * cr / (cr - 1.) );
            m_vol = m_piston->position(0.) * m_piston->area();
        }
    }

    //! Get cylinder displacement [m^3]
    virtual double getDisplacement() const {
        if (m_piston) {
            return m_piston->getStrokeLength() * m_piston->area();
        } else {
            throw CanteraError("Cylinder::getDisplacement",
                               "Function requires attached piston.");
        }
    }

    //! Get minimum volume [m^3]
    virtual double getVMin() const {
        if (m_piston) {
            return m_piston->getZMin() * m_piston->area();
        } else {
            throw CanteraError("Cylinder::getVMin",
                               "Function requires attached piston.");
        }
    }

    //! Get maximum volume [m^3]
    virtual double getVMax() const {
        if (m_piston) {
          return m_piston->getZMax() * m_piston->area();
        } else {
            throw CanteraError("Cylinder::getVMax",
                               "Function requires attached piston.");
        }
    }

protected:

    PistonBase* m_piston;
};
}

#endif
