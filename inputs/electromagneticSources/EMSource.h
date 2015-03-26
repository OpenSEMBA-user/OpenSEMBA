// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include "magnitude/MagnitudeGaussian.h"
#include "magnitude/MagnitudeNumerical.h"
#include "../../geometry/elements/ElementsGroup.h"
#include "../../math/CartesianVector.h"
#include "../../math/Constants.h"

using namespace std;

template<class T = void>
class EMSource;

template<>
class EMSource<void> : public virtual ClassBase {
public:
    EMSource();
    EMSource(const Magnitude* magnitude);
    EMSource(const EMSource& rhs);
    virtual ~EMSource();

    template<class T>
    bool magnitudeIs() {
        return magnitude_->is<T>();
    }

    virtual string getMagnitudeFilename() const;
    virtual string getName() const = 0;
    virtual ClassBase* clone() const = 0;

    void convertToNumerical(const string file,
                            const double step,
                            const double finalTime);

    virtual ElementsGroup<> elems() const = 0;

    void printInfo() const;

protected:
    const Magnitude* getMagnitude() const;

private:
    const Magnitude* magnitude_;
};

template<class T>
class EMSource : public virtual EMSource<void>,
                 public virtual ElementsGroup<T> {
public:
    EMSource() {}
    virtual ~EMSource() {}

    ElementsGroup<> elems() const {
        return *this;
    }

    void printInfo() const { EMSource<void>::printInfo(); }
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
