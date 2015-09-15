// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "geometry/elements/GroupElements.h"
#include "options/OptionsMesher.h"
#include "../sources/magnitude/MagnitudeNumerical.h"
#include "math/FunctionGaussian.h"
#include "math/CartesianVector.h"
#include "math/Constants.h"

#include "base/class/ClassGroupBase.h"

class EMSourceBase : public virtual ClassGroupBase<GroupElements<const Elem>> {
public:
    EMSourceBase();
    EMSourceBase(const Magnitude* magnitude);
    EMSourceBase(const EMSourceBase& rhs);
    virtual ~EMSourceBase();

    virtual bool hasSameProperties(const EMSourceBase& rhs) const;
    virtual bool isSimilar(const EMSourceBase& rhs) const = 0;
    bool check() const;

    template<class T>
    bool magnitudeIs() const {
        return magnitude_->is<T>();
    }

    virtual string getMagnitudeFilename() const;
    virtual const string& getName() const = 0;

    void convertToNumerical(const ProjectFile& file,
                            const Real step,
                            const Real finalTime);
    MagnitudeNumerical* exportToFile(const ProjectFile& file,
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
                 public virtual GroupElements<const T> {
public:
    EMSource() {}
    virtual ~EMSource() {}

    virtual void set(const GroupElements<const Elem>&);
    void add(const GroupElements<const Elem>&);

    virtual EMSource* clone() const = 0;
    GroupElements<const Elem> elems() const { return *this; }

    bool isSimilar(const EMSourceBase& rhs) const;
    void printInfo() const;
};

#include "../sources/EMSource.hpp"

#endif /* ELECTROMAGNETICSOURCE_H_ */
