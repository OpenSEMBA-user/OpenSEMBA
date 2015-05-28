/*
 * Surfel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_SURFEL_H_
#define SRC_MESHER_FDTD_MESHCONF_SURFEL_H_

#include "Linel.h"

typedef array<Linel,4> SurfelLinels;

class Surfel: public Pixel {
public:
    Surfel(){normId_ = CartesianDirection::dirNode;};
    Surfel(const CVecI3& cVecI3_): Pixel(cVecI3_){normId_ = CartesianDirection::dirNode;}
    Surfel(const CVecI3& pos, const CartesianDirection& norm);

    virtual ~Surfel(){};

    bool isInto (const CVecI3Frac& intFractional);

    bool operator==(const Surfel& rhs) const;

    CartesianDirection  getNormId ()const{return normId_;}
    CartesianDirection& NormId (){return normId_;}
    void setNormId (const CartesianDirection& _normId){normId_ = _normId;}

    SurfelLinels getLinels() const;
    Linel getLinel(const UInt side) const;

    string toStr() const;
    friend std::ostream& operator<<(ostream& os, const Surfel& vec) {
       return os << vec.toStr();
    }

protected:

private:
    CartesianDirection normId_;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_SURFEL_H_ */
