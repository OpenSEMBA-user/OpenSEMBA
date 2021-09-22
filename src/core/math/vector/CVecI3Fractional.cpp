

#include "CVecI3Fractional.h"

namespace SEMBA {
namespace Math {

CVecI3Fractional::CVecI3Fractional() {
}

CVecI3Fractional& CVecI3Fractional::operator =(
        const CVecI3Fractional & rhs) {
    this->CVecI3::operator =(rhs);
    len_ = rhs.len_;
    return *this;
}

bool CVecI3Fractional::operator ==(const CVecI3Fractional& rhs) const {
    if(CVecI3::operator !=(rhs)){
        return false;
    }
    for (unsigned int n = 0; n < 3; ++n) {
        if(len_(n) != rhs.len_(n)){
            return false;
        }
    }
    return true;
}


CartesianDirection CVecI3Fractional::getDirBase() const {
    std::size_t rang = this->getRangeBase();
    if (rang == 1) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] > Util::tolerance) {
                return CartesianDirection(n + 1);
            }
        }
    } else if (rang == 2) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] < Util::tolerance) {
                return CartesianDirection(n + 1);
            }
        }
    }
    return dirNode;
}

std::size_t CVecI3Fractional::getRangeBase() const {
    std::size_t rang = 0;
    for (std::size_t n = 0; n < 3; ++n) {
        if (len_[n] > Util::tolerance) {
            ++rang;
        }
    }
    return rang;
}

CVecR3 CVecI3Fractional::getScalePos() const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = (Real)val[n] + len_.val[n];
    }
    return retPos;
}

CVecR3 CVecI3Fractional::getScalePos(const CVecI3 origin) const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos(n) = (Real)((*this)(n)-origin(n)) + this->len_(n);
    }
    return retPos;
}

std::string CVecI3Fractional::toStr() const {
    return CVecI3::toStr() + " len: " + len_.toStr();
}



CVecR3 CVecI3Fractional::meanRelativePoint(const CVecI3Fractional& rhs) const {
    return (len_ + rhs.len_)*0.5;
}

CVecR3 CVecI3Fractional::getRelativePosNearestNode() const {
    CVecR3 nearestPos;
    for (std::size_t dir = 0; dir<3; ++dir) {
        nearestPos(dir) = 0.0;
        if(len_(dir)>0.5){
            nearestPos(dir) = 1.0;
        }
    }
    return nearestPos;
}

void CVecI3Fractional::join(CVecI3Fractional& rhs) {
    for (std::size_t dir = 0; dir<3; ++dir) {
        if(Util::equal(len_(dir),0.0)){
            rhs.len_(dir) = 0.0;
        }else if(Util::equal(rhs.len_(dir),0.0)){
            len_(dir) = 0.0;
        }else{
            len_(dir) = rhs.len_(dir);
        }
    }
}

CVecI3Fractional& CVecI3Fractional::reduceTopology() {
    CVecR3 posNode = getRelativePosNearestNode();
    CVecR3 dst =  (posNode - len_).abs();
    Real dstMin = 1e20;
    Int minDir = -1;
    for (std::size_t dir = 0; dir<3; ++dir) {
        if(len_(dir)!=0.0){
            if(dstMin>dst[dir]){
                dstMin = dst(dir);
                minDir = dir;
            }
        }
    }
    if(minDir!=-1){
        len_(minDir) = posNode(minDir);
        reduceCoords();
    }
    return *this;
}

CVecI3Fractional& CVecI3Fractional::reduceTopology(const std::size_t range) {
    std::size_t rangeLoc = getRangeBase();
    std::size_t n = 0;
    while(rangeLoc>range && rangeLoc>0 && n <3){
        reduceTopology();
        rangeLoc = getRangeBase();
        ++n;
    }
    return *this;
}

CVecI3Fractional& CVecI3Fractional::reduceCoords() {
    for (std::size_t dir = 0; dir<3; ++dir) {
        if(len_(dir)>=(1.0-Util::tolerance)){
            len_(dir)= 0.0;
            (*this)(dir)++;
//        }else if (len_(dir)<(-1.0)*MathUtils::tolerance){
//            len_(dir) = 0.0;
//            val[dir]--;
        }else if (len_(dir) < Util::tolerance){
            len_(dir)=0.0;
        }
    }

    return *this;
}

CVecI3Fractional& CVecI3Fractional::move(
        CVecI3Fractional& rhs,
        const bool forceProject,
        bool& canBeMoved) const {
    canBeMoved = true;
    CVecR3 len;
    for (std::size_t dir = 0; dir < 3; ++dir) {
        if (Util::equal(len_(dir), 0.0)) {
            len(dir) = 0.0;
        } else {
            if (Util::equal(rhs.len_(dir), 0.0)) {
                canBeMoved = false;
            } else {
                len(dir) = len_(dir);
            }
        }
    }
    if (canBeMoved || forceProject) {
        rhs.len_ = len;
    }
    return rhs;
}

} /* namespace Math */
} /* namespace SEMBA */
