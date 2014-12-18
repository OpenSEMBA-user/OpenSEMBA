#include "RectilinearGrid.h"

RectilinearGrid::RectilinearGrid(){
}

RectilinearGrid::RectilinearGrid(const RectilinearGrid& grid) {
	origin_ = grid.origin_;
	offsetGrid_ = grid.offsetGrid_;
	for (int i = 0; i < D; i++) {
		pos_[i] = grid.pos_[i];
	}
}

RectilinearGrid::RectilinearGrid(
 const BoundingBox& box,
 const CVecD3& dxyz){
    origin_ = box.getMin();
    offsetGrid_ = CVecI3(0, 0, 0);
    for (int i = 0; i < D; i++) {
	    double boxLength = box.getMax()(i) - box.getMin()(i);
	    int nCells = ceil(boxLength / dxyz(i));
	    if ((boxLength - nCells * dxyz(i)) > tolerance) {
	        nCells++;
	    }
		pos_[i].resize(nCells+1, dxyz(i));
	}
}

RectilinearGrid::RectilinearGrid(
 const BoundingBox &boundingBox,
 const CVecI3& dims) {
    origin_ = boundingBox.getMin();
    offsetGrid_ = CVecI3(0, 0, 0);
	for (int i = 0; i < D; i++) {
		double step =
		 (boundingBox.getMax()(i) - boundingBox.getMin()(i)) / dims(i);
		pos_[i].resize(dims(i)+1, step);
	}
}

RectilinearGrid::RectilinearGrid(
        const CVecI3& offset,
        const CVecD3& origin,
        const vector<double> step[D]) {
   origin_ = origin;
   offsetGrid_ = offset;
   for(int d = 0; d < D; d++) {
       pos_[d].resize(step[d].size()+1);
       pos_[d][0] = origin(d);
       for (uint i = 1; i < pos_[d].size(); i++) {
           pos_[d][i] = pos_[d][i-1] + step[d][i];
       }
   }
}

RectilinearGrid::~RectilinearGrid(){
}

RectilinearGrid&
RectilinearGrid::operator =(const RectilinearGrid& rhs) {
	if (this == &rhs) {
		return *this;
	}
	origin_ = rhs.origin_;
	offsetGrid_ = rhs.offsetGrid_;
	for (int i = 0; i < D; i++) {
		pos_[i] = rhs.pos_[i];
	}
	return *this;
}

inline vector<double>
RectilinearGrid::getPos(const int& direction) const {
    assert(direction >= 0 && direction < D);
    return pos_[direction];
};

inline CVecD3
RectilinearGrid::getPos(const CVecI3& ijk) const {
    CVecI3 dims = getNumCells();
    for (int i = 0; i < D; i++) {
        assert((ijk(i) >= offsetGrid_(i)) &&
                (ijk(i) <= offsetGrid_(i)+dims(i)));
    }
    return CVecD3(getPos(x)[ijk(x)-offsetGrid_(x)],
            getPos(y)[ijk(y)-offsetGrid_(y)],
            getPos(z)[ijk(z)-offsetGrid_(z)]);
};

inline CVecI3
RectilinearGrid::getNumCells() const {
    return CVecI3(pos_[x].size()-1,pos_[y].size()-1, pos_[z].size()-1);
}

vector<double>
RectilinearGrid::getStep(const int dir) const {
    assert(dir >= 0 && dir < D);
    vector<double> res(pos_[dir].size()-1);
    for (uint i = 0; i < pos_[dir].size()-1; i++) {
        res[i] = pos_[dir][i+1] - pos_[dir][i];
    }
    return res;
}

bool RectilinearGrid::isIntoDir(
 const int& direction, const double& pos) const {
	if (pos >= getPos(direction)[0] && pos <= getPos(direction).back()) {
		return true;
	}
	return false;
}

bool RectilinearGrid::isInto (const CVecD3& pos) const {
	for (int i = 0; i < 3; i++) {
		if (!isIntoDir(i, pos(i))) {
			return false;
		}
	}
	return true;
}

bool RectilinearGrid::getNaturalCellDir(
 long int &ijk,
 double &relativeLen,
 const int& dir,
 const double &xyz,
 const double tol) const {
	relativeLen = -1.0;
	assert(getPos(dir).size() >= 1);
	double diff = abs(getPos(dir)[0] - xyz);
	if(diff > tol && xyz < getPos(dir)[0]){
		ijk=offsetGrid_(dir);
		return false;
	}
	for (uint i = 0; i < pos_[dir].size(); i++) {
	    diff = abs(getPos(dir)[i] - xyz);
        if (diff < tol) {
			ijk = i + offsetGrid_(dir);
			relativeLen = 0.0;
			return true;
		} else if (getPos(dir)[i] > xyz) {
			ijk = i - 1 + offsetGrid_(dir);
			double pos = getPos(dir)[ijk];
			double step = getStep(dir)[ijk];
			relativeLen = (xyz - pos) / step;
			return true;
		}
	}
	return false;
}

pair<CVecI3, CVecD3>
RectilinearGrid::getNaturalCellPair(
		const CVecD3& xyz,
		const bool approx,
		const double tol) const {
	pair<CVecI3, CVecD3> res;
	for (int dir = 0; dir < D; dir++) {
	    if(!getNaturalCellDir(res.first(dir), res.second(dir), dir, xyz(dir), tol)) {
	        return make_pair(CVecI3(-1,-1,-1), CVecD3());
	    }
	    if(res.second(dir) > 0.5 && approx) {
	        res.first(dir)++;
	    }
	}
	return res;
}

CVecI3 RectilinearGrid::getNaturalCell(
 const CVecD3 &coords,
 const bool approx,
 const double tol) const {
	return getNaturalCellPair(coords, approx, tol).first;
}

BoundingBox
RectilinearGrid::getBoundingBox(
 const pair<CVecI3, CVecI3>& ijk) const {
	return BoundingBox(make_pair(getPos(ijk.first), getPos(ijk.second)));
}

inline BoundingBox
RectilinearGrid::getFullDomainBoundingBox() const {
    return getBoundingBox(
            pair<CVecI3,CVecI3> (offsetGrid_, offsetGrid_+getNumCells()) );
}

bool
RectilinearGrid::isRegular() const {
	for (int i = 0; i < D; i++) {
	    vector<double> step = getStep(i);
	    for (uint n = 1; n < step.size(); n++) {
			if (step[n] != step[0]) {
				return false;
			}
		}
	}
	return true;
}

bool
RectilinearGrid::isCartesian() const {
    double canon = getStep(0)[0];
    for (int i = 0; i < D; i++) {
        vector<double> step = getStep(i);
        for (uint n = 1; n < step.size(); n++) {
            if (step[n] != canon) {
                return false;
            }
        }
    }
    return true;
}

double
RectilinearGrid::getMinimumSpaceStep() const {
	double minStep = numeric_limits<double>::infinity();
	for (int i = 0; i < D; i++) {
	    vector<double> step = getStep(i);
		for (uint n = 0; n < step.size(); n++) {
			if (step[n] < minStep) {
				minStep = step[n];
			}
		}
	}
	return minStep;
}

void RectilinearGrid::printInfo() const {
	CVecI3 numCells = getNumCells();
	CVecD3 minPos = getPos(CVecI3(0,0,0));
	CVecD3 maxPos = getPos(getNumCells());
	cout << "-- Cartesian Grid --" << endl;
	cout << "Dims: " << numCells(0) << " " << numCells(1) << " " << numCells(2) << endl;
	cout << "Min val: " << minPos(0) << " " << minPos(1) << " " << minPos(2) << endl;
	cout << "Max val: " << maxPos(0) << " " << maxPos(1) << " " << maxPos(2) << endl;
}

double
RectilinearGrid::getPositionOfNaturalCellDir(
        const int dir,
        long int i) const {
    return getPos(dir)[i];
}

CVecD3
RectilinearGrid::getPositionOfNaturalCell(const CVecI3 &coords) const {
	return getPos(coords);
}

bool
RectilinearGrid::isNaturalCell(const CVecD3 position,
 const double tol) const {
	pair<CVecI3, CVecD3> natCell = getNaturalCellPair(position);
	bool res = true;
	for (int i = 0; i < D; i++) {
		if (natCell.second(i) < 0.5) {
			res &= natCell.second(i) < tol;
		} else {
			res &= (1.0 - natCell.second(i)) < tol;
		}
	}
	return res;
}

bool
RectilinearGrid::isNaturalCell(const vector<CVecD3> pos,
 const double tol) const {
	for (uint i = 0; i < pos.size(); i++) {
		if (!isNaturalCell(pos[i], tol)) {
			return false;
		}
	}
	return true;
}

vector<double>
RectilinearGrid::getPosInRange(
 const int dir,
 const double min,
 const double max) const {
	return extractRange(getPos(dir), pair<double, double>(min, max));
}

void
RectilinearGrid::applyScalingFactor(const double factor) {
	origin_ *= factor;
	for (int d = 0; d < D; d++) {
	    for (uint i = 0; i < pos_[d].size(); i++) {
	        pos_[d][i] *= factor;
	    }
	}
}

bool
RectilinearGrid::hasZeroSize() const {
	bool res = true;
	for (int i = 0; i < D; i++) {
		res &= (pos_[i].size() <= 1);
	}
	return res;
}

vector<CVecD3>
RectilinearGrid::getCenterOfNaturalCellsInside(
        const BoundingBox& bound) const {
    // Determines centers of cells.
    vector<double> center[D];
    for (int dir = 0; dir < D; dir++) {
        vector<double> pos
         = getPosInRange(dir, bound.getMin()(dir), bound.getMax()(dir));
        if (pos.size() > 0) {
            center[dir].reserve(pos.size()-1);
        }
        for (uint i = 1; i < pos.size(); i++) {
            double auxCenter = (pos[i-1] + pos[i]) / 2.0;
            center[dir].push_back(auxCenter);
        }
    }
    // Combines centers in a vector of CVecD3 positions.
    vector<CVecD3> res;
    res.reserve(center[x].size() * center[y].size() * center[z].size());
    for (uint i = 0; i < center[x].size(); i++) {
        for (uint j = 0; j < center[y].size(); j++) {
            for (uint k = 0; k < center[z].size(); k++) {
                res.push_back(CVecD3(center[x][i], center[y][j], center[z][k]));
            }
        }
    }
    return res;
}

BoundingBox
RectilinearGrid::getBoundingBoxContaining(
        const CVecD3& point) const {
    CVecI3 naturalMin = getNaturalCell(point, false);
    CVecD3 min = getPositionOfNaturalCell(naturalMin);
    CVecD3 max = getPositionOfNaturalCell(naturalMin + (long int) 1);
    return BoundingBox(min, max);
}

vector<CVecD3>
RectilinearGrid::getPos() const {
    // Combines positions in a vector of CVecD3 positions.
    vector<CVecD3> res;
    res.reserve(pos_[x].size() * pos_[y].size() * pos_[z].size());
    for (uint i = 0; i < pos_[x].size(); i++) {
        for (uint j = 0; j < pos_[y].size(); j++) {
            for (uint k = 0; k < pos_[z].size(); k++) {
                res.push_back(CVecD3(pos_[x][i], pos_[y][j], pos_[z][k]));
            }
        }
    }
    return res;
}

vector<double>
RectilinearGrid::extractRange(
 const vector<double>& vec,
 const pair<double, double>& minMax) const {
	vector<double> res;
	res.reserve(vec.size());
	for (uint i = 0; i < vec.size(); i++) {
		if ((abs(vec[i] - minMax.first) < tolerance)
		 || (vec[i] >= minMax.first && vec[i] <= minMax.second)
		 || (abs(vec[i] - minMax.second) < tolerance)) {
			res.push_back(vec[i]);
		}
	}
	return res;
}
