#include "Grid.h"

template<Int D>
Grid<D>::Grid(){
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
Grid<D>::Grid(
      const BoxDD& box,
      const CVecR3& dxyz){
   origin_ = box.getMin();
   offsetGrid_ = CVecI3(0, 0, 0);
   for (Int i = 0; i < D; i++) {
      Real boxLength = box.getMax()(i) - box.getMin()(i);
      Int nCells = ceil(boxLength / dxyz(i));
      if ((boxLength - nCells * dxyz(i)) > tolerance) {
         nCells++;
      }
      pos_[i].resize(nCells+1);
      for (Int j = 0; j < nCells+1; j++) {
         pos_[i][j] = origin_(i) + j * dxyz(i);
      }
   }
}

template<Int D>
Grid<D>::Grid(
      const BoxDD &boundingBox,
      const CVecI3& dims) {
   origin_ = boundingBox.getMin();
   offsetGrid_ = CVecI3(0, 0, 0);
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
Grid<D>::Grid(
      const CVecI3& offset,
      const CVecR3& origin,
      const vector<Real> step[D]) {
   origin_ = origin;
   offsetGrid_ = offset;
   for(Int d = 0; d < D; d++) {
      pos_[d].resize(step[d].size()+1);
      pos_[d][0] = origin(d);
      for (UInt i = 1; i < pos_[d].size(); i++) {
         pos_[d][i] = pos_[d][i-1] + step[d][i];
      }
   }
}

template<Int D>
Grid<D>::~Grid(){
}

template<Int D>
Grid<D>& Grid<D>::operator =(const Grid<D>& rhs) {
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
inline vector<Real>
Grid<D>::getPos(const Int& direction) const {
   assert(direction >= 0 && direction < D);
   return pos_[direction];
};

template<Int D>
inline CartesianVector<Real,D>
Grid<D>::getPos(const CartesianVector<Int,D>& ijk) const {
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
inline CVecI3 Grid<D>::getNumCells() const {
   return CVecI3(pos_[x].size()-1,pos_[y].size()-1, pos_[z].size()-1);
}

template<Int D>
vector<Real> Grid<D>::getStep(const CartesianAxis dir) const {
   assert(dir >= 0 && dir < D);
   vector<Real> res(pos_[dir].size()-1);
   for (UInt i = 0; i < pos_[dir].size()-1; i++) {
      res[i] = pos_[dir][i+1] - pos_[dir][i];
   }
   return res;
}

template<Int D>
bool Grid<D>::isIntoDir(
      const Int& direction, const Real& pos) const {
   if (pos >= getPos(direction)[0] && pos <= getPos(direction).back()) {
      return true;
   }
   return false;
}

template<Int D>
bool Grid<D>::isInto (const CVecR3& pos) const {
   for (Int i = 0; i < 3; i++) {
      if (!isIntoDir(i, pos(i))) {
         return false;
      }
   }
   return true;
}

template<Int D>
bool
Grid<D>::getNaturalCellDir(
      Int &ijk,
      Real &relativeLen,
      const Int& dir,
      const Real &xyz,
      const Real tol) const {
   relativeLen = -1.0;
   assert(getPos(dir).size() >= 1);
   // Checks if it is below the grid.
   Real diff = abs(getPos(dir)[0] - xyz);
   if(diff > tol && xyz < getPos(dir)[0]){
      ijk=offsetGrid_(dir);
      return false;
   }
   for (UInt i = 0; i < pos_[dir].size(); i++) {
      bool same = abs(getPos(dir)[i] - xyz) < tol;
      if (same) {
         ijk = i + offsetGrid_(dir);
         relativeLen = 0.0;
         return true;
      } else if (getPos(dir)[i] > xyz) {
         ijk = i - 1 + offsetGrid_(dir);
         Real pos = getPos(dir)[ijk];
         Real step = getStep(dir)[ijk];
         relativeLen = (xyz - pos) / step;
         return true;
      }
   }
   return false;
}

template<Int D>
pair<CVecI3, CVecR3>
Grid<D>::getNaturalCellPair(
      const CVecR3& xyz,
      const bool approx,
      const Real tol) const {
   pair<CVecI3, CVecR3> res;
   for (Int dir = 0; dir < D; dir++) {
      if(!getNaturalCellDir(res.first(dir), res.second(dir), dir, xyz(dir), tol)) {
         return make_pair(CVecI3(-1,-1,-1), CVecR3());
      }
      if(res.second(dir) > 0.5 && approx) {
         res.first(dir)++;
      }
   }
   return res;
}

template<Int D>
CVecI3 Grid<D>::getNaturalCell(
      const CVecR3 &coords,
      const bool approx,
      const Real tol) const {
   return getNaturalCellPair(coords, approx, tol).first;
}

template<Int D>
inline Box<Real,D>
Grid<D>::getBoundingBox(
      const pair<CVecI3, CVecI3>& ijk) const {
   BoxDD res(getPos(ijk.first), getPos(ijk.second));
   return res;
}

template<Int D>
inline Box<Real,D>
Grid<D>::getFullDomainBoundingBox() const {
   return getBoundingBox(
         pair<CVecI3,CVecI3> (offsetGrid_, offsetGrid_+getNumCells()) );
}

template<Int D>
bool
Grid<D>::isRegular() const {
   for (Int i = 0; i < D; i++) {
      if (!isRegular(i)) {
          return false;
      }
   }
   return true;
}

template<Int D>
bool
Grid<D>::isRegular(const Int d) const {
    vector<Real> step = getStep(d);
    for (UInt n = 1; n < step.size(); n++) {
        if (abs(step[n] - step[0]) > tolerance) {
            return false;
        }
    }
   return true;
}

template<Int D>
bool
Grid<D>::isCartesian() const {
   Real canon = getStep(0)[0];
   for (Int i = 0; i < D; i++) {
      vector<Real> step = getStep(i);
      for (UInt n = 1; n < step.size(); n++) {
         if (step[n] != canon) {
            return false;
         }
      }
   }
   return true;
}

template<Int D>
Real
Grid<D>::getMinimumSpaceStep() const {
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
void Grid<D>::printInfo() const {
   CVecI3 numCells = getNumCells();
   BoxDD bound = getFullDomainBoundingBox();
   cout << "-- Cartesian Grid<" << D << "> --" << endl;
   cout << "Dims: " << numCells.toStr() << endl;
   cout << "Min val: " << bound.getMin().toStr() << endl;
   cout << "Max val: " << bound.getMax().toStr() << endl;
}

template<Int D>
Real
Grid<D>::getPositionOfNaturalCellDir(
      const Int dir,
      Int i) const {
   return getPos(dir)[i];
}

template<Int D>
CVecR3
Grid<D>::getPositionOfNaturalCell(const CVecI3 &coords) const {
   return getPos(coords);
}

template<Int D>
bool
Grid<D>::isNaturalCell(const CVecR3 position,
      const Real tol) const {
   pair<CVecI3, CVecR3> natCell = getNaturalCellPair(position);
   bool res = true;
   for (Int i = 0; i < D; i++) {
      if (natCell.second(i) < 0.5) {
         res &= natCell.second(i) < tol;
      } else {
         res &= (1.0 - natCell.second(i)) < tol;
      }
   }
   return res;
}

template<Int D>
bool Grid<D>::isNaturalCell(const vector<CVecR3> pos,
      const Real tol) const {
   for (UInt i = 0; i < pos.size(); i++) {
      if (!isNaturalCell(pos[i], tol)) {
         return false;
      }
   }
   return true;
}

template<Int D>
vector<Real> Grid<D>::getPosInRange(
      const CartesianAxis dir,
      const Real min,
      const Real max) const {
   return extractRange(getPos(dir), pair<Real, Real>(min, max));
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
bool Grid<D>::hasZeroSize() const {
   bool res = true;
   for (Int i = 0; i < D; i++) {
      res &= (pos_[i].size() <= 1);
   }
   return res;
}

template<Int D>
vector<CVecR3> Grid<D>::getCenterOfNaturalCellsInside(
      const BoxDD& bound) const {
   // Determines centers of cells.
   vector<Real> center[D];
   for (Int dir = 0; dir < D; dir++) {
      vector<Real> pos
      = getPosInRange(dir, bound.getMin()(dir), bound.getMax()(dir));
      if (pos.size() > 0) {
         center[dir].reserve(pos.size()-1);
      }
      for (UInt i = 1; i < pos.size(); i++) {
         Real auxCenter = (pos[i-1] + pos[i]) / 2.0;
         center[dir].push_back(auxCenter);
      }
   }
   // Combines centers in a vector of CVecR3 positions.
   vector<CVecR3> res;
   res.reserve(center[x].size() * center[y].size() * center[z].size());
   for (UInt i = 0; i < center[x].size(); i++) {
      for (UInt j = 0; j < center[y].size(); j++) {
         for (UInt k = 0; k < center[z].size(); k++) {
            res.push_back(CVecR3(center[x][i], center[y][j], center[z][k]));
         }
      }
   }
   return res;
}

template<Int D>
Box<Real,D> Grid<D>::getBoundingBoxContaining(
      const CVecR3& point) const {
   CVecI3 naturalMin = getNaturalCell(point, false);
   CVecR3 min = getPositionOfNaturalCell(naturalMin);
   CVecR3 max = getPositionOfNaturalCell(naturalMin + (Int) 1);
   return BoxDD(min, max);
}

template<Int D>
vector<CVecR3> Grid<D>::getPos() const {
   // Combines positions in a vector of CVecR3 positions.
   vector<CVecR3> res;
   res.reserve(pos_[x].size() * pos_[y].size() * pos_[z].size());
   for (UInt i = 0; i < pos_[x].size(); i++) {
      for (UInt j = 0; j < pos_[y].size(); j++) {
         for (UInt k = 0; k < pos_[z].size(); k++) {
            res.push_back(CVecR3(pos_[x][i], pos_[y][j], pos_[z][k]));
         }
      }
   }
   return res;
}

template<Int D>
vector<Real> Grid<D>::extractRange(
      const vector<Real>& vec,
      const pair<Real, Real>& minMax) const {
   vector<Real> res;
   res.reserve(vec.size());
   for (UInt i = 0; i < vec.size(); i++) {
      if ((abs(vec[i] - minMax.first) < tolerance)
            || (vec[i] >= minMax.first && vec[i] <= minMax.second)
            || (abs(vec[i] - minMax.second) < tolerance)) {
         res.push_back(vec[i]);
      }
   }
   return res;
}
