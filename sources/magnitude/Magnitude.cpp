/*
 * Magnitude.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: luis
 */

#include "../../sources/magnitude/Magnitude.h"

Magnitude::Magnitude() {
    mathFunction_ = NULL;
}

Magnitude::Magnitude(FunctionRR* mathFunction) {
    mathFunction_ = mathFunction;
}

Magnitude::Magnitude(const Magnitude& rhs) {
    mathFunction_ = rhs.mathFunction_->cloneTo<FunctionRR>();
}

Magnitude::~Magnitude() {
    if (mathFunction_ != NULL) {
        delete mathFunction_;
    }
}

Magnitude& Magnitude::operator =(const Magnitude& rhs) {
    if (this == &rhs) {
        return *this;
    }
    mathFunction_ = rhs.mathFunction_->cloneTo<FunctionRR>();
    return *this;
}

bool Magnitude::operator ==(const Magnitude& rhs) const {
    return *mathFunction_ == *rhs.mathFunction_;
}

void Magnitude::printInfo() const {
    cout << " --- Magnitude ---" << endl;
    mathFunction_->printInfo();
}

Real Magnitude::evaluate(const Real time) const {
    return mathFunction_->operator()(time);
}
