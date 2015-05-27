/*
 * Linel.h
 *
 *  Created on: May 19, 2015
 *      Author: mdebi
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_LINEL_H_
#define SRC_MESHER_FDTD_MESHCONF_LINEL_H_

#include "Pixel.h"

class Linel: public Pixel {
public:
    Linel();
    Linel(const CVecI3& pos, const CartesianDirection& dir);
    virtual ~Linel();

    bool isInto (const CVecI3Fractional& coordIntFractional_);

    bool operator==(const Linel& rhs) const;

    CartesianDirection  getDirId ()const{return dirId_;}
    CartesianDirection& DirId (){return dirId_;}
    void setDirId (const CartesianDirection& _dirId){dirId_ = _dirId;}

private:
    CartesianDirection dirId_;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_LINEL_H_ */
