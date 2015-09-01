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
typedef array<Pixel,4> SurfelPixels;

class Surfel: public Pixel {
public:
    Surfel(){normId_ = CartesianDirection::dirNode;};
    Surfel(const CVecI3& pos): Pixel(pos){normId_ = CartesianDirection::dirNode;}
    Surfel(const CVecI3& pos, const CartesianDirection& norm) : Pixel(pos), normId_(norm){};

    virtual ~Surfel(){};

    bool isInto (const CVecI3Frac& intFractional)const;

    bool operator==(const Surfel& rhs) const;
    bool operator<(const Surfel& rhs) const;

    CartesianDirection  getNormId ()const{return normId_;}
    CartesianDirection& NormId (){return normId_;}
    void setNormId (const CartesianDirection& _normId){normId_ = _normId;}

//    SurfelLinels getLinels() const;
//    Linel getLinel(const UInt side) const;
    SurfelPixels getPixels() const;

    string toStr() const;
    friend std::ostream& operator<<(ostream& os, const Surfel& vec) {
       return os << vec.toStr();
    }

protected:
    CartesianDirection normId_;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_SURFEL_H_ */
