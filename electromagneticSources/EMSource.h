// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>

using namespace std;

#include "magnitude/MagnitudeGaussian.h"
#include "magnitude/MagnitudeNumerical.h"
#include "../../geometry/elements/ElementsGroup.h"
#include "../../math/CartesianVector.h"
#include "../../math/Constants.h"

#include "ClassGroupBase.h"

class EMSourceBase : public virtual ClassGroupBase<Elem> {
public:
    EMSourceBase();
    EMSourceBase(const Magnitude* magnitude);
    EMSourceBase(const EMSourceBase& rhs);
    virtual ~EMSourceBase();

    template<class T>
    bool magnitudeIs() const {
        return magnitude_->is<T>();
    }

    virtual string getMagnitudeFilename() const;
    virtual const string& getName() const = 0;

    void convertToNumerical(const string file,
                            const Real step,
                            const Real finalTime);
    MagnitudeNumerical* exportToFile(const string file,
                                     const Real step,
                                     const Real finalTime) const;

    virtual void printInfo() const = 0;

protected:
    const Magnitude* getMagnitude() const;

private:
    const Magnitude* magnitude_;
};

template<class T>
class EMSource : public virtual EMSourceBase,
                 public virtual ElementsGroup<const T> {
public:
    EMSource() {}
    virtual ~EMSource() {}

    virtual ClassBase* clone() const = 0;

    Group<const Elem> elems() const { return *this; }

    void set(const Group<const Elem>&);
    void add(const Group<const Elem>&);

    virtual void printInfo() const = 0;
};

#include "EMSource.hpp"

#endif /* ELECTROMAGNETICSOURCE_H_ */
