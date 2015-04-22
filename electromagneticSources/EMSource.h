// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "geometry/elements/ElementsGroup.h"
#include "magnitude/MagnitudeGaussian.h"
#include "magnitude/MagnitudeNumerical.h"
#include "math/CartesianVector.h"
#include "math/Constants.h"

#include "base/class/ClassGroupBase.h"

class EMSourceBase : public virtual ClassGroupBase<ElementsGroup<const Elem>> {
public:
    EMSourceBase();
    EMSourceBase(const Magnitude* magnitude);
    EMSourceBase(const EMSourceBase& rhs);
    virtual ~EMSourceBase();

    virtual bool hasSameProperties(const EMSourceBase& rhs) const;
    virtual bool isSimilar(const EMSourceBase& rhs) const = 0;
    virtual bool check() const = 0;

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


    virtual void printInfo() const;

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

    ElementsGroup<const Elem> elems() const { return *this; }

    bool isSimilar(const EMSourceBase& rhs) const;
    bool check() const;

    void set(const ElementsGroup<const Elem>&);
    void add(const ElementsGroup<const Elem>&);

    void printInfo() const;
};



#include "EMSource.hpp"

#endif /* ELECTROMAGNETICSOURCE_H_ */
