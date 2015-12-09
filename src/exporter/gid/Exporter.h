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
/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef SEMBA_EXPORTER_GID_EXPORTER_H_
#define SEMBA_EXPORTER_GID_EXPORTER_H_

#include "exporter/Exporter.h"
#include "gidpost/gidpost.h"

namespace SEMBA {
namespace Exporter {
namespace GiD {

class Exporter : public SEMBA::Exporter::Exporter {
public:
    static const Math::CVecR3 pecColor, pmcColor, smaColor, pmlColor,
    sibcColor, emSourceColor;

    Exporter(
            const Data* smb,
            GiD_PostMode mode = GiD_PostAscii);
    Exporter(
            const Data* smb,
            const std::string& fn,
            GiD_PostMode mode = GiD_PostAscii);
    virtual ~Exporter();

    void beginMesh(
            const std::string& tName,
            GiD_Dimension dim,
            GiD_ElementType elementType,
            Math::Int nNode,
            const Math::CVecR3& ColorRGB = Math::CVecR3()) const;
    void beginResult(
            const std::string& fieldName,
            const std::string& timeName,
            const Math::Real time,
            GiD_ResultType resultType,
            GiD_ResultLocation resultLocaltion,
            const std::string gaussPointType,
            const std::vector<std::string>& componentsNames) const;
    void flushPostFile() const;
private:
    static Math::Int coordCounter_;
    static Math::Int elemCounter_;
    static Math::Int numberOfOutputGiD_;

    GiD_FILE meshFile_;
    GiD_FILE resultFile_;
    GiD_PostMode mode_;

    void init_(const Data* smb, GiD_PostMode mode, const std::string& fn);
    void writeMesh_(const Data* smb);
    void writeElements_(
            const Group::Group<const Geometry::ElemR>& entities,
            const std::string& name,
            const GiD_ElementType type,
            const Math::Int nV);
    void writeElement_(Math::Int elemId, int nId[]) const;
    void writeCoordinates_(Geometry::CoordR3Group& pos);

    void beginCoordinates_() const;
    void endCoordinates_() const;
    void beginElements_() const;
    void endElements_() const;
    void endMesh_() const;

    GiD_ResultType getGiDResultType_(
    		OutputRequest::OutputRequest::Type type) const;
    GiD_ResultLocation getGiDResultLocation_() const;

    void openPostMeshFile_(const std::string& filename);
    void openPostResultFile_(const std::string& filename);

    void writeMaterialsInLayer_(const Geometry::Layer* lay);
    void writeAllElements_(
    		const Group::Group<const Geometry::ElemR>& elem,
            const std::string& name);

    static std::string makeValid_(std::string name);
};

} /* namespace GiD */
} /* namespace Exporter */
} /* namespace SEMBA */

#endif /* EXPORTER_GID_EXPORTERGID_H_ */
