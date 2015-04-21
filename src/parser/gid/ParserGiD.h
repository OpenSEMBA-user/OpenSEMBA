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
    ParserGiD(const string& fn, const string& pTPath);
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
    MeshUnstructured* mesh_;
    string problemTypePath_;
    ProblemSize pSize_;
    OptionsSolver* readSolverOptions();
    OptionsMesher* readMesherOptions();
    EMSourceGroup<>* readEMSources();
    OutRqGroup<>* readOutputRequests();
    PhysicalModelGroup<>* readMaterials();
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
    LayerGroup<> readLayers();
    CoordinateGroup<CoordR3> readCoordinates();
    ElementsGroup<ElemR> readElements(const CoordinateGroup<CoordR3>&);
    void readHex8Elements (const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    void readTet10Elements(const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    void readTet4Elements (const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    void readTri6Elements (const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    void readTri3Elements (const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    void readLin2Elements (const CoordinateGroup<CoordR3>& v,
                           vector<ElemR*>& elems);
    Grid3* readCartesianGrid();
    void readOutRqInstances(OutRqGroup<>* res);
    void getNextLabelAndValue(string& label, string& value);
    PlaneWave* readPlaneWave();
    Dipole* readDipole();
    Waveport* readWaveport();
    Generator* readGenerator();
    SourceOnLine* readSourceOnLine();
    PhysicalModel* readPhysicalModel(const MatId id);
    Magnitude* readMagnitude(const string type);
    pair<CVecR3, CVecR3> strToBound(const string& str) const;
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
    void init(const string& pTPath);
    string readVersion();
    bool checkVersionCompatibility(const string version) const;
    ElementsGroup<Vol> boundToElemGroup(const string& line);
};

#endif /* PARSERGID_H_ */
