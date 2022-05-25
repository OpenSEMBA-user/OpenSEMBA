
/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#pragma once

#include "exporters/Exporter.h"
#include "gidpost.h"

namespace SEMBA {
namespace Exporters {
namespace GiD {

class Exporter : public SEMBA::Exporters::Exporter {
public:
    static const Math::CVecR3 pecColor, pmcColor, smaColor, pmlColor,
        sibcColor, emSourceColor;

    Exporter(const Data& smb, const std::string& fn, GiD_PostMode mode = GiD_PostAscii);
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

    void writeMaterialsInLayer_(const Geometry::Layer::Layer* lay);
    void writeAllElements_(
    		const Group::Group<const Geometry::ElemR>& elem,
            const std::string& name);

    static std::string makeValid_(std::string name);
};


} /* namespace GiD */
} /* namespace Exporters */
} /* namespace SEMBA */

