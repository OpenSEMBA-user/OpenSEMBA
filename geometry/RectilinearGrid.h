#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <vector>
#include "BoundingBox.h"
#include "Box.h"

#ifdef compileWithVTK
#include "../../utils/ugrMesher/vtkLittle.h"
#endif

using namespace std;

#ifndef  _rectilinearGrid_hxx
# define _rectilinearGrid_hxx

class RectilinearGrid {
public:
    static const int D = 3;
    RectilinearGrid();
    RectilinearGrid(const RectilinearGrid& grid);
    RectilinearGrid(
            const BoundingBox &boundingBox,
            const CVecD3& dxyz);
    RectilinearGrid(
            const BoundingBox &boundingBox,
            const CVecI3& dims);
    RectilinearGrid(
            const CVecI3& offset,
            const CVecD3& origin,
            const vector<double> step[D]);
    ~RectilinearGrid ();
    RectilinearGrid&
     operator=(const RectilinearGrid& cGrid);
    inline vector<double>
     getPos(const int& direction) const;
    inline CVecD3
     getPos(const CVecI3& ijk) const;
    vector<double>
     getPosInRange(
            const int direction,
            const double min,
            const double max) const;
    vector<CVecD3>
     getPos() const;
    vector<CVecD3>
     getCenterOfNaturalCellsInside(
            const BoundingBox& bound) const;
    vector<double>
     getStep(
            const int direction) const;
    inline CVecI3
     getNumCells() const;
    BoundingBox
     getBoundingBox(const pair<CVecI3, CVecI3>& ijkMinMax) const;
    BoundingBox
     getBoundingBoxContaining(const CVecD3& point) const;
    inline BoundingBox
     getFullDomainBoundingBox() const;
    bool
     hasZeroSize() const;
    bool
     isIntoDir(const int& direction, const double& pos) const;
    bool
     isInto (const CVecD3& pos) const;
    bool
     getNaturalCellDir(
            long int &i,
            double &relativeLen,
            const int& dir,
            const double &x,
            const double tol = tolerance) const;
    pair<CVecI3, CVecD3>
    getNaturalCellPair(
            const CVecD3& pos,
            const bool approx = true,
            const double tol = tolerance) const;
    CVecI3
    getNaturalCell(
            const CVecD3 &coords,
            const bool approx = true,
            const double tol = tolerance) const;
    void
     applyScalingFactor(const double factor);

    double getPositionOfNaturalCellDir(const int dir, long int i) const;
    CVecD3 getPositionOfNaturalCell(const CVecI3& ijk) const;
    CVecD3 getOrigin() const {	return origin_;}

    double getMinimumSpaceStep() const;
    bool isRegular() const;
    bool isCartesian() const;
    bool isNaturalCell(
            const CVecD3 position,
            const double tol = tolerance) const;
    bool isNaturalCell(
            const vector<CVecD3> positions,
            const double tol = tolerance) const;
    void printInfo() const;
private:
#   ifdef __GXX_EXPERIMENTAL_CXX0X__
    static constexpr double tolerance = 1e-12;
#   else
    static const double tolerance = 1e-12;
#   endif
    CVecI3 offsetGrid_;
    CVecD3 origin_;
    vector<double> pos_[D];
    vector<double>
    extractRange(
            const vector<double>& vec,
            const pair<double,double>& minMax) const;
};

#endif //_cartesianGrid_hxx
