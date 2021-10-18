

#include "Options.h"

namespace SEMBA {
namespace Solver {

Options::Options() {
//    solver = Solver::none;
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

void Options::addArguments(Argument::Group& args) const {

}

void Options::set(const Settings& opts) {
    if (opts.existsName(      "runSimulation")) {
        setRunSimulation(opts("runSimulation").getBool());
    }
    if (opts.existsName(  "finalTime")) {
        setFinalTime(opts("finalTime").getReal());
    }
    if (opts.existsName(          "numberOfTimeSteps")) {
        setNumberOfTimeSteps(opts("numberOfTimeSteps").getInt());
    }
    if (opts.existsName( "timeStep")) {
        setTimeStep(opts("timeStep").getReal());
    }
    if (opts.existsName("cfl")) {
        setCFL(opts(    "cfl").getReal());
    }
    if (opts.existsName(       "defaultSamplingPeriod")) {
        setSamplingPeriod(opts("defaultSamplingPeriod").getReal());
    }
    if (opts.existsName(        "forceRestarting")) {
        setForceRestarting(opts("forceRestarting").getBool());
    }
    if (opts.existsName(         "resumeSimulation")) {
        setResumeSimulation(opts("resumeSimulation").getBool());
    }
    if (opts.existsName("flush")) {
        setFlush(opts(  "flush").getReal());
    }
    if (opts.existsName(            "additionalArguments")) {
        setAdditionalArguments(opts("additionalArguments").getString());
    }
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
//    return solver;
//}
//
//void Options::setSolver(Options::Solver solver) {
//    solver = solver;
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
