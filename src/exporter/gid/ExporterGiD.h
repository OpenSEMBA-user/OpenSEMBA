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
#ifndef EXPORTER_GID_EXPORTERGID_H_
#define EXPORTER_GID_EXPORTERGID_H_

#include "exporter/Exporter.h"
#include "gidpost/gidpost.h"

using namespace std;

class ExporterGiD : public Exporter {
public:
    ExporterGiD(
            const SmbData* smb,
            GiD_PostMode mode = GiD_PostAscii);
    ExporterGiD(
            const SmbData* smb,
            const string& fn,
            GiD_PostMode mode = GiD_PostAscii);
//    ExporterGiD(
//            const MeshUnstructured* mesh,
//            const GroupPhysicalModels<>* mat,
//            const string& fn,
//            GiD_PostMode mode = GiD_PostAscii);
    virtual ~ExporterGiD();
protected:
    void beginMesh(
            const string& tName,
            GiD_Dimension dim,
            GiD_ElementType elementType,
            Int nNode,
            const CVecR3& ColorRGB = CVecR3()) const;
    void beginResult(
            const string& fieldName,
            const string& timeName,
            const Real time,
            GiD_ResultType resultType,
            GiD_ResultLocation resultLocaltion,
            const string gaussPointType,
            const vector<string>& componentsNames) const;
    void flushPostFile() const;
private:
    static Int coordCounter_;
    static Int elemCounter_;
    static const CVecR3 pecColor, pmcColor, smaColor, pmlColor,
     sibcColor, emSourceColor;
    static Int numberOfOutputGiD_;
    GiD_FILE meshFile_;
    GiD_FILE resultFile_;
    GiD_PostMode mode_;
    void writeMesh(const SmbData* smb);
    void writeElements(
            const GroupElements<const ElemR>& entities,
            const string& name,
            const GiD_ElementType type,
            const Int nV);
    static string makeValid(string name);
    void beginCoordinates() const;
    void writeGaussPoints() const;
    void writeCoordinates(CoordR3Group& pos);
    void endCoordinates() const;
    void beginElements() const;
    void writeElement(Int elemId, int nId[]) const;
    void endElements() const;
    void endMesh() const;
    GiD_ResultType getGiDResultType(OutRqBase::Type type) const;
    GiD_ResultLocation getGiDResultLocation() const;
    void openPostMeshFile(const string& filename);
    void openPostResultFile(const string& filename);
    void writeMaterialsInLayer(const Layer* lay);
    void writeAllElements(const GroupElements<const ElemR>& elem,
                          const string& name);
    void initDefault(const SmbData* smb, GiD_PostMode mode, const string& fn);
};

#endif /* EXPORTER_GID_EXPORTERGID_H_ */
