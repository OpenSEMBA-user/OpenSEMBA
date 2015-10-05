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
 * ParserGiD.h
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#ifndef PARSERGID_H_
#define PARSERGID_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "parser/Parser.h"
#include "SmbData.h"
#include "ProblemSize.h"

#define LABEL_ENDING ':'

class ParserGiD : public Parser, public ProjectFile {
    friend class SmbData;
public:
    ParserGiD();
    ParserGiD(const string& fn);
    virtual ~ParserGiD();

    const ProblemSize* getProblemSize() const;

    virtual SmbData* read();
    void printInfo() const;
private:
    typedef enum {
        outRqOnPoint,
        outRqOnLine,
        outRqOnSurface,
        outRqOnVolume,
        bulkCurrentOnSurface,
        bulkCurrentOnVolume,
        farField
    } GiDOutputType;
    typedef enum {
        sibc,
        multilayer,
        undefinedSIBC
    } SIBCType;
    typedef enum {
        rectangular,
        undefined
    } WaveportShape;

    MeshUnstructured* mesh_;
    ProblemSize pSize_;

    OptionsSolver* readOptionsSolver();
    OptionsSolverFDTD* readOptionsSolverFDTD(const OptionsSolver& base);
    OptionsSolverDGTD* readOptionsSolverDGTD(const OptionsSolver& base);
    OptionsMesher* readMesherOptions();
    GroupEMSources<>* readEMSources();
    GroupOutRqs<>* readOutputRequests();
    GroupPhysicalModels<>* readMaterials();
    MeshUnstructured* readMesh();
    ProblemSize readProblemSize();
    PMVolumeDispersive* readDispersiveMatFile(
            const MatId id,
            const string& name,
            const ProjectFile& file) const;
    PMSurfaceSIBC* readIsotropicSurfMatFile(
            const MatId id,
            const string& name,
            const ProjectFile& file) const;
    PMSurfaceMultilayer*  readMultilayerSurf(
            const MatId id,
            const string& name,
            const string& layersString) const;
    GroupLayers<> readLayers();
    GroupCoordinates<CoordR3> readCoordinates();
    GroupElements<ElemR> readElements(const CoordR3Group&);
    void readHex8Elements (const CoordR3Group& v, vector<ElemR*>& elems);
    void readTet10Elements(const CoordR3Group& v, vector<ElemR*>& elems);
    void readTet4Elements (const CoordR3Group& v, vector<ElemR*>& elems);
    void readTri6Elements (const CoordR3Group& v, vector<ElemR*>& elems);
    void readTri3Elements (const CoordR3Group& v, vector<ElemR*>& elems);
    void readLin2Elements (const CoordR3Group& v, vector<ElemR*>& elems);
    Grid3* readCartesianGrid();
    void readOutRqInstances(GroupOutRqs<>* res);
    void getNextLabelAndValue(string& label, string& value);
    PlaneWave* readPlaneWave();
    Dipole* readDipole();
    PortWaveguide* readPortWaveguide();
    PortTEM* readPortTEM();
    Generator* readGenerator();
    SourceOnLine* readSourceOnLine();
    PhysicalModel* readPhysicalModel(const MatId id);
    Magnitude* readMagnitude(const string type);
    LocalAxes strToLocalAxes(const string& str);

    string readVersion();
    bool checkVersionCompatibility(const string version) const;
    GroupElements<> boxToElemGroup(const string& line);

    static OutRq<void>::Type strToOutputType(string label);
    static SIBCType strToSIBCType(string str);
    static GiDOutputType strToGidOutputType(string label);
    static Domain strToDomain(string line);
    static OptionsMesher::Mesher strToMesher(string);
    static OptionsMesher::Mode strToMesherMode(string);
    static CVecR3 strToCVecR3(const string& str);
    static Generator::Type strToGeneratorType(string label);
    static Generator::Hardness strToGeneratorHardness(string str);
    static SourceOnLine::Type strToNodalType(string label);
    static SourceOnLine::Hardness strToNodalHardness(string label);
    static OptionsMesher::BoundType strToBoundType(string label);
    static PhysicalModel::Type strToMaterialType(string label);
    static PMMultiport::Type strToMultiportType(string label);
    static PMVolumeAnisotropic::Model strToAnisotropicModel(string label);

    static OptionsSolverFDTD::PMLBacking strToPMLBacking(const string& string);
    static OptionsSolverFDTD::NoNF2FF strToNoNF2FF(const string& string);
    static OptionsSolver::Solver strToSolver(string);
    static OptionsSolverFDTD::WireModel strToWireModel(string);
    static OptionsSolverFDTD::InductanceModel strToInductanceModel(string);
    static OptionsSolverFDTD::CompositeModel strToCompositeModel(string);
    static pair<CVecR3, CVecR3> strToBox(const string& str);
    static PoleResidue readPoleResiduePair(ifstream& stream);
};

#endif /* PARSERGID_H_ */
