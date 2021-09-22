

#pragma once

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
                relPermeability_(relPermeability), elecCond_(elecCond) {
			if (relPermittivity_ == 0.0) {
				throw std::logic_error(
					"Layer relative permittivity must be greater than zero.");
			}
			if (relPermeability_ == 0.0) {
				throw std::logic_error(
					"Layer relative permeability must be greater than zero.");
			}
		}

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

    Math::Real getThickness   (const std::size_t i) const;
    Math::Real getPermittivity(const std::size_t i) const;
    Math::Real getPermeability(const std::size_t i) const;
    Math::Real getElecCond    (const std::size_t i) const;
    Math::Real getMagnCond    (const std::size_t i) const;

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

