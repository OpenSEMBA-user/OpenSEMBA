

#include "Grid.h"

#include <stdexcept>

namespace SEMBA {
namespace Geometry {

template<std::size_t D>
const Math::Real Grid<D>::tolerance = 1e-2;

template<std::size_t D>
Grid<D>::Grid(const BoxRD& box,
              const CVecRD& dxyz) {
    CVecRD origin = box.getMin();
    for (std::size_t i = 0; i < D; i++) {
        Math::Real boxLength = box.getMax()(i) - box.getMin()(i);
        std::size_t nCells;
        if (dxyz(i) == (Math::Real) 0.0) {
            nCells = 1;
        } else {
            nCells = ceil(boxLength / dxyz(i));
            if (Math::Util::greater(boxLength, nCells*dxyz(i),
                    dxyz(i), tolerance)) {
                nCells++;
            }
        }
        pos_[i].resize(nCells+1);
        for (std::size_t j = 0; j < nCells+1; j++) {
            pos_[i][j] = origin(i) + j * dxyz(i);
        }
    }
}

template<std::size_t D>
Grid<D>::Grid(const BoxRD &boundingBox,
              const CVecID& dims) {
    CVecRD origin = boundingBox.getMin();
    for (std::size_t i = 0; i < D; i++) {
        Math::Real step =
                (boundingBox.getMax()(i) - boundingBox.getMin()(i)) / dims(i);
        std::size_t nCells = dims(i);
        pos_[i].resize(nCells+1);
        for (std::size_t j = 0; j < nCells+1; j++) {
            pos_[i][j] = origin(i) + j * step;
        }
    }
}

template<std::size_t D>
Grid<D>::Grid(const std::vector<Math::Real> pos[D]) {
    for(std::size_t d = 0; d < D; d++) {
        pos_[d] = pos[d];
    }
}


template<std::size_t D>
Grid<D>::Grid(const Grid<D>& grid) {
    for (std::size_t i = 0; i < D; i++) {
        pos_[i] = grid.pos_[i];
    }
}

template<std::size_t D>
Grid<D>& Grid<D>::operator=(const Grid<D>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    for (std::size_t i = 0; i < D; i++) {
        pos_[i] = rhs.pos_[i];
    }
    return *this;
}

template<std::size_t D>
bool Grid<D>::operator==(const Grid& rhs) const {
    return this->pos_ == rhs.pos_;
}

template<std::size_t D>
void Grid<D>::setPos(const std::vector<Math::Real> pos[D]) {
    for(std::size_t d = 0; d < D; d++) {
        if (pos[d].size() == 0) {
            throw std::out_of_range(
                      "Grid positions must contain at least one value");
        }
        pos_[d] = pos[d];
        if (pos_[d].size() == 1) {
            pos_[d].push_back(pos_[d][0]);
        }
    }
}

template<std::size_t D>
void Grid<D>::setAdditionalSteps(
        const Math::Constants::CartesianAxis d,
        const Math::Constants::CartesianBound b,
        const std::vector<Math::Real>& step) {
    const std::size_t nCells = step.size();
    std::vector<Math::Real> newPos(nCells);
    if (b == Math::Constants::U) {
        newPos[0] = pos_[d].back() + step[0];
        for (std::size_t i = 1; i < nCells; i++) {
            newPos[i] = newPos[i-1] + step[i];
        }
        pos_[d].insert(pos_[d].end(), newPos.begin(), newPos.end());
    } else {
        newPos[0] = pos_[d].front() - step[0];
        for (std::size_t i = 1; i < nCells; i++) {
            newPos[i] = newPos[i-1] - step[i];
        }
        std::reverse(newPos.begin(), newPos.end());
        newPos.insert(newPos.end(), pos_[d].begin(), pos_[d].end());
        pos_[d] = newPos;
    }
}

template<std::size_t D>
bool Grid<D>::hasZeroSize() const {
    bool res = true;
    for (std::size_t i = 0; i < D; i++) {
        res &= (pos_[i].size() <= 1);
    }
    return res;
}

template<std::size_t D>
bool Grid<D>::isInto(const std::size_t dir, const Math::Real pos) const {
    if (pos >= getPos(dir)[0] && pos <= getPos(dir).back()) {
        return true;
    }
    return false;
}

template<std::size_t D>
bool Grid<D>::getNaturalCell(
        const Math::Constants::CartesianAxis dir,
        const Math::Real& x,
        long int& i,
        Math::Real& relativeLen) const {
    size_t n = 0;
    relativeLen = -1.0;
    if (x < getPos(dir,0)) {
        i = 0;
        return false;
    } else if (getPos(dir, getNumCells()(dir)) <= x) {
        i = getNumCells()(dir);
        return false;
    }
    while (getPos(dir)[n] <=x ){
        n++;
    }  /*mod this: use sort*/
    i = n-1;
    relativeLen = (x - getPos(dir)[i])/
                  getStep(dir,i);
    return true;
}

template<std::size_t D>
bool Grid<D>::isInto(const CVecRD& pos) const {
    for (std::size_t i = 0; i < D; i++) {
        if (!isInto(i, pos(i))) {
            return false;
        }
    }
    return true;
}

template<std::size_t D>
bool Grid<D>::isRegular() const {
    for (std::size_t i = 0; i < D; i++) {
        if (!isRegular(i)) {
            return false;
        }
    }
    return true;
}

template<std::size_t D>
bool Grid<D>::isRegular(const std::size_t d) const {
    std::vector<Math::Real> step = getStep(d);
    for (std::size_t n = 1; n < step.size(); n++) {
        if (Math::Util::notEqual(step[n], step[0], step[0], tolerance)) {
            return false;
        }
    }
    return true;
}

template<std::size_t D>
bool Grid<D>::isCartesian() const {
    Math::Real canon = getStep(Math::Constants::x)[0];
    for (std::size_t i = 0; i < D; i++) {
        std::vector<Math::Real> step = getStep(i);
        for (std::size_t n = 1; n < step.size(); n++) {
            if (Math::Util::notEqual(step[n], canon, canon, tolerance)) {
                return false;
            }
        }
    }
    return true;
}

template<std::size_t D>
bool Grid<D>::isCell(const CVecRD& position, const Math::Real tol) const {
    std::pair<CVecID, CVecRD> natCell = getCellPair(position, true, tol);
    return natCell.second == CVecRD(0.0);
}

template<std::size_t D>
bool Grid<D>::isCell(const std::vector<CVecRD>& pos,
                     const Math::Real tol) const {
    for (std::size_t i = 0; i < pos.size(); i++) {
        if (!isCell(pos[i], tol)) {
            return false;
        }
    }
    return true;
}

template<std::size_t D>
Math::Vector::Cartesian<Math::Int,D> Grid<D>::getNumCells() const {
    CVecID res;
    for (std::size_t d = 0; d < D; d++) {
        res(d) = getPos(d).size() - 1; // Minimum size of pos is 2.
    }
    return res;
}

template<std::size_t D>
Math::Vector::Cartesian<Math::Int,D> Grid<D>::getOffset() const {
    return CVecID(0,0,0);
}

template<std::size_t D>
Math::Vector::Cartesian<Math::Real,D> Grid<D>::getOrigin() const {
    CVecRD res;
    for (std::size_t d = 0; d < D; d++) {
        if (pos_[d].size() == 0) {
            throw std::out_of_range("Positions are not initialized.");
        }
        res(d) = pos_[d][0];
    }
    return res;
}

template<std::size_t D>
std::vector<Math::Real> Grid<D>::getStep(const std::size_t dir) const {
    assert(dir >= 0 && dir < D);
    if (pos_[dir].size() == 0) {
        return std::vector<Math::Real>();
    }
    std::vector<Math::Real> res(pos_[dir].size()-1);
    for (std::size_t i = 0; i < pos_[dir].size()-1; i++) {
        res[i] = pos_[dir][i+1] - pos_[dir][i];
    }
    return res;
}


template<std::size_t D>
Math::Real Grid<D>::getStep(const std::size_t dir, const Math::Int& n) const {
    assert(dir >= 0 && dir < D);
    assert(n   >= 0 && n < (Math::Int(pos_[dir].size()) - 1));

    if (pos_[dir].empty()) {
        return 0.0;
    }
    return pos_[dir][n+1] - pos_[dir][n];
}



template<std::size_t D>
Math::Real Grid<D>::getMinimumSpaceStep() const {
    Math::Real minStep = std::numeric_limits<Math::Real>::infinity();
    for (std::size_t i = 0; i < D; i++) {
        std::vector<Math::Real> step = getStep(i);
        for (std::size_t n = 0; n < step.size(); n++) {
            if (step[n] < minStep) {
                minStep = step[n];
            }
        }
    }
    return minStep;
}

template<std::size_t D>
Box<Math::Real,D> Grid<D>::getFullDomainBoundingBox() const {
    return getBoundingBox(
            std::pair<CVecID,CVecID> (CVecID(0,0,0), getNumCells()));
}

template<std::size_t D>
Box<Math::Int,D> Grid<D>::getFullDomainBoundingCellBox() const {
    CVecID min, max, dims;
    for (std::size_t n=0; n<D; n++){
        dims[n] = pos_[n].size();
    }

    return BoxID(CVecID(0,0,0), dims);
}

template<std::size_t D>
Box<Math::Real,D> Grid<D>::getBoundingBox(const BoxID& bound) const {
    BoxRD res(getPos(bound.getMin()), getPos(bound.getMax()));
    return res;
}

template<std::size_t D>
Box<Math::Real,D> Grid<D>::getBoxRContaining(const CVecRD& point) const {
    BoxID boxI = getBoxIContaining(point);
    return getBoundingBox(boxI);
}


template<std::size_t D>
Box<Math::Int,D> Grid<D>::getBoxIContaining(const CVecRD& point) const {
    CVecID min = getCell(point, false);
    CVecID max = min + 1;
    return BoxID(min, max);
}

template<std::size_t D>
std::vector< Math::Vector::Cartesian<Math::Real,D> >
    Grid<D>::getCenterOfCellsInside(const BoxRD& bound) const {
    // Determines centers of cells.
    std::vector<Math::Real> center[D];
    for (std::size_t dir = 0; dir < D; dir++) {
        std::vector<Math::Real> pos = getPosInRange(dir,
                bound.getMin()(dir),
                bound.getMax()(dir));
        if (pos.size() > 0) {
            center[dir].reserve(pos.size()-1);
        }
        for (std::size_t i = 1; i < pos.size(); i++) {
            Math::Real auxCenter = (pos[i-1] + pos[i]) / 2.0;
            center[dir].push_back(auxCenter);
        }
    }
    // Combines centers in a std::vector of CVecRD positions.
    std::vector<CVecRD> res;
    res.reserve(center[Math::Constants::x].size() *
                center[Math::Constants::y].size() *
                center[Math::Constants::z].size());
    for (std::size_t i = 0; i < center[Math::Constants::x].size(); i++) {
        for (std::size_t j = 0; j < center[Math::Constants::y].size(); j++) {
            for (std::size_t k = 0; k < center[Math::Constants::z].size();
                 k++) {
                res.push_back(CVecRD(center[Math::Constants::x][i],
                                     center[Math::Constants::y][j],
                                     center[Math::Constants::z][k]));
            }
        }
    }
    return res;
}

template<std::size_t D>
std::vector<Math::Real> Grid<D>::getPosInRange(const std::size_t dir,
        const Math::Real min,
        const Math::Real max) const {
    std::vector<Math::Real> pos   = getPos (dir);
    std::vector<Math::Real> steps = getStep(dir);
    std::vector<Math::Real> res;
    res.reserve(pos.size());
    for (std::size_t i = 0; i < pos.size(); i++) {
        Math::Real step;
        if (i < steps.size()) {
            step = steps[i];
        } else {
            step = steps.back();
        }
        const bool inMin = Math::Util::equal(pos[i], min, step, tolerance);
        const bool inMax = Math::Util::equal(pos[i], max, step, tolerance);
        const bool inRange = (pos[i] >= min && pos[i] <= max);
        if (inMin || inMax || inRange) {
            res.push_back(pos[i]);
        }
    }
    return res;
}

template<std::size_t D>
std::vector< Math::Vector::Cartesian<Math::Real,D> > Grid<D>::getPos() const {
    // Combines positions in a std::vector of CVecRD positions.
    std::vector<CVecRD> res;
    res.reserve(pos_[Math::Constants::x].size() *
                pos_[Math::Constants::y].size() *
                pos_[Math::Constants::z].size());
    for (std::size_t i = 0; i < pos_[Math::Constants::x].size(); i++) {
        for (std::size_t j = 0; j < pos_[Math::Constants::y].size(); j++) {
            for (std::size_t k = 0; k < pos_[Math::Constants::z].size(); k++) {
                res.push_back(CVecRD(pos_[Math::Constants::x][i],
                                     pos_[Math::Constants::y][j],
                                     pos_[Math::Constants::z][k]));
            }
        }
    }
    return res;
}

template<std::size_t D>
std::vector<Math::Real> Grid<D>::getPos(const std::size_t direction) const {
    assert(direction >= 0 && direction < D);
    return pos_[direction];
};

template<std::size_t D>
Math::Vector::Cartesian<Math::Real,D> Grid<D>::getPos(
        const CVecID& ijk) const {
    CVecRD res;
    for (std::size_t i = 0; i < D; i++) {
        res(i) = pos_[i][ijk(i)];
    }
    return res;
};

template<std::size_t D>
Math::Real Grid<D>::getPos(const std::size_t dir, const Math::Int i) const {
    return  pos_[dir][i];
}

template<std::size_t D>
std::pair<Math::Int, Math::Real> Grid<D>::getCellPair(const std::size_t dir,
                                                      const Math::Real x,
                                                      const bool approx,
                                                      const Math::Real tol,
                                                      bool* err) const {
    if (err != nullptr) {
        *err = false;
    }

    Math::Int  cell;
    Math::Real dist;
    std::vector<Math::Real> pos   = getPos (dir);
    std::vector<Math::Real> steps = getStep(dir);
    assert(pos_[dir].size() >= 1);
    // Checks if it is below the grid.
    if (Math::Util::lower(x, pos[0], steps[0], tol)) {
        cell = 0;
        dist = (x-pos[0])/steps[0];
        if (err != nullptr) {
            *err = true;
        }
        return std::make_pair(cell, dist);
    }
    for(std::size_t i = 0; i < pos.size(); i++) {
        Math::Real step;
        if (i == 0) {
            step = steps.front();
        } else if (i <= steps.size()) {
            step = steps[i-1];
        } else {
            step = steps.back();
        }
        if (Math::Util::equal(x, pos[i], step, tol)) {
            cell = i;
            dist = 0.0;
            if (err != nullptr) {
                *err = false;
            }
            return std::make_pair(cell, dist);
        } else if (Math::Util::lower(x, pos[i], step, tol)) {
            cell = i - 1;
            dist = (x - pos[i-1])/step;
            if(Math::Util::equal(Math::Util::round(dist),1.0) && approx) {
                cell++;
                dist -= 1.0;
            }
            if (err != nullptr) {
                *err = false;
            }
            return std::make_pair(cell, dist);
        }
    }
    cell = getNumCells()(dir);
    dist = (x - pos.back())/steps.back();
    if (err != nullptr) {
        *err = true;
    }
    return std::make_pair(cell, dist);
}

template<std::size_t D>
std::pair<Math::Vector::Cartesian<Math::Int,D>,
          Math::Vector::Cartesian<Math::Real,D>>
          Grid<D>::getCellPair(const CVecRD& xyz,
                               const bool approx,
                               const Math::Real tol,
                               bool* err) const {
    if (err != nullptr) {
        *err = false;
    }
    bool stepErr = false;

    CVecID cell;
    CVecRD dist;
    for (std::size_t dir = 0; dir < D; dir++) {
        std::pair<Math::Int, Math::Real> res =
            getCellPair(dir,xyz(dir),approx,tol,&stepErr);
        cell(dir) = res.first;
        dist(dir) = res.second;
        if (err != nullptr) {
            *err = *err || stepErr;
        }
    }
    return std::make_pair(cell, dist);
}

template<std::size_t D>
Math::Int Grid<D>::getCell(const std::size_t dir,
                           const Math::Real x,
                           const bool approx,
                           const Math::Real tol,
                           bool* err) const {
    return getCellPair(dir, x, approx, tol, err).first;
}

template<std::size_t D>
Math::Vector::Cartesian<Math::Int,D> Grid<D>::getCell(const CVecRD &coords,
        const bool approx,
        const Math::Real tol,
        bool* err) const {
    return getCellPair(coords, approx, tol, err).first;
}

template<std::size_t D>
void Grid<D>::applyScalingFactor(const Math::Real factor) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t i = 0; i < pos_[d].size(); i++) {
            pos_[d][i] *= factor;
        }
    }
}

template<std::size_t D>
void Grid<D>::enlarge(const std::pair<CVecRD, CVecRD>& pad,
                      const std::pair<CVecRD, CVecRD>& sizes) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t b = 0; b < 2; b++) {
            if (b == Math::Constants::L) {
                enlargeBound(Math::Constants::CartesianAxis(d),
                             Math::Constants::CartesianBound(b),
                             pad.first(d), sizes.first(d));
            } else {
                enlargeBound(Math::Constants::CartesianAxis(d),
                             Math::Constants::CartesianBound(b),
                             pad.second(d), sizes.second(d));
            }
        }
    }
}

template<std::size_t D>
void Grid<D>::enlargeBound(Math::Constants::CartesianAxis d,
                           Math::Constants::CartesianBound b,
                           Math::Real pad, Math::Real siz) {
    assert(getNumCells()(d) > 0);
    if (std::abs(siz) > std::abs(pad)) {
        std::cerr << "WARNING @ Grid enlarge bound: "
                << "std::size_t was larger than padding. Ignoring padding in "
                << "axis " << d << " and bound " << b << "." << std::endl;
        return;
    }
    if (pad == 0.0) {
        return;
    }
    Math::Int boundCell;
    if (b == Math::Constants::L) {
        boundCell = 0;
    } else {
        boundCell = this->getNumCells()(d) - 1;
    }
    std::vector<Math::Real> newSteps;
    if (Math::Util::greaterEqual(getStep(d,b), siz) || siz == 0.0) {
        siz = getStep(d,boundCell);
        // Computes enlargement for a padding with same size.
        Math::Real nCellsFrac = std::abs(pad/siz);
        const Math::Real tol = 0.01;
        std::size_t nCells = (std::size_t) Math::Util::ceil(nCellsFrac, tol);
        newSteps.resize(nCells, siz);
    } else {
        // Computes enlargement for padding with different size.
        // Taken from AutoCAD interface programmed in LISP (2001).
        Math::Real d12 = getStep(d,boundCell);
        Math::Real d14 = std::abs(pad) + d12 + std::abs(siz);
        Math::Real d34 = std::abs(siz);
        Math::Real d13 = d14 - d34;
        Math::Real t0 = d12;
        Math::Real r0 = (d14-d12) / (d14-d34);
        Math::Real r = r0;
        Math::Int n = Math::Util::ceil(log(d34/d12)/log(r0),
                                       (Math::Real) 0.01) - 1;
        if (n > 1) {
            // Newton method to adjust the sum of available space.
            Math::Real f = 1;
            const Math::Real threshold =
                    std::numeric_limits<Math::Real>::epsilon()*1.0e6;
            while (std::abs(f) >= threshold) {
                f = t0 * (1-pow(r,n)) / (1-r) - d13;
                Math::Real df = t0*(1-pow(r,n))/pow(1-r,2) - 
                                t0*n*pow(r,n-1)/(1-r);
                r = r - f / df;
            }
            newSteps.resize(n-1);
            for (Math::Int i = 0; i < n-2; i++) {
                newSteps[i] = t0 * pow(r,(i+1));
            }
            newSteps[n-2] = d34;
        } else {
            newSteps.resize(1, d34);
        }
    }
    setAdditionalSteps(d, b, newSteps);
}

} /* namespace Geometry */
} /* namespace SEMBA */
