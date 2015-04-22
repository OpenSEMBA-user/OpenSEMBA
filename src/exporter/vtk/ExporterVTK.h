/*
 * ExporterVTK.h
 *
 *  Created on: 22/4/2015
 *      Author: Daniel
 */

#ifndef EXPORTER_VTK_EXPORTERVTK_H_
#define EXPORTER_VTK_EXPORTERVTK_H_

#include <fstream>
#include <utility>
using namespace std;

#include "exporter/Exporter.h"
#include "SmbData.h"
#include "gidpost/gidpost.h"

class ExporterVTK : public Exporter {
public:
    ExporterVTK(const SmbData* smb,
                const string& fn);
    virtual ~ExporterVTK();

private:
    enum CELL_TYPES {
        VTK_VERTEX               = 1,
        VTK_POLY_VERTEX          = 2,
        VTK_LINE                 = 3,
        VTK_POLY_LINE            = 4,
        VTK_TRIANGLE             = 5,
        VTK_TRIANGLE_STRIP       = 6,
        VTK_POLYGON              = 7,
        VTK_PIXEL                = 8,
        VTK_QUAD                 = 9,
        VTK_TETRA                = 10,
        VTK_VOXEL                = 11,
        VTK_HEXAHEDRON           = 12,
        VTK_WEDGE                = 13,
        VTK_PYRAMID              = 14,
        VTK_QUADRATIC_EDGE       = 21,
        VTK_QUADRATIC_TRIANGLE   = 22,
        VTK_QUADRATIC_QUAD       = 23,
        VTK_QUADRATIC_TETRA      = 24,
        VTK_QUADRATIC_HEXAHEDRON = 25
    };

    void initDir_(const string& fn);
    void writeMesh_(const Mesh* inMesh,
                    const GroupPhysicalModels<>* mat,
                    const GroupEMSources<>* src = NULL,
                    const GroupOutRqs<>* oRq = NULL);
    void writeFile_(const GroupElements<const ElemR>& elems,
                    const string& name,
                    ofstream& outMain,
                    UInt& part);
    pair<vector<CVecR3>, map<CoordinateId, UInt>> getPoints_(
            const GroupElements<const ElemR>& elems);
    void writePoints_(ofstream& outFile,
                      const vector<CVecR3>& pos);
    void writeCells_(ofstream& outFile,
                     const GroupElements<const ElemR>& elems,
                     const map<CoordinateId, UInt>& mapCoords);

};

#endif /* EXPORTER_VTK_EXPORTERVTK_H_ */
