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
#include "Box.h"

#ifdef compileWithVTK
#include "../../utils/ugrMesher/vtkLittle.h"
#endif

using namespace std;

#ifndef  _GRID_H_
# define _GRID_H_

template<int D>
class Grid {
    typedef Box<double,D> BoxDD;
    typedef CartesianVector<double,D> CVecDD;
    typedef CartesianVector<long,D> CVecID;
public:
    Grid();
    Grid(const Grid& grid);
    Grid(
            const BoxDD &boundingBox,
            const CVecD3& dxyz);
    Grid(
            const BoxDD &boundingBox,
            const CVecI3& dims);
    Grid(
            const CVecI3& offset,
            const CVecD3& origin,
            const vector<double> step[D]);
    ~Grid ();
    Grid&
     operator=(const Grid& cGrid);
    inline vector<double>
     getPos(const int& direction) const;
    inline CartesianVector<double,D>
     getPos(const CartesianVector<long,D>& ijk) const;
    vector<double>
     getPosInRange(
            const int direction,
            const double min,
            const double max) const;
    vector<CVecD3>
     getPos() const;
    vector<CVecD3>
     getCenterOfNaturalCellsInside(
            const BoxDD& bound) const;
    vector<double>
     getStep(
            const int direction) const;
    inline CVecI3
     getNumCells() const;
    BoxDD
     getBoundingBox(const pair<CVecI3, CVecI3>& ijkMinMax) const;
    BoxDD
     getBoundingBoxContaining(const CVecD3& point) const;
    inline BoxDD
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

#include "Grid.hpp"

typedef Grid<3> Grid3;

#endif //_cartesianGrid_hxx
