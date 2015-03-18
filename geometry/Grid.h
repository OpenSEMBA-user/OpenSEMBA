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

template<Int D>
class Grid {
    typedef Box<Real,D> BoxRD;
    typedef CartesianVector<Real,D> CVecRD;
    typedef CartesianVector<Int,D> CVecID;
public:
    Grid();
    Grid(const Grid& grid);
    Grid(
            const BoxRD &boundingBox,
            const CVecR3& dxyz);
    Grid(
            const BoxRD &boundingBox,
            const CVecI3& dims);
    Grid(
            const CVecI3& offset,
            const CVecR3& origin,
            const vector<Real> step[D]);
    ~Grid ();
    Grid& operator=(const Grid& cGrid);
    inline vector<Real> getPos(const Int& direction) const;
    inline CVecRD getPos(const CVecID& ijk) const;
    vector<Real> getPosInRange(
            const CartesianAxis dir,
            const Real min,
            const Real max) const;
    vector<CVecR3> getPos() const;
    vector<CVecR3> getCenterOfNaturalCellsInside(
            const BoxRD& bound) const;
    vector<Real> getStep(const CartesianAxis dir) const;
    inline CVecI3 getNumCells() const;
    BoxRD getBoundingBox(const pair<CVecI3, CVecI3>& ijkMinMax) const;
    BoxRD getBoundingBoxContaining(const CVecR3& point) const;
    inline BoxRD getFullDomainBoundingBox() const;
    bool hasZeroSize() const;
    bool isIntoDir(const Int& direction, const Real& pos) const;
    bool isInto (const CVecR3& pos) const;

    void enlarge(
            const pair<CVecI3,CVecI3>& additionalCells,
            const pair<CVecR3,CVecR3>& sizesOfNewCells);

    bool getNaturalCellDir(
            Int &i,
            Real &relativeLen,
            const Int& dir,
            const Real &x,
            const Real tol = tolerance) const;
    pair<CVecI3, CVecR3> getNaturalCellPair(
            const CVecR3& pos,
            const bool approx = true,
            const Real tol = tolerance) const;
    CVecI3 getNaturalCell(
            const CVecR3 &coords,
            const bool approx = true,
            const Real tol = tolerance) const;
    void applyScalingFactor(const Real factor);

    Real getPositionOfNaturalCellDir(const Int dir, Int i) const;
    CVecR3 getPositionOfNaturalCell(const CVecI3& ijk) const;
    CVecR3 getOrigin() const {	return origin_;}

    Real getMinimumSpaceStep() const;
    bool isRegular() const;
    bool isRegular(const Int d) const;
    bool isCartesian() const;
    bool isNaturalCell(
            const CVecR3 position,
            const Real tol = tolerance) const;
    bool isNaturalCell(
            const vector<CVecR3> positions,
            const Real tol = tolerance) const;
    void printInfo() const;
private:
    static const Real tolerance;
    CVecI3 offsetGrid_;
    CVecR3 origin_;
    vector<Real> pos_[D];
    vector<Real>
    extractRange(
            const vector<Real>& vec,
            const pair<Real,Real>& minMax) const;
};

template<Int D>
const Real Grid<D>::tolerance = 1e-12;

#include "Grid.hpp"

typedef Grid<3> Grid3;

#endif //_cartesianGrid_hxx
