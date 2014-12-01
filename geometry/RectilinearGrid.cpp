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
	offset_ = grid.offset_;
	originGrid_ = grid.originGrid_;
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
	offset_ = boundingBox.get_min() - dxyz * 4.0;
	originGrid_ = CVecI3(0, 0, 0);
	generatePositionsGrid();
}

RectilinearGrid::RectilinearGrid(
 const BoundingBox &boundingBox, const CVecI3& dims) {
	for (int i = 0; i < 3; i++) {
		double step =
		 (boundingBox.get_max()(i) - boundingBox.get_min()(i)) / dims(i);
		step_[i].resize(dims(i), step);
	}
	offset_ = boundingBox.get_min();
	originGrid_ = CVecI3(0, 0, 0);
	generatePositionsGrid();
}

RectilinearGrid::~RectilinearGrid(){
}

//======== tools methods =======================================================
void RectilinearGrid::generatePositionsGrid(){
	CVecI3 dims = getNumCells();
	for (uint dir = 0; dir < 3; dir++) {
		pos_[dir].resize(dims(dir) + 1);
		pos_[dir][0] = offset_(dir);
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
		ijk=0;
		return false;
	}
	assert(pos_[dir].size() >= 1);
	for (uint i = 1; i < pos_[dir].size(); i++) {
		if (abs(getPos(dir)[i] - xyz) < tolerance) {
			ijk = i;
			relativeLen = 0.0;
			return true;
		} else if (getPos(dir)[i] > xyz) {
			ijk = i-1;
			double pos = getPos(dir)[ijk];
			double step = getStep(dir)[ijk];
			relativeLen = (xyz - pos) / step;
			return true;
		}
		if (i == pos_[dir].size()-1) {
			ijk = getPos(dir).size() - 1;
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
		return pair<CVecI3, CVecD3> (CVecI3(-1,-1,-1), CVecD3());
	}
	if(res.second(x) > 0.5 && approx) {
		res.first(x)++;
	}
	if(!getNaturalCelly(xyz(y), res.first(y), res.second(y))) {
		return pair<CVecI3, CVecD3> (CVecI3(-1,-1,-1), CVecD3());
	}
	if(res.second(y) > 0.5 && approx) {
		res.first(y)++;
	}
	if(!getNaturalCellz(xyz(z), res.first(z), res.second(z))) {
		return pair<CVecI3, CVecD3> (CVecI3(-1,-1,-1), CVecD3());
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
	offset_ = rhs.offset_;
	originGrid_ = rhs.originGrid_;
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

double RectilinearGrid::getPositionOfNaturalCelli(long int i) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(i,0,0));
	return coords(0);
}

double RectilinearGrid::getPositionOfNaturalCellj(long int j) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(0,j,0));
	return coords(1);
}

double RectilinearGrid::getPositionOfNaturalCellk(long int k) const {
	CVecD3 coords = getPositionOfNaturalCell(CVecI3(0,0,k));
	return coords(2);
}

CVecD3
RectilinearGrid::getPositionOfNaturalCell(const CVecI3 &coords) const {
	return getPos(coords);
}

bool
RectilinearGrid::isNaturalCell(const CVecD3 position) const {
	pair<CVecI3, CVecD3> natCell = getNaturalCellPair(position);
	bool res = true;
	for (uint i = 0; i < 3; i++) {
		if (natCell.second(i) < 0.5) {
			res &= natCell.second(i) < tolerance;
		} else {
			res &= (1.0 - natCell.second(i)) < tolerance;
		}
	}
	return res;
}

bool
RectilinearGrid::isNaturalCell(const vector<CVecD3> pos) const {
	for (uint i = 0; i < pos.size(); i++) {
		if (!isNaturalCell(pos[i])) {
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
	offset_ *= factor;
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
//==============================================================================
//   Develop: Miguel D. Ruiz - Cabello N. (University of Granada)
//==============================================================================

