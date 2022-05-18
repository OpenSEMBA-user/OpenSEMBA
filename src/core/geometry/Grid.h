#pragma once

#include <vector>
#include <algorithm>

#include "Box.h"
#include "math/util/Real.h"

namespace SEMBA {
namespace Geometry {

template<class T, std::size_t D> class Box;

template<std::size_t D>
class Grid {
    typedef Box<Math::Real,D> BoxRD;
    typedef Box<Math::Int ,D> BoxID;
    typedef Math::Vector::Cartesian<Math::Real,D> CVecRD;
    typedef Math::Vector::Cartesian<Math::Int, D> CVecID;
public:
    static const Math::Real tolerance;

    Grid() = default;
    Grid(const BoxRD&  boundingBox,
         const CVecRD& dxyz);
    Grid(const BoxRD&  boundingBox,
         const CVecID& dims);
    Grid(const std::vector<Math::Real> positions[D]);
    Grid(const Grid& grid);
    virtual ~Grid () = default;

    Grid& operator=(const Grid& cGrid);
    bool operator==(const Grid& rhs) const;
    void setPos(const std::vector<Math::Real> pos[D]);
    void setAdditionalSteps(const Math::Constants::CartesianAxis d,
                            const Math::Constants::CartesianBound b,
                            const std::vector<Math::Real>& step);

    bool hasZeroSize() const;

    bool isInto(const CVecRD& pos) const;
    bool isInto(const std::size_t dir, const Math::Real pos) const;
    bool isRegular() const;
    bool isRegular(const std::size_t d) const;
    bool isCartesian() const;
    bool isCell(const CVecRD& position,
                const Math::Real tol = tolerance) const;
    bool isCell(const std::vector<CVecRD>& positions,
                const Math::Real tol = tolerance) const;

    CVecID getNumCells() const;

    CVecID getOffset()   const; // DEPRECATED

    CVecRD getOrigin()   const;
    bool getNaturalCell(
            const Math::Constants::CartesianAxis dir,
            const Math::Real& x,
            long int& i,
            Math::Real& relativeLen) const;

    std::vector<Math::Real> getStep(const std::size_t dir) const;
    Math::Real              getStep(const std::size_t dir,
                                    const Math::Int& n) const;

    Math::Real getMinimumSpaceStep() const;

    BoxRD getFullDomainBoundingBox() const;
    BoxID getFullDomainBoundingCellBox() const;
    BoxRD getBoundingBox(const BoxID& bound) const;
    BoxRD getBoxRContaining(const CVecRD& point) const;
    BoxID getBoxIContaining(const CVecRD& point) const;

    std::vector<CVecRD> getCenterOfCellsInside(const BoxRD& bound) const;
    std::vector<Math::Real> getPosInRange(const std::size_t dir,
                                          const Math::Real min,
                                          const Math::Real max) const;

    std::vector<CVecRD>     getPos() const;
    std::vector<Math::Real> getPos(const std::size_t dir) const;
    Math::Real              getPos(const std::size_t dir,
                                   const Math::Int i) const;
    CVecRD                  getPos(const CVecID& ijk) const;
    CVecRD                  getPos(const CVecRD& ijk) const { return ijk; }

    std::pair<Math::Int, Math::Real> getCellPair(
            const std::size_t       dir,
            const Math::Real x,
            const bool approx = true,
            const Math::Real tol = tolerance,
            bool* err = nullptr) const;
    std::pair<CVecID, CVecRD>        getCellPair(
            const CVecRD& pos,
            const bool approx = true,
            const Math::Real tol = tolerance,
            bool* err = nullptr) const;

    Math::Int getCell(const std::size_t dir,
                      const Math::Real  x,
                      const bool  approx = true,
                      const Math::Real  tol = tolerance,
                      bool* err = nullptr) const;
    CVecID    getCell(const CVecRD& pos,
                      const bool  approx = true,
                      const Math::Real tol = tolerance,
                      bool* err = nullptr) const;
    CVecID    getCell(const CVecID& pos,
                      const bool approx = true,
                      const Math::Real tol = tolerance) const { return pos; }

    void applyScalingFactor(const Math::Real factor);

    void enlarge(const std::pair<CVecRD,CVecRD>& additionalCells,
                 const std::pair<CVecRD,CVecRD>& sizesOfNewCells);
    void enlargeBound(Math::Constants::CartesianAxis d,
                      Math::Constants::CartesianBound b,
                      Math::Real pad, Math::Real siz);

private:
    std::vector<Math::Real> pos_[D];
};

typedef Grid<3> Grid3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Grid.hpp"

