//============================================================================//
//   Title: confMeher                                                         //
//   Author: Miguel D. Ruiz - Cabello N.                                      //
//============================================================================//
// HYSTORY & VERSION:                                                         //
//    DATE beginning: 11.12.2013                                              //
//============================================================================//
#include "RectilinearGrid.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%   CARTESIAN GRID  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//======== constructors ========================================================
RectilinearGrid::RectilinearGrid(){
}

RectilinearGrid::RectilinearGrid(const RectilinearGrid& grid) {
	origin_ = grid.origin_;
	offsetGrid_ = grid.offsetGrid_;
	for (int i = 0; i < 3; i++) {
		step_[i] = grid.step_[i];
		pos_[i] = grid.pos_[i];
	}
	generatePositionsGrid();
}

RectilinearGrid::RectilinearGrid(
 const BoundingBox& boundingBox,
 const CVecD3& dxyz){
	for (int i = 0; i < 3; i++) {
		int dim =
		 ceil((boundingBox.get_max()(i)-boundingBox.get_min()(i))/dxyz(i))+8;
		pos_[i].resize(dim);
		step_[i].resize(dim);
	}
	origin_ = boundingBox.get_min() - dxyz * 4.0;
	offsetGrid_ = CVecI3(0, 0, 0);
	generatePositionsGrid();
}

RectilinearGrid::RectilinearGrid(
 const BoundingBox &boundingBox, const CVecI3& dims) {
	for (int i = 0; i < 3; i++) {
		double step =
		 (boundingBox.get_max()(i) - boundingBox.get_min()(i)) / dims(i);
		step_[i].resize(dims(i), step);
	}
	origin_ = boundingBox.get_min();
	offsetGrid_ = CVecI3(0, 0, 0);
	generatePositionsGrid();
}

RectilinearGrid::RectilinearGrid(const CVecI3& offset,
                                 const CVecD3& origin,
                                 const vector<double> step[]) {

   origin_ = origin;
   offsetGrid_ = offset;
   for(int d = 0; d < 3; d++)
      step_[d] = step[d];
   generatePositionsGrid();
}

RectilinearGrid::~RectilinearGrid(){
}

//======== tools methods =======================================================
void RectilinearGrid::generatePositionsGrid(){
	CVecI3 dims = getNumCells();
	for (uint dir = 0; dir < 3; dir++) {
		pos_[dir].resize(dims(dir) + 1);
		pos_[dir][0] = origin_(dir);
		for (long int n=1; n<dims(dir)+1; n++){
			pos_[dir][n] = pos_[dir][n-1] + step_[dir][n-1];
		}
	}

}
//==============================================================================
bool RectilinearGrid::isIntoDirX (const double &dir)const{
	return isIntoDir(x, dir);
}

bool RectilinearGrid::isIntoDirY (const double &dir)const{
	return isIntoDir(y, dir);
}

bool RectilinearGrid::isIntoDirZ (const double &dir)const{
	return isIntoDir(z, dir);
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

//==============================================================================
bool RectilinearGrid::getNaturalCellx
		 (const double &posX,long int &i, double &relativeLen) const{
	return getNaturalCellDir(i, relativeLen, x, posX);
}

bool RectilinearGrid::getNaturalCelly
		 (const double &posY,long int &j, double &relativeLen) const{
	return getNaturalCellDir(j, relativeLen, y, posY);
}

bool RectilinearGrid::getNaturalCellz
		 (const double &posZ,long int &k, double &relativeLen) const{
	return getNaturalCellDir(k, relativeLen, z, posZ);
}

bool RectilinearGrid::getNaturalCellDir(
 long int &ijk, double &relativeLen, const int& dir, const double &xyz) const {
	relativeLen = -1.0;
	if(xyz < getPos(dir)[0]){
		ijk=offsetGrid_(dir);
		return false;
	}
	assert(pos_[dir].size() >= 1);
	for (uint i = 1; i < pos_[dir].size(); i++) {
		if (abs(getPos(dir)[i] - xyz) < tolerance) {
			ijk = i+offsetGrid_(dir);
			relativeLen = 0.0;
			return true;
		} else if (getPos(dir)[i] > xyz) {
			ijk = i+offsetGrid_(dir)-1;
			double pos = getPos(dir)[ijk];
			double step = getStep(dir)[ijk];
			relativeLen = (xyz - pos) / step;
			return true;
		}
		if (i == pos_[dir].size()-1) {
			ijk = getPos(dir).size() - 1 + offsetGrid_(dir);
			return false;
		}
	}
	return true;
}

//========= general setters ====================================================
pair<CVecI3, CVecD3>
RectilinearGrid::getNaturalCellPair(
		const CVecD3& xyz,
		const bool approx) const {
	pair<CVecI3, CVecD3> res;
	if(!getNaturalCellx(xyz(x), res.first(x), res.second(x))) {
		return make_pair(CVecI3(-1,-1,-1)+offsetGrid_, CVecD3());
	}
	if(res.second(x) > 0.5 && approx) {
		res.first(x)++;
	}
	if(!getNaturalCelly(xyz(y), res.first(y), res.second(y))) {
		return make_pair(CVecI3(-1,-1,-1)+offsetGrid_, CVecD3());
	}
	if(res.second(y) > 0.5 && approx) {
		res.first(y)++;
	}
	if(!getNaturalCellz(xyz(z), res.first(z), res.second(z))) {
		return make_pair(CVecI3(-1,-1,-1)+offsetGrid_, CVecD3());
	}
	if(res.second(z) > 0.5 && approx) {
		res.first(z)++;
	}
	return res;
}

CVecI3 RectilinearGrid::getNaturalCell(
 const CVecD3 &coords,
 const bool approx) const {
	return getNaturalCellPair(coords, approx).first;
}

BoundingBox
RectilinearGrid::getBoundingBox(
 const pair<CVecI3, CVecI3>& ijk) const {
	return BoundingBox(make_pair(getPos(ijk.first), getPos(ijk.second)));
}

RectilinearGrid& RectilinearGrid::operator =(const RectilinearGrid& rhs) {
	if (this == &rhs) {
		return *this;
	}
	origin_ = rhs.origin_;
	offsetGrid_ = rhs.offsetGrid_;
	for (int i = 0; i < 3; i++) {
		step_[i] = rhs.step_[i];
		pos_[i] = rhs.pos_[i];
	}
	return *this;
}

bool RectilinearGrid::isRegular() const {
	double step;
	CVecI3 dim = getNumCells();
	for (int i = 0; i < 3; i++) {
		step = step_[i][0];
		for (long n = 0; n < dim(i); n++) {
			if (step_[i][n] != step) {
				return false;
			}
		}
	}
	return true;
}

double RectilinearGrid::getMinimumSpaceStep() const {
	double minStep = step_[0][0];
	CVecI3 dim = getNumCells();
	for (int i = 0; i < 3; i++) {
		for (long n = 0; n < dim(i); n++) {
			if (step_[i][n] < minStep) {
				minStep = step_[i][n];
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

double RectilinearGrid::getPositionOfNaturalCellDir(const int dir,
                                                    long int i) const {
   
   switch(dir) {
   case x:
      return getPositionOfNaturalCelli(i);
   case y:
      return getPositionOfNaturalCellj(i);
   case z:
      return getPositionOfNaturalCellk(i);
   }
   return 0.0;
}

double RectilinearGrid::getPositionOfNaturalCelli(long int i) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(i,
                                                   offsetGrid_(y),
                                                   offsetGrid_(z)));
	return coords(0);
}

double RectilinearGrid::getPositionOfNaturalCellj(long int j) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(offsetGrid_(x),
                                                   j,
                                                   offsetGrid_(z)));
	return coords(1);
}

double RectilinearGrid::getPositionOfNaturalCellk(long int k) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(offsetGrid_(x),
                                                   offsetGrid_(y),
                                                   k));
	return coords(2);
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
	for (uint i = 0; i < 3; i++) {
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

void RectilinearGrid::applyScalingFactorToVector(
		double *vec,
		const uint dim,
		const double factor) {
	for (uint i = 0; i < dim; i++) {
		vec[i] *= factor;
	}
}

void RectilinearGrid::applyScalingFactor(const double factor) {
	origin_ *= factor;
	for (int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < step_[i].size(); j++) {
			step_[i][j] *= factor;
 		}
	}
	generatePositionsGrid();
}

bool RectilinearGrid::hasZeroSize() const {
	bool res = true;
	for (uint i = 0; i < 3; i++) {
		res &= (pos_[i].size() <= 1);
	}
	return res;
}

vector<CVecD3>
RectilinearGrid::getCenterOfNaturalCellsInside(
        const BoundingBox& bound) const {
    // Determines centers of cells.
    vector<double> center[3];
    for (uint dir = 0; dir < 3; dir++) {
        vector<double> pos
         = getPosInRange(dir, bound.get_min()(dir), bound.get_max()(dir));
        center[dir].reserve(pos.size()-1);
        for (uint i = 1; i < pos.size(); i++) {
            double auxCenter = (pos[i-1] + pos[i]) / 2.0;
            center[dir].push_back(auxCenter);
        }
    }
    // Combines centers in a vector of positions.
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
