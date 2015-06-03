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
    Real scalingFactor_;
    pair<CVecR3,CVecR3> boundaryPadding_, boundaryMeshSize_;

    OptionsSolver* readSolverOptions();
    OptionsMesher* readMesherOptions();
    GroupEMSources<>* readEMSources();
    GroupOutRqs<>* readOutputRequests();
    GroupPhysicalModels<>* readMaterials();
    MeshUnstructured* readMesh();
    ProblemSize readProblemSize();
    PMVolumeDispersive* readDispersiveMatFile(
            const MatId id_,
            const string& name) const;
    PMSurfaceSIBC* readIsotropicSurfMatFile(
            const MatId id,
            const string& name) const;
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
    Waveport* readWaveport();
    Generator* readGenerator();
    SourceOnLine* readSourceOnLine();
    PhysicalModel* readPhysicalModel(const MatId id);
    Magnitude* readMagnitude(const string type);
    CVecR3 strToCVecR3(const string& str) const;
    Generator::Type strToGeneratorType(string label) const;
    Generator::Hardness strToGeneratorHardness(string str) const;
    SourceOnLine::Type strToNodalType(string label) const;
    SourceOnLine::Hardness strToNodalHardness(string label) const;
    OptionsMesher::BoundType strToBoundType(string label) const;
    PhysicalModel::Type strToMaterialType(string label) const;
    PMMultiport::Type strToMultiportType(string label) const;
    OutRq<void>::Type strToOutputType(string label) const;
    SIBCType strToSIBCType(string str) const;
    GiDOutputType strToGidOutputType(string label) const;
    Domain strToDomain(string line) const;
    OptionsMesher::Mesher strToMesher(string) const;
    OptionsMesher::Mode strToMesherMode(string) const;
    OptionsSolver::Solver strToSolver(string) const;
    OptionsSolver::CompositeModel strToCompositeModel(string) const;
    OptionsSolver::MetalModel strToMetalModel(string) const;
    OptionsSolver::WireModel strToWireModel(string) const;
    OptionsSolver::SelfInductanceModel strToSelfInductanceModel(string) const;
    string readVersion();
    bool checkVersionCompatibility(const string version) const;
    GroupElements<Vol> boundToElemGroup(const string& line);

    static pair<CVecR3, CVecR3> strToBound(const string& str);
};

#endif /* PARSERGID_H_ */
