

#pragma once

#include "argument/Object.h"

namespace SEMBA {
namespace Solver {

typedef Argument::Object Settings;

class Info {
public:
    Info() {}
    Info(const std::string& name) {
        solverName_ = name;
    }
    Info(const std::string& name, const Settings& sett) {
        solverName_ = name;
        solverSett_ = sett;
    }
    Info(const Info& rhs) {
        solverName_ = rhs.solverName_;
        solverSett_ = rhs.solverSett_;
    }
    Info(Info&& rhs) {
        solverName_ = std::move(rhs.solverName_);
        solverSett_ = std::move(rhs.solverSett_);
    }
    virtual ~Info() {}

    const std::string& getName    () const { return solverName_; }
    const Settings&    getSettings() const { return solverSett_; }

    void setName    (const std::string& name) { solverName_ = name; }
    void setSettings(const Settings&    sett) { solverSett_ = sett; }

    Info operator+ (const Settings& opts) {
        return Info(getName(), getSettings() + opts);
    }

    void printInfo() const {
        std::cout << "--- Solver Data ---" << std::endl;
        std::cout << "Solver name: " << solverName_ << std::endl;
        std::cout << "Solver settings:" << std::endl;
        solverSett_.printInfo();
    }

private:
    std::string solverName_;
    Settings    solverSett_;
};

} /* namespace Solver */
} /* namespace SEMBA */

