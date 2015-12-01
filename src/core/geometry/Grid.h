// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_GEOMETRY_GRID_H_
#define SEMBA_GEOMETRY_GRID_H_

#include <vector>

#include "Box.h"
#include "math/Util/Real.h"
#include "math/vector/CVecI3Fractional.h"

namespace SEMBA {
namespace Geometry {

template<class T, Size D> class Box;

template<Size D>
class Grid {
    typedef Box<Math::Real,D> BoxRD;
    typedef Box<Math::Int ,D> BoxID;
    typedef Math::Vector::Cartesian<Math::Real,D> CVecRD;
    typedef Math::Vector::Cartesian<Math::Int, D> CVecID;
public:
    static const Math::Real tolerance;

    Grid();
    Grid(const BoxRD&  boundingBox,
         const CVecRD& dxyz);
    Grid(const BoxRD&  boundingBox,
         const CVecID& dims);
    Grid(const std::vector<Math::Real> step[D],
         const CVecID& offset = CVecID(0),
         const CVecRD& origin = CVecRD(0.0));
    Grid(const Grid& grid);
    ~Grid ();

    Grid& operator=(const Grid& cGrid);
    void setPos(const std::vector<Math::Real> pos[D],
                const CVecID& offset = CVecID(0));
    void setAdditionalSteps(const Math::Constants::CartesianAxis d,
                            const Math::Constants::CartesianBound b,
                            const std::vector<Math::Real>& step);

    // TODO To be deleted >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    bool getNaturalCellx (const double &x, long int &i, double &relativeLen) const;
    bool getNaturalCelly (const double &y, long int &j, double &relativeLen) const;
    bool getNaturalCellz (const double &z, long int &k, double &relativeLen) const;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    bool hasZeroSize() const;

    bool isInto(const CVecRD& pos) const;
    bool isInto(const Size dir, const Math::Real pos) const;
    bool isRegular() const;
    bool isRegular(const Size d) const;
    bool isCartesian() const;
    bool isCell(const CVecRD& position,
                const Math::Real tol = tolerance) const;
    bool isCell(const std::vector<CVecRD>& positions,
                const Math::Real tol = tolerance) const;

    CVecID getNumCells() const;
    CVecID getOffset()   const;
    CVecRD getOrigin()   const;

    std::vector<Math::Real> getStep(const Size dir) const;
    Math::Real              getStep(const Size dir, const Math::Int& n) const;

    Math::Real getMinimumSpaceStep() const;

    BoxRD getFullDomainBoundingBox() const;
    BoxID getFullDomainBoundingCellBox() const;
    BoxRD getBoundingBox(const BoxID& bound) const;
    BoxRD getBoxRContaining(const CVecRD& point) const;
    BoxID getBoxIContaining(const CVecRD& point) const;

    std::vector<CVecRD> getCenterOfCellsInside(const BoxRD& bound) const;
    std::vector<Math::Real> getPosInRange(const Size dir,
                                          const Math::Real min,
                                          const Math::Real max) const;

    std::vector<CVecRD>     getPos() const;
    std::vector<Math::Real> getPos(const Size dir) const;
    Math::Real              getPos(const Size dir, const Math::Int i) const;
    CVecRD                  getPos(const CVecID& ijk) const;
    CVecRD                  getPos(const CVecRD& ijk) const { return ijk; }

    std::pair<Math::Int, Math::Real> getCellPair(
            const Size       dir,
            const Math::Real x,
            const bool approx = true,
            const Math::Real tol = tolerance,
            bool* err = NULL) const;
    std::pair<CVecID, CVecRD>        getCellPair(
            const CVecRD& pos,
            const bool approx = true,
            const Math::Real tol = tolerance,
            bool* err = NULL) const;

    Math::CVecI3Fractional getCVecI3Fractional (const CVecRD& xyz,
                                                bool& err = false) const;

    Math::Int getCell(const Size dir,
                      const Math::Real  x,
                      const bool  approx = true,
                      const Math::Real  tol = tolerance,
                      bool* err = NULL) const;
    CVecID    getCell(const CVecRD& pos,
                      const bool  approx = true,
                      const Math::Real tol = tolerance,
                      bool* err = NULL) const;
    CVecID    getCell(const CVecID& pos,
                      const bool approx = true,
                      const Math::Real tol = tolerance) const { return pos; }

    void applyScalingFactor(const Math::Real factor);

    void enlarge(const std::pair<CVecRD,CVecRD>& additionalCells,
                 const std::pair<CVecRD,CVecRD>& sizesOfNewCells);
    void enlargeBound(Math::Constants::CartesianAxis d,
                      Math::Constants::CartesianBound b,
                      Math::Real pad, Math::Real siz);

    void printInfo() const;

private:
    CVecID offset_;
    std::vector<Math::Real> pos_[D];
};

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Grid.hpp"

namespace SEMBA {
namespace Geometry {

typedef Grid<3> Grid3;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_GRID_H_ */
