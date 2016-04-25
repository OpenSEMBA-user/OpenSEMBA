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

#include "Options.h"

namespace SEMBA {
namespace Solver {

Options::Options() {
//    solver_ = Solver::none;
    runSimulation_ = true;
    endingCondition_ = EndingCondition::finalTime;
    finalTime_ = 0.0;
    samplingPeriod_ = 0.0;
    timeStep_ = 0.0;
    cfl_ = 0.8;
    forceRestarting_ = false;
    resumeSimulation_ = false;
    flush_ = 0.0;
}

Options::~Options() {}

void Options::addArguments(Argument::Group& args) const {

}

void Options::set(const Settings& opts) {
    if (opts.existsName("Run simulation")) {
        setRunSimulation(opts("Run simulation").getBool());
    }
    if (opts.existsName("Final time")) {
        setFinalTime(opts("Final time").getReal());
    }
    if (opts.existsName("Number of time steps")) {
        setNumberOfTimeSteps(opts("Number of time steps").getInt());
    }
    if (opts.existsName("Time step")) {
        setTimeStep(opts("Time step").getReal());
    }
    if (opts.existsName("CFL")) {
        setCFL(opts("CFL").getReal());
    }
    if (opts.existsName("Default sampling period")) {
        setSamplingPeriod(opts("Default sampling period").getReal());
    }
    if (opts.existsName("Force restarting")) {
        setForceRestarting(opts("Force restarting").getBool());
    }
    if (opts.existsName("Resume simulation")) {
        setResumeSimulation(opts("Resume simulation").getBool());
    }
    if (opts.existsName("Flush")) {
        setFlush(opts("Flush").getReal());
    }
    if (opts.existsName("Additional arguments")) {
        setAdditionalArguments(opts("Additional arguments").getString());
    }
}

void Options::printHelp() const {
    // TODO Options printHelp
}

Math::Real Options::getFinalTime() const {
    if (endingCondition_ == EndingCondition::finalTime) {
        return finalTime_;
    } else {
        return finalTime_ * timeStep_;
    }
}

void Options::setFinalTime(Math::Real finalTime) {
    endingCondition_ = EndingCondition::finalTime;
    finalTime_ = finalTime;
}

Math::Real Options::getSamplingPeriod() const {
    return samplingPeriod_;
}

void Options::setSamplingPeriod(Math::Real samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

Math::Real Options::getTimeStep() const {
    return timeStep_;
}

void Options::setTimeStep(Math::Real timeStep) {
    timeStep_ = timeStep;
}

double Options::getCFL() const {
    return cfl_;
}

void Options::setCFL(double cfl) {
    cfl_ = cfl;
}

std::size_t Options::getNumberOfTimeSteps() const {
    if (endingCondition_ == EndingCondition::finalTime) {
        if (timeStep_ != 0.0) {
            return ceil(finalTime_ / timeStep_);
        } else {
            return (std::size_t)0;
        }
    } else {
        return (std::size_t)finalTime_;
    }
}

void Options::setNumberOfTimeSteps(std::size_t numberOfTimeSteps) {
    endingCondition_ = EndingCondition::numberOfTimeSteps;
    finalTime_ = (Math::Real)numberOfTimeSteps;
}

void Options::printInfo() const {
    std::cout << " --- Solver parameters --- " << std::endl;
//    std::cout << "Solver:                  " << toStr(solver_) << std::endl;

    std::cout << "Run simulation:          " << isRunSimulation() << std::endl;

    std::cout << "Final time:              " << getFinalTime() << std::endl;
    std::cout << "Time step:               " << getTimeStep() << std::endl;
    std::cout << "CFL:                     " << getCFL() << std::endl;

    std::cout << "Default sampling period: " << getSamplingPeriod() << std::endl;
    std::cout << "Force restarting:        " << isForceRestarting() << std::endl;
    std::cout << "Resume simulation:       " << isResumeSimulation() << std::endl;
    std::cout << "Flush:                   " << getFlush() << std::endl;
    std::cout << "Additional arguments:    " << getAdditionalArguments() << std::endl;
}

const std::string& Options::getAdditionalArguments() const {
    return additionalArguments_;
}

void Options::setAdditionalArguments(const std::string& additionalArguments) {
    additionalArguments_ = additionalArguments;
}

Math::Real Options::getFlush() const {
    return flush_;
}

void Options::setFlush(Math::Real flush) {
    flush_ = flush;
}

bool Options::isForceRestarting() const {
    return forceRestarting_;
}

void Options::setForceRestarting(bool forceRestarting) {
    forceRestarting_ = forceRestarting;
}

bool Options::isResumeSimulation() const {
    return resumeSimulation_;
}

void Options::setResumeSimulation(bool resumeSimulation) {
    resumeSimulation_ = resumeSimulation;
}

//std::string Options::toStr(const Options::Solver& solver) {
//    switch (solver) {
//    case Solver::cudg3d:
//        return std::string("cudg3d");
//    case Solver::ugrfdtd:
//        return std::string("ugrfdtd");
//    case Solver::none:
//    default:
//        return std::string("none");
//    }
//}

std::string Options::toArgsStr() const {
    Options defaultOptions;
    std::stringstream ss;
    if (getCFL() != defaultOptions.getCFL()) {
        ss << " -cfl " << getCFL();
    }
    if (isResumeSimulation()) {
        if (getTimeStep() != 0.0) {
            ss << " -r " << (std::size_t)floor(getFinalTime() / getTimeStep());
        } else {
            ss << " -r " << (std::size_t)floor(getFinalTime());
        }
    }
    if (isForceRestarting()) {
        ss << " -s";
    }
    if (getFlush() != defaultOptions.getFlush()) {
        ss << " -flush " << getFlush();
    }
    return ss.str();
}

//Options::Solver Options::getSolver() const {
//    return solver_;
//}
//
//void Options::setSolver(Options::Solver solver) {
//    solver_ = solver;
//}

bool Options::isRunSimulation() const {
    return runSimulation_;
}

std::string Options::toStr() const {
    std::stringstream ss;
//    ss << "Solver:                        " << toStr(getSolver()) << std::endl;
    ss << "Run simulation:                " << isRunSimulation() << std::endl;
    ss << "Ending:                        " << getFinalTime() << std::endl;
    ss << "Time step:                     " << getTimeStep() << std::endl;
    ss << "Cfl:                           " << getCFL() << std::endl;
    ss << "Sampling period:               "
            << getSamplingPeriod() << std::endl;
    ss << "Force restarting:              "
            << isForceRestarting() << std::endl;
    ss << "Resume simulation:             "
            << isResumeSimulation() << std::endl;
    ss << "Flush:                         " << getFlush() << std::endl;
    ss << "Additional arguments:          " << getAdditionalArguments();
    return ss.str();
}

void Options::setRunSimulation(bool runSimulation) {
    runSimulation_ = runSimulation;
}

}
}
