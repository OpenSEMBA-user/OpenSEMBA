// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_
#define SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_

#include <cassert>
#include <exception>
#include <vector>

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class Multilayer : public virtual Surface {
public:
    class Layer {
    public:
        Layer(Math::Real thickness, Math::Real relPermittivity,
                Math::Real relPermeability, Math::Real elecCond) :
                thickness_(thickness), relPermittivity_(relPermittivity),
                relPermeability_(relPermeability), elecCond_(elecCond) { }

        Math::Real getThickness() const {return thickness_;}
        Math::Real getRelPermittivity() const {return relPermittivity_;}
        Math::Real getPermittivity() const {
            return relPermittivity_ * Math::Constants::eps0;
        }
        Math::Real getRelPermeability() const {return relPermeability_;}
        Math::Real getPermeability() const {
            return relPermeability_ * Math::Constants::mu0;
        }
        Math::Real getElecCond() const {return elecCond_;}
    private:
        Math::Real thickness_;
        Math::Real relPermittivity_;
        Math::Real relPermeability_;
        Math::Real elecCond_;
    };

    class FittingOptions {
    public:
        FittingOptions(
                std::pair<Math::Real, Math::Real> minMaxFreq,
                size_t numberOfPoles ) :
                    minMaxFreq_(minMaxFreq), numberOfPoles_(numberOfPoles) { }

        std::pair<Math::Real, Math::Real> getMinMaxFreq() const {
            return minMaxFreq_;
        }

        std::size_t getNumberOfPoles() const {
            return numberOfPoles_;
        }

    private:
        std::pair<Math::Real, Math::Real> minMaxFreq_;
        std::size_t numberOfPoles_;
    };

    Multilayer(const Id id,
            const std::string& name,
            const std::vector<Layer>& layers,
            const std::vector<FittingOptions>& options = {});
    virtual ~Multilayer();

    SEMBA_CLASS_DEFINE_CLONE(Multilayer);

    Layer getLayer(size_t i) const {
        return layers_[i];
    }
    std::size_t getNumberOfLayers() const {
        return layers_.size();
    }

    Math::Real Multilayer::getThickness   (const std::size_t i) const;
    Math::Real Multilayer::getPermittivity(const std::size_t i) const;
    Math::Real Multilayer::getPermeability(const std::size_t i) const;
    Math::Real Multilayer::getElecCond    (const std::size_t i) const;
    Math::Real Multilayer::getMagnCond    (const std::size_t i) const;

    bool hasFittingOptions() const;
    FittingOptions getFittingOptions() const;

    std::string printLayer(const std::size_t i) const;
    void printInfo() const;

private:
    std::vector<Layer> layers_;

    std::vector<FittingOptions> options_; // optional (0 or 1 size).
};

namespace Error {
namespace SurfaceMultilayer {

class IncompatibleSizes : public std::exception {
public:
    IncompatibleSizes() {}
    virtual ~IncompatibleSizes() throw() {}

    const char* what() const throw() {
        return "SurfaceMultilayer: "
                "Incompatible sizes of layers parameters.";
    }
};

} /* namespace SurfaceMultilayer */
} /* namespace Error */
} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_ */
