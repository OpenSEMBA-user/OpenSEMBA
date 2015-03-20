// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include "Magnitude.h"
#include "MagnitudeGaussian.h"
#include "MagnitudeNumerical.h"
#include "ElementsGroup.h"
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

    void convertToNumerical(const string file,
                            const double step,
                            const double finalTime);

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

    virtual ClassBase* clone() const = 0;

    void printInfo() const { EMSource<void>::printInfo(); }
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
