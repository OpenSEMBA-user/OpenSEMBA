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

#include "Exporter.h"

#ifdef _WIN32
#include <direct.h>
#endif

#include "geometry/mesh/Unstructured.h"
#include "physicalModel/PEC.h"
#include "physicalModel/SMA.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

Exporter::Exporter() {
    smb_ = NULL;
}

Exporter::Exporter(
        const Data* smb) {
    smb_ = smb;
    setFilename(smb_->getFolder() + "/openfoam/");
    deleteDirIfExists(getFilename());
    createOpenFoamDirs();
    writeOpenFoamDummyFile();
    writeControlDict();
    writefvSchemes();
    writefvSolution();
    writeAllClean();
    //   if (smb_->solverParams->numberOfProcesses == 1) {
    writeAllRun();
    //   } else {
    //      writeDecomposeParDict();
    //      writeAllRunParallel();
    //   }
    writeSTLs();
    writeBlockMeshDict();
    writeSurfaceFeatureExtractDict();
    writeSnappyHexMeshDict();
    writeMeshQualityDict();
}

Exporter::~Exporter() {
}

void
Exporter::createOpenFoamDirs() {
    dirConstant_ = getFilename() + "constant/";
    dirPolymesh_ = getFilename() + "constant/polyMesh/";
    dirTriSurface_ = getFilename() + "constant/triSurface/";
    dirSystem_ = getFilename() + "system/";
#ifdef _WIN32
    mkdir(getFilename().c_str());
    mkdir(dirConstant_.c_str());
    mkdir(dirPolymesh_.c_str());
    mkdir(dirTriSurface_.c_str());
    mkdir(dirSystem_.c_str());
#else
    mkdir(getFilename().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(dirConstant_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(dirPolymesh_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(dirTriSurface_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(dirSystem_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void
Exporter::writeSTLs() const {
    // Writes materials.
    for (std::size_t i = 0; i < smb_->physicalModels->size(); i++) {
        const PhysicalModel::PhysicalModel* mat = (*smb_->physicalModels)(i);
        if (!mat->is<PhysicalModel::SMA>()) {
            //TODO
            //bool includeTets = !smb_->mesherOptions->isBruteForceVolumes();
            bool includeTets = false;
            if (mat->is<PhysicalModel::PEC>()) {
                includeTets = true;
            }
            MatId matId = mat->getId();
            const Geometry::Mesh::Unstructured* mesh = 
                smb_->mesh->castTo<Geometry::Mesh::Unstructured>();
            Geometry::ConstElemRGroup elems = mesh->elems().getMatId(matId);
            Geometry::Element::Group<const Geometry::Tri> tri = 
                mesh->convertToTri(elems, includeTets);
            triToSTL(tri, dirTriSurface_, "mat", matId.toInt(), mat->getName());
        }
    }
    // Writes surface output requests.
    //	const std::size_t nOutRq =
    //	 smb_->outputRequests->countWithType(Element::SURFACE);
    //	for (std::size_t i = 0; i < nOutRq; i++) {
    //		const OutputRequest* outRq =
    //		 smb_->outputRequests->getWithType(i, Element::SURFACE);
    //		const std::vector<Tri3> tri = smb_->mesh->getTriWithId(outRq->getElem());
    //		const std::string name = outRq->getName();
    //		stringstream num;
    //		num << i;
    //		triToSTL(tri, dirTriSurface + "out." + num.str(), name);
    //	}
    //   // Writes surface EMSource.
    //   const std::size_t nEM = smb_->emSources->countWithType(ElementBase::surface);
    //   for (std::size_t i = 0; i < nEM; i++) {
    //      const EMSource* em = smb_->emSources->getWithType(i, ElementBase::surface);
    //      GroupElements<Tri3> tri = smb_->mesh->get(em->getElem()).getGroupOf<Tri3>();
    //      const std::string name = "Waveport";
    //      triToSTL(tri, dirTriSurface_, "src", i, name);
    //   }
}

void
Exporter::triToSTL(const Geometry::Element::Group<const Geometry::Tri>& tri,
                   const std::string& folder,
                   const std::string& type,
                   const std::size_t& typeId,
                   const std::string& name) const {
    std::map<Geometry::LayerId, std::vector<const Geometry::Tri*> > layers = 
        tri.separateByLayers();
    std::map<Geometry::LayerId, std::vector<const Geometry::Tri*> >::iterator 
        it;
    for (it = layers.begin(); it != layers.end(); ++it) {
        const Geometry::LayerId layerId = it->first;
        const std::vector<const Geometry::Tri*> tri = it->second;
        // Opens file.
        std::ofstream file;
        std::string filename;
        filename = folder + type
                + "." + intToStr(typeId) + "." + intToStr(layerId.toInt());
        openFile(filename + ".stl", file);
        // Writes data.
        const std::size_t nElem = tri.size();
        std::string solidName(name);
        const Geometry::Mesh::Unstructured* mesh = 
            smb_->mesh->castTo<Geometry::Mesh::Unstructured>();
        if (mesh->layers().getId(layerId) != NULL) {
            solidName += "@" + mesh->layers().getId(layerId)->getName();
        }
        file << "solid " << solidName << std::endl;
        for (std::size_t i = 0; i < nElem; i++) {
            file << "facet ";
            //          Math::CVecR3 n = tri[i]->getNormal();
            //          file << " normal " << n(0) << " " << n(1) << " " << n(2)
            file << std::endl;
            file << "  outer loop" << std::endl;
            for (std::size_t j = 0; j < tri[i]->numberOfVertices(); j++) {
                Math::CVecR3 v = tri[i]->getVertex(j)->pos();
                file << "    vertex "
                        << v(0) << " "
                        << v(1) << " "
                        << v(2) << std::endl;
            }
            file << "  endloop" << std::endl;
            file << "endfacet" << std::endl;
        }
        file << "endsolid " << solidName << std::endl;
        file.close();
    }
}

void
Exporter::writeOpenFoamDummyFile() const {
    std::ofstream file;
    openFile(getFilename() + "openfoam.foam", file);
    file.close();
}

void
Exporter::writeControlDict() const {
    std::string name = "controlDict";
    std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    file << writeOpenFoamHeader(std::string("system"), name) << std::endl;
    file << std::endl;
    file << "filestartFrom   startTime;" << std::endl;
    file << "startTime       latestTime;" << std::endl;
    file << "stopAt          endTime;" << std::endl;
    file << "endTime         0;" << std::endl;
    file << "deltaT          1;" << std::endl;
    file << "writeControl    timeStep;" << std::endl;
    file << "writeInterval   1;" << std::endl;
    file << "purgeWrite      0;" << std::endl;
    file << "writeFormat     ascii;" << std::endl;
    file << "writePrecision  6;" << std::endl;
    file << "writeCompression uncompressed;" << std::endl;
    file << "timeFormat      general;" << std::endl;
    file << "timePrecision   6;" << std::endl;
    file << "runTimeModifiable yes;" << std::endl;
    // Closes file.
    file.close();
}

void
Exporter::writefvSchemes() const {
    const std::string name = "fvSchemes";
    const std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    file << writeOpenFoamHeader("system", name) << std::endl;
    file << "gradSchemes {}" << std::endl;
    file << "divSchemes {}" << std::endl;
    file << "laplacianSchemes {}" << std::endl;
    // Closes file.
    file.close();
}

void
Exporter::writefvSolution() const {
    const std::string name = "fvSolution";
    const std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    file << writeOpenFoamHeader("system", name) << std::endl;
    // Closes file.
    file.close();
}

std::string
Exporter::writeAllBoundary() const {
    std::string res;
    res =  "allBoundary {\n";
    res += "  type patch; \n";
    res += "    faces ( (3 7 6 2) (0 4 7 3) (2 6 5 1)\n";
    res += "            (1 5 4 0) (0 3 2 1) (4 5 6 7) );\n";
    res += "}";
    return res;
}

//void
//Exporter::writeDecomposeParDict() const {
//   // Opens file for writing.
//   const std::string name = "decomposeParDict";
//   const std::string fileName = dirSystem_ + "/" + name;
//   std::ofstream file;
//   openFile(fileName, file);
//   // Prepares data.
//   std::size_t numberOfProcesses = smb_->solverParams->numberOfProcesses;
//   // Writes file.
//   file<< writeOpenFoamHeader("system/", name) << std::endl
//         << std::endl
//         << "numberOfSubdomains " << numberOfProcesses << ";" << std::endl
//         << std::endl
//         << "method hierarchical;" << std::endl
//         << std::endl
//         << "hierarchicalCoeffs {" << std::endl
//         << "    n (" << numberOfProcesses << " 1 1);" << std::endl
//         << "    order xyz;" << std::endl
//         << "    delta 0.001;" << std::endl
//         << "}" << std::endl
//         << std::endl
//         << "distributed no;" << std::endl;
//   // Closes file.
//   file.close();
//}

void
Exporter::writeBlockMeshDict() const {
    // Opens file for writing.
    const std::string name = "blockMeshDict";
    const std::string fileName = dirPolymesh_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    // Prepares data.
    const Geometry::Grid3* grid = smb_->grid;
    if (grid->hasZeroSize()) {
        throw std::logic_error("Rectilinear grid has zero size.");
    }
    Math::CVecR3 min(grid->getFullDomainBoundingBox().getMin());
    Math::CVecR3 max(grid->getFullDomainBoundingBox().getMax());
    // blockMesh can not run in parallel. Because of this reason we have to
    // create an initial mesh with nCells/nProc processors and then
    // use refineMesh. Therefore the following equality must be true:
    //                 (nCellsX / nProc) % 2 == 0
    // This is ensured by computeRefinableCellDim.
    Math::Int dimX = computeRefinableCellDim(grid->getNumCells()(0), 1);
    Math::Int dimY = computeRefinableCellDim(grid->getNumCells()(1), 1);
    Math::Int dimZ = computeRefinableCellDim(grid->getNumCells()(2), 1);
    // Writes file.
    file << writeOpenFoamHeader("constant/polyMesh", name) << std::endl;
    // Scaling should have already been applied to mesh. It is not
    // necessary to modify this.
    file << "convertToMeters 1;" << std::endl;
    file << std::endl;
    // Vertices.
    file << "vertices" << std::endl;
    file << "(" << std::endl;
    file << "    ( " << min(0) << " " << min(1) << " " << min(2) << ")" << std::endl;
    file << "    ( " << max(0) << " " << min(1) << " " << min(2) << ")" << std::endl;
    file << "    ( " << max(0) << " " << max(1) << " " << min(2) << ")" << std::endl;
    file << "    ( " << min(0) << " " << max(1) << " " << min(2) << ")" << std::endl;
    file << "    ( " << min(0) << " " << min(1) << " " << max(2) << ")" << std::endl;
    file << "    ( " << max(0) << " " << min(1) << " " << max(2) << ")" << std::endl;
    file << "    ( " << max(0) << " " << max(1) << " " << max(2) << ")" << std::endl;
    file << "    ( " << min(0) << " " << max(1) << " " << max(2) << ")" << std::endl;
    file << ");" << std::endl;
    file << std::endl;
    // Blocks.
    file << "blocks" << std::endl;
    file << "(" << std::endl;
    file << "    hex (0 1 2 3 4 5 6 7) ("
            << dimX << " " << dimY << " " << dimZ
            << ") simpleGrading (1 1 1)" << std::endl;
    file << ");" << std::endl;
    file << std::endl;
    // Edges.
    file << "edges" << std::endl;
    file << "(" << std::endl;
    file << ");" << std::endl;
    file << std::endl;
    // Boundary.
    file << "boundary" << std::endl;
    file << "(" << std::endl;
    file << "  xMin {type symmetryPlane; faces ((0 4 7 3));}" << std::endl;
    file << "  yMin {type symmetryPlane; faces ((1 5 4 0));}" << std::endl;
    file << "  zMin {type symmetryPlane; faces ((0 1 2 3));}" << std::endl;
    file << "  xMax {type symmetryPlane; faces ((1 5 6 2));}" << std::endl;
    file << "  yMax {type symmetryPlane; faces ((2 3 7 6));}" << std::endl;
    file << "  zMax {type symmetryPlane; faces ((4 5 6 7));}" << std::endl;
    file << ");" << std::endl;
    // Closes file.
    file.close();
}

std::string
Exporter::writeOpenFoamHeader(
        const std::string& location,
        const std::string& object) const {
    std::string res;
    res =  "// Written automatically by smbExtract\n";
    res += "// Propietary software of University of Granada, 2014.\n";
    res += "// In the case of unexpected behaviour contact lm@diazangulo.com\n";
    res +=  "FoamFile\n";
    res += "{\n";
    res += "    version     2.0;\n";
    res += "    format      ascii;\n";
    res += "    class       dictionary;\n";
    res += "    location    \"" + location + "\";\n";
    res += "    object      " + object + ";\n";
    res += "}\n";
    return res;
}

void
Exporter::writeSurfaceFeatureExtractDict() const {
    // Opens file.
    std::string name = "surfaceFeatureExtractDict";
    std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    // Prepares data.
    std::vector<std::string> stlFile = getFilesBasenames(dirTriSurface_, ".stl");
    const std::size_t nSTLs = stlFile.size();
    // Writes data.
    file << writeOpenFoamHeader("system", name) << std::endl;
    file << std::endl;
    for (std::size_t i = 0; i < nSTLs; i++) {
        file<< stlFile[i] << std::endl
                << "{" << std::endl
                << "    extractionMethod    extractFromSurface;" << std::endl
                << "    extractFromSurfaceCoeffs" << std::endl
                << "    {" << std::endl
                << "        includedAngle 180;" << std::endl
                << "    }" << std::endl
                << "    writeObj true;" << std::endl
                << "}" << std::endl
                << std::endl;
    }
    // Closes file.
    file.close();
}

void
Exporter::writeSnappyHexMeshDict() const {
    // Opens file.
    std::string name = "snappyHexMeshDict";
    std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    // Prepares data.
    std::vector<std::string> stlFile = getFilesBasenames(dirTriSurface_, ".stl");
    const std::size_t nSTLs = stlFile.size();
    // Writes data.
    file << writeOpenFoamHeader("system", name) << std::endl;
    file << std::endl;
    file << "castellatedMesh " << boolToStr(true) << ";" << std::endl;
    file << "snap            " << boolToStr(false) << ";" << std::endl;
    file << "addLayers       " << boolToStr(false) << ";" << std::endl;
    file << std::endl;
    // ---- Geometry ----
    file << "geometry" << std::endl;
    file << "{" << std::endl;
    for (std::size_t i = 0; i < nSTLs; i++) {
        std::string matName = removeExtension(stlFile[i]);
        file << "    " << stlFile[i] << std::endl;
        file << "    {" << std::endl;
        file << "        type triSurfaceMesh;" << std::endl;
        file << "        name " << matName << ";" << std::endl;
        file << "    }" << std::endl;
    }
    file << "};" << std::endl;
    file << std::endl;
    // ---- Castellated mesh controls ----
    file << "castellatedMeshControls" << std::endl;
    file << "{" << std::endl;
    file << "    maxLocalCells 100000;" << std::endl;
    file << "    maxGlobalCells 2000000;" << std::endl;
    file << "    minRefinementCells 0;" << std::endl;
    file << "    nCellsBetweenLevels 1;" << std::endl;

    file << std::endl;
    // Explicit features.
    file << "    features" << std::endl;
    file << "    (" << std::endl;
    //	for (std::size_t i = 0; i < nSTLs; i++) {
    //		file<< "        {" << std::endl
    //		 << "            file \"" << removeExtension(stlFile[i])
    //		 << ".extendedFeatureEdgeMesh\";" << std::endl
    //		 << "            level "
    //		 << smb_->ofParams->getFeatureRefinementLevel()<< ";" << std::endl
    //		 << "        }" << std::endl;
    //	}
    file << "    );" << std::endl;
    file << std::endl;
    // Refinement surfaces.
    file << "    refinementSurfaces" << std::endl;
    file << "    {" << std::endl;
    for (std::size_t i = 0; i < nSTLs; i++) {
        file << "        " << removeExtension(stlFile[i]) << std::endl;
        file << "        {" << std::endl;
        file << "            level (0 0);" << std::endl;
        file << "        }" << std::endl;
        // TODO If it is a surface we have to put:
        /* faceZone mat.1;
		   faceType baffle;
		   cellZone mat.1;
		   cellZoneInside inside;
         */
        // To obtain a non duplicated surface when snapping is activated.
    }
    file << "    }" << std::endl;
    file << std::endl;
    file << "    resolveFeatureAngle 30;" << std::endl;
    file << std::endl;
    // Refinement regions.
    file << "    refinementRegions" << std::endl;
    file << "    {" << std::endl;
    file << "    }" << std::endl;
    // Mesh selection.
    Math::CVecR3 loc = computeLocationInMesh();
    file << "   locationInMesh "
            << "(" << loc(0) << " " << loc(1) << " " << loc(2) << ");" << std::endl;
    file << std::endl;
    file << "   allowFreeStandingZoneFaces true;" << std::endl;
    file << "}" << std::endl; // Closes castellated mesh controls.
    file << std::endl;
    // ---- Snap controls ----
    file << "snapControls" << std::endl;
    file << "{" << std::endl;
    file << "    nSmoothPatch 3;" << std::endl;
    file << "    tolerance 1.5;" << std::endl;
    file << "    nSolveIter 150;" << std::endl;
    file << "    nRelaxIter 5;" << std::endl;
    file << "    nFeatureSnapIter 10;" << std::endl;
    file << "    implicitFeatureSnap false;" << std::endl;
    file << "    explicitFeatureSnap true;" << std::endl;
    file << "    multiRegionFeatureSnap true;" << std::endl;
    file << "}" << std::endl;
    file << std::endl;
    // ---- addLayers controls ----
    file << "addLayersControls" << std::endl;
    file << "{" << std::endl;
    file << "    relativeSizes true;" << std::endl;
    file << "    layers { }" << std::endl;
    file << "    expansionRatio 1.0;" << std::endl;
    file << "    finalLayerThickness 0.3;" << std::endl;
    file << "    minThickness 0.25;" << std::endl;
    file << "    nGrow 0;" << std::endl;
    file << "    featureAngle 30;" << std::endl;
    file << "    nRelaxIter 5;" << std::endl;
    file << "    nSmoothSurfaceNormals 1;" << std::endl;
    file << "    nSmoothNormals 3;" << std::endl;
    file << "    nSmoothThickness 10;" << std::endl;
    file << "    maxFaceThicknessRatio 0.5;" << std::endl;
    file << "    maxThicknessToMedialRatio 0.3;" << std::endl;
    file << "    minMedianAxisAngle 90;" << std::endl;
    file << "    nBufferCellsNoExtrude 0;" << std::endl;
    file << "    nLayerIter 50;" << std::endl;
    file << "}" << std::endl;
    file << std::endl;
    // ---- meshQuality control ----
    file << "meshQualityControls" << std::endl;
    file << "{" << std::endl;
    file << "    #include \"meshQualityDict\"" << std::endl;
    file << "}" << std::endl;
    file << std::endl;
    // ---- Write flags ----
    file << "writeFlags" << std::endl;
    file << "(" << std::endl;
    file << "    scalarLevels" << std::endl;
    file << "    layerSets" << std::endl;
    file << "    layerFields" << std::endl;
    file << ");" << std::endl;
    file << std::endl;
    // ---- Merge tolerance ----
    file << "mergeTolerance 1E-6;" << std::endl;
    file << std::endl;
    // Closes file.
    file.close();
}

void
Exporter::writeMeshQualityDict() const {
    // Opens file.
    std::string name = "meshQualityDict";
    std::string fileName = dirSystem_ + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    // Writes data.
    file << writeOpenFoamHeader("system", name) << std::endl;
    file << std::endl;
    file << "#include \"$WM_PROJECT_DIR/etc/caseDicts/meshQualityDict\"" << std::endl;
    //	file << "    relaxed" << std::endl;
    //	file << "    {" << std::endl;
    //	file << "        maxNonOrtho 75;" << std::endl;
    //	file << "    }" << std::endl;
    file << "    nSmoothScale 4;" << std::endl;
    file << "    errorReduction 0.75;" << std::endl;
    // Closes file.
    file.close();
}

void
Exporter::writeAllClean() const {
    std::string name = "Allclean";
    std::string fileName = getFilename() + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    file<< "#!/bin/bash" << std::endl;
    file << "source /opt/openfoam230/etc/bashrc" << std::endl;
    file << ". /opt/openfoam230/bin/tools/CleanFunctions" << std::endl;
    file << "rm -rf 0 > /dev/null 2>&1" << std::endl;
    file << "rm -rf constant/extendedFeatureEdgeMesh > /dev/null 2>&1" << std::endl;
    file << "rm -f constant/triSurface/*.eMesh > /dev/null 2>&1" << std::endl;
    file << "rm -f constant/polyMesh/boundary" << std::endl;
    file << "cleanCase" << std::endl;
    // Closes file.
    file.close();
#ifndef _WIN32
    chmod(fileName.c_str(), ALLPERMS);
#endif
}

void
Exporter::writeAllRun() const {
    std::string name = "Allrun";
    std::string fileName = getFilename() + "/" + name;
    std::ofstream file;
    openFile(fileName, file);
    file<< "#!/bin/bash" << std::endl;
    file<< "cd \"$(dirname \"$0\")\"" << std::endl;
    file<< "source /opt/openfoam230/etc/bashrc" << std::endl;
    file<< ". /opt/openfoam230/bin/tools/RunFunctions" << std::endl;
    file<< "runApplication blockMesh" << std::endl;
    //file<< "runApplication surfaceFeatureExtract" << std::endl;
    file<< "runApplication snappyHexMesh -overwrite" << std::endl;
    // Closes file.
    file.close();
#ifndef _WIN32
    chmod(fileName.c_str(), ALLPERMS);
#endif
}

//void
//Exporter::writeAllRunParallel() const {
//   // blockMesh can not run in parallel. Because of this reason we have to
//   // create an initial mesh with nCells/nProc processors and then
//   // use refineMesh. Therefore the following equality must be true:
//   //                 (nCellsX / nProc) % 2 == 0
//   std::string name = "Allrun";
//   std::string fileName = getFilename() + "/" + name;
//   std::ofstream file;
//   openFile(fileName, file);
//   const std::size_t nProc = smb_->solverParams->numberOfProcesses;
//   file<< "#!/bin/bash" << std::endl;
//   file<< "source /opt/openfoam230/etc/bashrc" << std::endl;
//   file<< ". /opt/openfoam230/bin/tools/RunFunctions" << std::endl;
//   file<< "runApplication blockMesh" << std::endl;
//   //file<< "runApplication surfaceFeatureExtract" << std::endl;
//   file<< "runApplication decomposePar" << std::endl;
//   file<< "runParallel refineMesh " << nProc << " -overwrite" << std::endl;
//   file<< "runParallel snappyHexMesh " << nProc << " -overwrite" << std::endl;
//   file<< "runApplication reconstructParMesh -constant" << std::endl;
//   // Closes file.
//   file.close();
//#ifndef _WIN32
//   chmod(fileName.c_str(), ALLPERMS);
//#endif
//}

std::size_t
Exporter::computeRefinableCellDim(
        const std::size_t originalCellDim,
        const std::size_t nProc) const {
    if (nProc == 1) {
        return originalCellDim;
    }
    std::size_t res = originalCellDim;
    bool showWarning = false;
    while ((res/nProc) % 2 != 0) {
        res++;
        showWarning = true;
    }
    if (showWarning) {
        std::cout << "WARNING @ computeRefinableCellDim: "
                  << "The number of cells has changed to make possible parallelization."
                  << "To avoid this message use a number of cells such that "
                  << "NumberOfCellsInDim / numberOfProcessors can be divided by 2."
                  << std::endl;
    }
    return (res/2);
}

std::string
Exporter::intToStr(const std::size_t i) const {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

std::string
Exporter::boolToStr(const bool constBool) const {
    if (constBool) {
        return std::string("true");
    } else {
        return std::string("false");
    }
}

Math::CVecR3 Exporter::computeLocationInMesh() const {
    if (smb_->mesherOptions->isLocationInMeshSet()) {
        return smb_->mesherOptions->getLocationInMesh();
    } else {
        const Geometry::Grid3* g = smb_->grid;
        return g->getPos(g->getNumCells() - 1);
    }
}

}
}
}
