#include "Grid.h"

template<Int D>
const Real Grid<D>::tolerance = 1e-2;

template<Int D>
Grid<D>::Grid() {

}

template<Int D>
Grid<D>::Grid(const BoxRD& box,
              const CVecRD& dxyz) {
    origin_ = box.getMin();
    offsetGrid_ = CVecID(0, 0, 0);
    for (Int i = 0; i < D; i++) {
        Real boxLength = box.getMax()(i) - box.getMin()(i);
        Int nCells;
        if (dxyz(i) == (Real) 0.0) {
            nCells = 1;
        } else {
            nCells = ceil(boxLength / dxyz(i));
            if (MathUtils::greater(boxLength, nCells*dxyz(i),
                    dxyz(i), tolerance)) {
                nCells++;
            }
        }
        pos_[i].resize(nCells+1);
        for (Int j = 0; j < nCells+1; j++) {
            pos_[i][j] = origin_(i) + j * dxyz(i);
        }
    }
}

template<Int D>
Grid<D>::Grid(const BoxRD &boundingBox,
              const CVecID& dims) {
    origin_ = boundingBox.getMin();
    offsetGrid_ = CVecID(0, 0, 0);
    for (Int i = 0; i < D; i++) {
        Real step =
        (boundingBox.getMax()(i) - boundingBox.getMin()(i)) / dims(i);
        Int nCells = dims(i);
        pos_[i].resize(nCells+1);
        for (Int j = 0; j < nCells+1; j++) {
            pos_[i][j] = origin_(i) + j * step;
        }
    }
}

template<Int D>
Grid<D>::Grid(const CVecID& offset,
              const CVecRD& origin,
              const vector<Real> step[D]) {
    origin_ = origin;
    offsetGrid_ = offset;
    for(Int d = 0; d < D; d++) {
        pos_[d].resize(step[d].size()+1);
        pos_[d][0] = origin(d);
        for (UInt i = 0; i < step[d].size(); i++) {
            pos_[d][i+1] = pos_[d][i] + step[d][i];
        }
    }
}

template<Int D>
Grid<D>::Grid(const Grid<D>& grid) {
    origin_ = grid.origin_;
    offsetGrid_ = grid.offsetGrid_;
    for (Int i = 0; i < D; i++) {
        pos_[i] = grid.pos_[i];
    }
}

template<Int D>
Grid<D>::~Grid() {

}

template<Int D>
Grid<D>& Grid<D>::operator=(const Grid<D>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    origin_ = rhs.origin_;
    offsetGrid_ = rhs.offsetGrid_;
    for (Int i = 0; i < D; i++) {
        pos_[i] = rhs.pos_[i];
    }
    return *this;
}

template<Int D>
bool Grid<D>::hasZeroSize() const {
    bool res = true;
    for (Int i = 0; i < D; i++) {
        res &= (pos_[i].size() <= 1);
    }
    return res;
}

template<Int D>
bool Grid<D>::isInto(const Int dir, const Real pos) const {
    if (pos >= getPos(dir)[0] && pos <= getPos(dir).back()) {
        return true;
    }
    return false;
}

template<Int D>
bool Grid<D>::isIntoDir(const CartesianAxis dir, const double val)const{
    if(val < pos_[dir].front()){return false;}
    if(val > pos_[dir].back()){return false;}
    return true;
}

template<Int D>
bool Grid<D>::getNaturalCellx(
        const double &x,long int &i, double &relativeLen)const{
    long int n; n = 0;
    relativeLen = -1.0;
    if(x<getPx()[0]){i=0; return false;}
    else if(getPx()[getDimsx()]<=x) {i=getDimsx(); return false;}
    while(getPx()[n]<=x){n++;}  /*mod this: use sort*/
    i = n-1; relativeLen = (x-getPx()[i])/getDx()[i]; return true;
}

template<Int D>
bool Grid<D>::getNaturalCelly(
        const double &y,long int &i, double &relativeLen)const{
    long int n; n = 0;
    relativeLen = -1.0;
    if(y<getPy()[0]){i=0; return false;}
    else if(getPy()[getDimsy()]<=y) {i=getDimsy(); return false;}
    while(getPy()[n]<=y){n++;}  /*mod this: use sort*/
    i = n-1; relativeLen = (y-getPy()[i])/getDy()[i]; return true;

}

template<Int D>
bool Grid<D>::getNaturalCellz(
        const double &z,long int &i, double &relativeLen)const{
    long int n; n = 0;
    relativeLen = -1.0;
    if(z<getPz()[0]){i=0; return false;}
    else if(getPz()[getDimsz()]<=z) {i=getDimsz(); return false;}
    while(getPz()[n]<=z){n++;}  /*mod this: use sort*/
    i = n-1; relativeLen = (z-getPz()[i])/getDz()[i]; return true;
}

template<Int D>
bool Grid<D>::isInto(const CVecRD& pos) const {
    for (Int i = 0; i < 3; i++) {
        if (!isInto(i, pos(i))) {
            return false;
        }
    }
    return true;
}

template<Int D>
bool Grid<D>::isRegular() const {
    for (Int i = 0; i < D; i++) {
        if (!isRegular(i)) {
            return false;
        }
    }
    return true;
}

template<Int D>
bool Grid<D>::isRegular(const Int d) const {
    vector<Real> step = getStep(d);
    for (UInt n = 1; n < step.size(); n++) {
        if (MathUtils::notEqual(step[n], step[0], step[0], tolerance)) {
            return false;
        }
    }
    return true;
}

template<Int D>
bool Grid<D>::isCartesian() const {
    Real canon = getStep(x)[0];
    for (Int i = 0; i < D; i++) {
        vector<Real> step = getStep(i);
        for (UInt n = 1; n < step.size(); n++) {
            if (MathUtils::notEqual(step[n], canon, canon, tolerance)) {
                return false;
            }
        }
    }
    return true;
}

template<Int D>
bool Grid<D>::isCell(const CVecRD& position, const Real tol) const {
    pair<CVecID, CVecRD> natCell = getCellPair(position, true, tol);
    return natCell.second == CVecRD(0.0);
}

template<Int D>
bool Grid<D>::isCell(const vector<CVecRD>& pos, const Real tol) const {
    for (UInt i = 0; i < pos.size(); i++) {
        if (!isCell(pos[i], tol)) {
            return false;
        }
    }
    return true;
}

template<Int D>
CartesianVector<Int,D> Grid<D>::getNumCells() const {
    CVecID res;
    for (UInt d = 0; d < D; d++) {
        res(d) = getPos(d).size() - 1;
    }
    return res;
}

template<Int D>
vector<Real> Grid<D>::getStep(const Int dir) const {
    assert(dir >= 0 && dir < D);
    if (pos_[dir].size() == 0) {
        return vector<Real>();
    }
    vector<Real> res(pos_[dir].size()-1);
    for (UInt i = 0; i < pos_[dir].size()-1; i++) {
        res[i] = pos_[dir][i+1] - pos_[dir][i];
    }
    return res;
}


template<Int D>
Real Grid<D>::getStep(const Int dir, const Int& n) const {
    assert(dir >= 0 && dir < D);
    assert(n   >= 0 && n < (pos_.size()-1));

    if (pos_[dir].empty() == 0) {
        return 0.0;
    }
    return pos_[dir][n+1] - pos_[dir][n];
}



template<Int D>
Real Grid<D>::getMinimumSpaceStep() const {
    Real minStep = numeric_limits<Real>::infinity();
    for (Int i = 0; i < D; i++) {
        vector<Real> step = getStep(i);
        for (UInt n = 0; n < step.size(); n++) {
            if (step[n] < minStep) {
                minStep = step[n];
            }
        }
    }
    return minStep;
}

template<Int D>
Box<Real,D> Grid<D>::getFullDomainBoundingBox() const {
    return getBoundingBox(
    pair<CVecID,CVecID> (offsetGrid_, offsetGrid_+getNumCells()) );
}

template<Int D>
Box<Int,D> Grid<D>::getFullDomainBoundingCellBox() const {

    CVecID min, max, dims;
    for (UInt n=0; n<D; n++){
        dims[n] = pos_[n].size();
    }

    BoxID res(offsetGrid_, offsetGrid_ + dims);
    return res;
}

template<Int D>
Box<Real,D> Grid<D>::getBoundingBox(const BoxID& bound) const {
    BoxRD res(getPos(bound.getMin()), getPos(bound.getMax()));
    return res;
}

template<Int D>
Box<Real,D> Grid<D>::getBoxRContaining(const CVecRD& point) const {
    BoxID boxI = getBoxIContaining(point);
    return getBoundingBox(boxI);
}


template<Int D>
Box<Int,D> Grid<D>::getBoxIContaining(const CVecRD& point) const {
    CVecID min = getCell(point, false);
    CVecID max = min + (Int)1;
    return BoxID(min, max);
}

template<Int D>
vector< CartesianVector<Real,D> > Grid<D>::getCenterOfCellsInside(
        const BoxRD& bound) const {
    // Determines centers of cells.
    vector<Real> center[D];
    for (Int dir = 0; dir < D; dir++) {
        vector<Real> pos = getPosInRange(dir,
                                         bound.getMin()(dir),
                                         bound.getMax()(dir));
        if (pos.size() > 0) {
            center[dir].reserve(pos.size()-1);
        }
        for (UInt i = 1; i < pos.size(); i++) {
            Real auxCenter = (pos[i-1] + pos[i]) / 2.0;
            center[dir].push_back(auxCenter);
        }
    }
    // Combines centers in a vector of CVecRD positions.
    vector<CVecRD> res;
    res.reserve(center[x].size() * center[y].size() * center[z].size());
    for (UInt i = 0; i < center[x].size(); i++) {
        for (UInt j = 0; j < center[y].size(); j++) {
            for (UInt k = 0; k < center[z].size(); k++) {
                res.push_back(CVecRD(center[x][i], center[y][j], center[z][k]));
            }
        }
    }
    return res;
}

template<Int D>
vector<Real> Grid<D>::getPosInRange(const Int dir,
                                    const Real min,
                                    const Real max) const {
    vector<Real> pos   = getPos (dir);
    vector<Real> steps = getStep(dir);
    vector<Real> res;
    res.reserve(pos.size());
    for (UInt i = 0; i < pos.size(); i++) {
        Real step;
        if (i < steps.size()) {
            step = steps[i];
        } else {
            step = steps.back();
        }
        if (MathUtils::equal(pos[i], min, step, tolerance) ||
            (pos[i] >= min && pos[i] <= max)               ||
            MathUtils::equal(pos[i], max, step, tolerance)) {
            res.push_back(pos[i]);
        }
    }
    return res;
}

template<Int D>
vector< CartesianVector<Real,D> > Grid<D>::getPos() const {
    // Combines positions in a vector of CVecRD positions.
    vector<CVecRD> res;
    res.reserve(pos_[x].size() * pos_[y].size() * pos_[z].size());
    for (UInt i = 0; i < pos_[x].size(); i++) {
        for (UInt j = 0; j < pos_[y].size(); j++) {
            for (UInt k = 0; k < pos_[z].size(); k++) {
                res.push_back(CVecRD(pos_[x][i], pos_[y][j], pos_[z][k]));
            }
        }
    }
    return res;
}

template<Int D>
vector<Real> Grid<D>::getPos(const Int direction) const {
    assert(direction >= 0 && direction < D);
    return pos_[direction];
};

template<Int D>
CartesianVector<Real,D> Grid<D>::getPos(const CVecID& ijk) const {
    CVecID dims = getNumCells();
    CVecRD res;
    for (Int i = 0; i < D; i++) {
        assert((ijk(i) >= offsetGrid_(i)) &&
               (ijk(i) <= offsetGrid_(i)+dims(i)));
        res(i) = pos_[i][ijk(i) - offsetGrid_(i)];
    }
    return res;
};

template<Int D>
Real Grid<D>::getPos(const Int dir, const Int i) const {
    return getPos(dir)[i];
}

template<Int D>
pair<Int, Real> Grid<D>::getCellPair(const Int  dir,
                                     const Real x,
                                     const bool approx,
                                     const Real tol,
                                     bool* err) const {
    if (err != NULL) {
        *err = false;
    }

    Int  cell;
    Real dist;
    vector<Real> pos   = getPos (dir);
    vector<Real> steps = getStep(dir);
    assert(pos_[dir].size() >= 1);
    // Checks if it is below the grid.
    if (MathUtils::lower(x, pos[0], steps[0], tol)) {
        cell = offsetGrid_(dir);
        dist = (x-pos[0])/steps[0];
        if (err != NULL) {
            *err = true;
        }
        return make_pair(cell, dist);
    }
    for(UInt i = 0; i < pos.size(); i++) {
        Real step;
        if (i == 0) {
            step = steps.front();
        } else if (i <= steps.size()) {
            step = steps[i-1];
        } else {
            step = steps.back();
        }
        if (MathUtils::equal(x, pos[i], step, tol)) {
            cell = i + offsetGrid_(dir);
            dist = 0.0;
            if (err != NULL) {
                *err = false;
            }
            return make_pair(cell, dist);
        } else if (MathUtils::lower(x, pos[i], step, tol)) {
            cell = i - 1 + offsetGrid_(dir);
            dist = (x - pos[i-1])/step;
            if(dist > 0.5 && approx) {
                cell++;
                dist -= 1.0;
            }
            if (err != NULL) {
                *err = false;
            }
            return make_pair(cell, dist);
        }
    }
    cell = getNumCells()(dir) + offsetGrid_(dir);
    dist = (x - pos.back())/steps.back();
    if (err != NULL) {
        *err = true;
    }
    return make_pair(cell, dist);
}

template<Int D>
pair<CartesianVector<Int,D>, CartesianVector<Real,D> >
    Grid<D>::getCellPair(const CVecRD& xyz,
                         const bool approx,
                         const Real tol,
                         bool* err) const {
    if (err != NULL) {
        *err = false;
    }
    bool stepErr = false;

    CVecID cell;
    CVecRD dist;
    for (Int dir = 0; dir < D; dir++) {
        pair<Int, Real> res = getCellPair(dir,xyz(dir),approx,tol,&stepErr);
        cell(dir) = res.first;
        dist(dir) = res.second;
        if (err != NULL) {
            *err = *err || stepErr;
        }
    }
    return make_pair(cell, dist);
}

template<Int D>
CVecI3Fractional Grid<D>::getCVecI3Fractional (const CVecRD& xyz,
                                               bool* err) const{
    long int n; n = 0;
    CVecI3 ijk_;
    CVecR3 len_ = 0.0;

    for(UInt dir=0; dir<D; ++dir){
        if(xyz[dir]<pos_[dir].front()){
            ijk_[dir] = 0;
            *err = false;
        }else if(pos_[dir].back()<=xyz[dir]) {
            ijk_[dir] = pos_[dir].size()+offsetGrid_[dir];
            *err = false;
        }
        while(pos_[dir][dir] <= xyz[dir]){
            ++n;
        }
        ijk_[dir] = n-1;
        len_[dir] = (xyz[dir]-pos_[dir][ijk_[dir]])/getStep(dir,ijk_[dir]);
    }
    return CVecI3Fractional (ijk_,len_);
}

template<Int D>
Int Grid<D>::getCell(const Int dir,
                     const Real x,
                     const bool approx,
                     const Real tol,
                     bool* err) const {
    return getCellPair(dir, x, approx, tol, err).first;
}

template<Int D>
CartesianVector<Int,D> Grid<D>::getCell(const CVecRD &coords,
                                        const bool approx,
                                        const Real tol,
                                        bool* err) const {
    return getCellPair(coords, approx, tol, err).first;
}

template<Int D>
void Grid<D>::applyScalingFactor(const Real factor) {
    origin_ *= factor;
    for (Int d = 0; d < D; d++) {
        for (UInt i = 0; i < pos_[d].size(); i++) {
            pos_[d][i] *= factor;
        }
    }
}

template<Int D>
void Grid<D>::enlarge(const pair<CVecID, CVecID>& additionalCells,
                      const pair<CVecRD, CVecRD>& sizesOfNewCells) {
    // TODO
    //    const vector<Real> pos = grid.getPos(dir);
    //    const vector<Real> step = grid.getStep(dir);
    //    if ((padding.first(dir) != 0.0 && finalSize.first(dir) == 0.0)
    //            || (padding.second(dir) != 0.0 && finalSize.second(dir) == 0.0)) {
    //        cerr << endl << "WARNING @ writeRectilinearSpaceStep: "
    //                << "Boundary final size can not be 0.0. "
    //                << "Defaulting to first step size." << endl;
    //    }
    //    if (finalSize.first(dir) == 0.0) {
    //        finalSize.first(dir) = *step.begin();
    //    }
    //    if (finalSize.second(dir) == 0.0) {
    //        finalSize.second(dir) = step.back();
    //    }
    //    const Int nCellsLowest
    //    = abs(padding.first(dir)) / abs(finalSize.first(dir));
    //    const Int nCellsHighest
    //    = abs(padding.second(dir)) / abs(finalSize.second(dir));
    //    // Writes data.
    //    SmbData::SpaceSteps* res = &nfde.spacesteps[dir];
    //    res->m = - nCellsLowest;
    //    res->n = grid.getNumCells()(dir) + nCellsLowest + nCellsHighest;
    //    res->d.resize(res->n);
    //    for (Int i = 0; i < nCellsLowest; i++) {
    //        res->d[i] = finalSize.first(dir);
    //    }
    //    for (Int i = 0; i < (grid.getNumCells()(dir)); i++) {
    //        res->d[i + nCellsLowest] = step[i];
    //    }
    //    for (Int i = 0; i < nCellsHighest; i++) {
    //        res->d[i + nCellsLowest + grid.getNumCells()(dir)]
    //               = finalSize.second(dir);
    //    }
    //    res->origin = grid.getOrigin()(dir);
    //    res->cons = false;
    //}
}


template<Int D>
void Grid<D>::printInfo() const {
    CVecID numCells = getNumCells();
    BoxRD bound = getFullDomainBoundingBox();
    cout << "-- Cartesian Grid<" << D << "> --" << endl;
    cout << "Dims: " << numCells.toStr() << endl;
    cout << "Min val: " << bound.getMin().toStr() << endl;
    cout << "Max val: " << bound.getMax().toStr() << endl;
}

template class Grid<1>;
template class Grid<2>;
template class Grid<3>;
