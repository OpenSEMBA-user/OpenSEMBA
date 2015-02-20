/*
 * OutOpenFoam.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: luis
 */

#include "OutputOpenFoam.h"

OutputOpenFoam::OutputOpenFoam() {
   smb_ = NULL;
   bruteForceVolumes_ = false;
}

OutputOpenFoam::OutputOpenFoam(
      const SmbData* smb,
      const bool bruteForceVolumes) {
   bruteForceVolumes_ = bruteForceVolumes;
   smb_ = smb;
   setFilename(smb_->getProjectFolder() + "/openfoam/");
   deleteDirIfExists(getFilename());
   createOpenFoamDirs();
   writeOpenFoamDummyFile();
   writeControlDict();
   writefvSchemes();
   writefvSolution();
   writeAllClean();
   if (smb_->gData->numberOfProcesses == 1) {
      writeAllRun();
   } else {
      writeDecomposeParDict();
      writeAllRunParallel();
   }
   writeSTLs();
   writeBlockMeshDict();
   writeSurfaceFeatureExtractDict();
   writeSnappyHexMeshDict();
   writeMeshQualityDict();
}

OutputOpenFoam::~OutputOpenFoam() {
}

void
OutputOpenFoam::createOpenFoamDirs() {
   mkdir(getFilename().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   dirConstant_ = getFilename() + "constant/";
   mkdir(dirConstant_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   dirPolymesh_ = getFilename() + "constant/polyMesh/";
   mkdir(dirPolymesh_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   dirTriSurface_ = getFilename() + "constant/triSurface/";
   mkdir(dirTriSurface_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   dirSystem_ = getFilename() + "system/";
   mkdir(dirSystem_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void
OutputOpenFoam::writeSTLs() const {
   // Writes materials.
   const uint nMat = smb_->pMGroup->count();
   for (uint i = 0; i < nMat; i++) {
      const PhysicalModel* mat = smb_->pMGroup->get(i);
      if (!mat->isSMA()) {
         bool ignoreTets = false;
         if (mat->isPEC()) {
            ignoreTets = false;
         } else {
            ignoreTets = bruteForceVolumes_;
         }
         const vector<Tri3> tri =
               smb_->mesh->getTriWithMatId(mat->getId(), ignoreTets);
         // Separate in layers.
         const string name = smb_->pMGroup->get(i)->getName();
         triToSTL(tri, dirTriSurface_, "mat", mat->getId(), name);
      }
   }
   // Writes surface output requests.
   //	const uint nOutRq =
   //	 smb_->outputRequests->countWithType(Element::SURFACE);
   //	for (uint i = 0; i < nOutRq; i++) {
   //		const OutputRequest* outRq =
   //		 smb_->outputRequests->getWithType(i, Element::SURFACE);
   //		const vector<Tri3> tri = smb_->mesh->getTriWithId(outRq->getElem());
   //		const string name = outRq->getName();
   //		stringstream num;
   //		num << i;
   //		triToSTL(tri, dirTriSurface + "out." + num.str(), name);
   //	}
   // Writes surface EMSource.
   const uint nEM = smb_->emSources->countWithType(Element::SURFACE);
   for (uint i = 0; i < nEM; i++) {
      const EMSource* em = smb_->emSources->getWithType(i, Element::SURFACE);
      const vector<Tri3> tri = smb_->mesh->getTriWithId(em->getElem());
      const string name = "Waveport";
      triToSTL(tri, dirTriSurface_, "src", i, name);
   }
}

void
OutputOpenFoam::triToSTL(
      const vector<Tri3>& tri,
      const string& folder,
      const string& type,
      const uint& typeId,
      const string& name) const {
   // Separates tri by layers.
   vector<const Element*> elem;
   elem.reserve(tri.size());
   for (uint i = 0; i < tri.size(); i++) {
      elem.push_back(&tri[i]);
   }
   map<uint, vector<const Element*> > layers =
         smb_->mesh->elem.separateLayers(elem);
   // Writes a file for each layer.
   map<uint, vector<const Element*> >::iterator it;
   for (it = layers.begin(); it != layers.end(); ++it) {
      const uint layerId = it->first;
      const vector<const Element*> tri = it->second;
      // Opens file.
      ofstream file;
      string filename;
      filename = folder + type
            + "." + intToStr(typeId) + "." + intToStr(layerId);
      openFile(filename + ".stl", file);
      // Writes data.
      const uint nElem = tri.size();
      string solidName(name);
      if (smb_->layers->get(layerId) != NULL) {
         solidName += "@" + smb_->layers->get(layerId)->getName();
      }
      file << "solid " << solidName << endl;
      for (uint i = 0; i < nElem; i++) {
         file << "facet ";
         //          CVecD3 n = tri[i]->getNormal();
         //          file << " normal " << n(0) << " " << n(1) << " " << n(2)
         file << endl;
         file << "  outer loop" << endl;
         for (uint j = 0; j < 3; j++) {
            CVecD3 v = tri[i]->getVertex(j)->pos();
            file << "    vertex "
                  << v(0) << " "
                  << v(1) << " "
                  << v(2) << endl;
         }
         file << "  endloop" << endl;
         file << "endfacet" << endl;
      }
      file << "endsolid " << solidName << endl;
      // Closes
      file.close();
   }
}

void
OutputOpenFoam::writeOpenFoamDummyFile() const {
   ofstream file;
   openFile(getFilename() + "openfoam.foam", file);
   file.close();
}

void
OutputOpenFoam::writeControlDict() const {
   string name = "controlDict";
   string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   file << writeOpenFoamHeader(string("system"), name) << endl;
   file << endl;
   file << "filestartFrom   startTime;" << endl;
   file << "startTime       latestTime;" << endl;
   file << "stopAt          endTime;" << endl;
   file << "endTime         0;" << endl;
   file << "deltaT          1;" << endl;
   file << "writeControl    timeStep;" << endl;
   file << "writeInterval   1;" << endl;
   file << "purgeWrite      0;" << endl;
   file << "writeFormat     ascii;" << endl;
   file << "writePrecision  6;" << endl;
   file << "writeCompression uncompressed;" << endl;
   file << "timeFormat      general;" << endl;
   file << "timePrecision   6;" << endl;
   file << "runTimeModifiable yes;" << endl;
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writefvSchemes() const {
   const string name = "fvSchemes";
   const string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   file << writeOpenFoamHeader("system", name) << endl;
   file << "gradSchemes {}" << endl;
   file << "divSchemes {}" << endl;
   file << "laplacianSchemes {}" << endl;
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writefvSolution() const {
   const string name = "fvSolution";
   const string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   file << writeOpenFoamHeader("system", name) << endl;
   // Closes file.
   file.close();
}

string
OutputOpenFoam::writeAllBoundary() const {
   string res;
   res =  "allBoundary {\n";
   res += "  type patch; \n";
   res += "    faces ( (3 7 6 2) (0 4 7 3) (2 6 5 1)\n";
   res += "            (1 5 4 0) (0 3 2 1) (4 5 6 7) );\n";
   res += "}";
   return res;
}

void
OutputOpenFoam::writeDecomposeParDict() const {
   // Opens file for writing.
   const string name = "decomposeParDict";
   const string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   // Prepares data.
   uint numberOfProcesses = smb_->gData->numberOfProcesses;
   // Writes file.
   file<< writeOpenFoamHeader("system/", name) << endl
         << endl
         << "numberOfSubdomains " << numberOfProcesses << ";" << endl
         << endl
         << "method hierarchical;" << endl
         << endl
         << "hierarchicalCoeffs {" << endl
         << "    n (" << numberOfProcesses << " 1 1);" << endl
         << "    order xyz;" << endl
         << "    delta 0.001;" << endl
         << "}" << endl
         << endl
         << "distributed no;" << endl;
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writeBlockMeshDict() const {
   // Opens file for writing.
   const string name = "blockMeshDict";
   const string fileName = dirPolymesh_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   // Prepares data.
   const Grid3* grid = smb_->mesh->getGrid();
   if (grid->hasZeroSize()) {
      cerr<< "ERROR @ writeBlockMeshDict: "
            << "Rectilinear grid has zero size." << endl;
   }
   uint nProc = smb_->gData->numberOfProcesses;
   CVecD3 min(grid->getFullDomainBoundingBox().getMin());
   CVecD3 max(grid->getFullDomainBoundingBox().getMax());
   // blockMesh can not run in parallel. Because of this reason we have to
   // create an initial mesh with nCells/nProc processors and then
   // use refineMesh. Therefore the following equality must be true:
   //                 (nCellsX / nProc) % 2 == 0
   // This is ensured by computeRefinableCellDim.
   long int dimX = computeRefinableCellDim(grid->getNumCells()(0), nProc);
   long int dimY = computeRefinableCellDim(grid->getNumCells()(1), nProc);
   long int dimZ = computeRefinableCellDim(grid->getNumCells()(2), nProc);
   // Writes file.
   file << writeOpenFoamHeader("constant/polyMesh", name) << endl;
   // Scaling should have already been applied to mesh. It is not
   // necessary to modify this.
   file << "convertToMeters 1;" << endl;
   file << endl;
   // Vertices.
   file << "vertices" << endl;
   file << "(" << endl;
   file << "    ( " << min(0) << " " << min(1) << " " << min(2) << ")" << endl;
   file << "    ( " << max(0) << " " << min(1) << " " << min(2) << ")" << endl;
   file << "    ( " << max(0) << " " << max(1) << " " << min(2) << ")" << endl;
   file << "    ( " << min(0) << " " << max(1) << " " << min(2) << ")" << endl;
   file << "    ( " << min(0) << " " << min(1) << " " << max(2) << ")" << endl;
   file << "    ( " << max(0) << " " << min(1) << " " << max(2) << ")" << endl;
   file << "    ( " << max(0) << " " << max(1) << " " << max(2) << ")" << endl;
   file << "    ( " << min(0) << " " << max(1) << " " << max(2) << ")" << endl;
   file << ");" << endl;
   file << endl;
   // Blocks.
   file << "blocks" << endl;
   file << "(" << endl;
   file << "    hex (0 1 2 3 4 5 6 7) ("
         << dimX << " " << dimY << " " << dimZ
         << ") simpleGrading (1 1 1)" << endl;
   file << ");" << endl;
   file << endl;
   // Edges.
   file << "edges" << endl;
   file << "(" << endl;
   file << ");" << endl;
   file << endl;
   // Boundary.
   file << "boundary" << endl;
   file << "(" << endl;
   file << "  xMin {type symmetryPlane; faces ((0 4 7 3));}" << endl;
   file << "  yMin {type symmetryPlane; faces ((1 5 4 0));}" << endl;
   file << "  zMin {type symmetryPlane; faces ((0 1 2 3));}" << endl;
   file << "  xMax {type symmetryPlane; faces ((1 5 6 2));}" << endl;
   file << "  yMax {type symmetryPlane; faces ((2 3 7 6));}" << endl;
   file << "  zMax {type symmetryPlane; faces ((4 5 6 7));}" << endl;
   file << ");" << endl;
   // Closes file.
   file.close();
}

string
OutputOpenFoam::writeOpenFoamHeader(
      const string& location,
      const string& object) const {
   string res;
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
OutputOpenFoam::writeSurfaceFeatureExtractDict() const {
   // Opens file.
   string name = "surfaceFeatureExtractDict";
   string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   // Prepares data.
   vector<string> stlFile = getFilesBasenames(dirTriSurface_, ".stl");
   const uint nSTLs = stlFile.size();
   // Writes data.
   file << writeOpenFoamHeader("system", name) << endl;
   file << endl;
   for (uint i = 0; i < nSTLs; i++) {
      file<< stlFile[i] << endl
            << "{" << endl
            << "    extractionMethod    extractFromSurface;" << endl
            << "    extractFromSurfaceCoeffs" << endl
            << "    {" << endl
            << "        includedAngle "
            << smb_->ofParams->getEdgeFeatureAngle() << ";" << endl
            << "    }" << endl
            << "    writeObj true;" << endl
            << "}" << endl
            << endl;
   }
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writeSnappyHexMeshDict() const {
   // Opens file.
   string name = "snappyHexMeshDict";
   string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   // Prepares data.
   vector<string> stlFile = getFilesBasenames(dirTriSurface_, ".stl");
   const uint nSTLs = stlFile.size();
   // Writes data.
   file << writeOpenFoamHeader("system", name) << endl;
   file << endl;
   file << "castellatedMesh "
         << boolToStr(smb_->ofParams->isCastellateMesh()) << ";" << endl;
   file << "snap            "
         << boolToStr(smb_->ofParams->isSnapMesh()) << ";" << endl;
   file << "addLayers       "
         << boolToStr(smb_->ofParams->isAddLayers()) << ";" << endl;
   file << endl;
   // ---- Geometry ----
   file << "geometry" << endl;
   file << "{" << endl;
   for (uint i = 0; i < nSTLs; i++) {
      string matName = removeExtension(stlFile[i]);
      file << "    " << stlFile[i] << endl;
      file << "    {" << endl;
      file << "        type triSurfaceMesh;" << endl;
      file << "        name " << matName << ";" << endl;
      file << "    }" << endl;
   }
   file << "};" << endl;
   file << endl;
   // ---- Castellated mesh controls ----
   file << "castellatedMeshControls" << endl;
   file << "{" << endl;
   file << "    maxLocalCells 100000;" << endl;
   file << "    maxGlobalCells 2000000;" << endl;
   file << "    minRefinementCells 0;" << endl;
   file << "    nCellsBetweenLevels 1;" << endl;

   file << endl;
   // Explicit features.
   file << "    features" << endl;
   file << "    (" << endl;
   //	for (uint i = 0; i < nSTLs; i++) {
   //		file<< "        {" << endl
   //		 << "            file \"" << removeExtension(stlFile[i])
   //		 << ".extendedFeatureEdgeMesh\";" << endl
   //		 << "            level "
   //		 << smb_->ofParams->getFeatureRefinementLevel()<< ";" << endl
   //		 << "        }" << endl;
   //	}
   file << "    );" << endl;
   file << endl;
   // Refinement surfaces.
   file << "    refinementSurfaces" << endl;
   file << "    {" << endl;
   for (uint i = 0; i < nSTLs; i++) {
      file << "        " << removeExtension(stlFile[i]) << endl;
      file << "        {" << endl;
      file << "            level (0 0);" << endl;
      file << "        }" << endl;
      // TODO If it is a surface we have to put:
      /* faceZone mat.1;
		   faceType baffle;
		   cellZone mat.1;
		   cellZoneInside inside;
       */
      // To obtain a non duplicated surface when snapping is activated.
   }
   file << "    }" << endl;
   file << endl;
   file << "    resolveFeatureAngle 30;" << endl;
   file << endl;
   // Refinement regions.
   file << "    refinementRegions" << endl;
   file << "    {" << endl;
   file << "    }" << endl;
   // Mesh selection.
   CVecD3 loc = computeLocationInMesh();
   file << "   locationInMesh "
         << "(" << loc(0) << " " << loc(1) << " " << loc(2) << ");" << endl;
   file << endl;
   file << "   allowFreeStandingZoneFaces true;" << endl;
   file << "}" << endl; // Closes castellated mesh controls.
   file << endl;
   // ---- Snap controls ----
   file << "snapControls" << endl;
   file << "{" << endl;
   file << "    nSmoothPatch 3;" << endl;
   file << "    tolerance 1.5;" << endl;
   file << "    nSolveIter 150;" << endl;
   file << "    nRelaxIter 5;" << endl;
   file << "    nFeatureSnapIter 10;" << endl;
   file << "    implicitFeatureSnap false;" << endl;
   file << "    explicitFeatureSnap true;" << endl;
   file << "    multiRegionFeatureSnap true;" << endl;
   file << "}" << endl;
   file << endl;
   // ---- addLayers controls ----
   file << "addLayersControls" << endl;
   file << "{" << endl;
   file << "    relativeSizes true;" << endl;
   file << "    layers { }" << endl;
   file << "    expansionRatio 1.0;" << endl;
   file << "    finalLayerThickness 0.3;" << endl;
   file << "    minThickness 0.25;" << endl;
   file << "    nGrow 0;" << endl;
   file << "    featureAngle 30;" << endl;
   file << "    nRelaxIter 5;" << endl;
   file << "    nSmoothSurfaceNormals 1;" << endl;
   file << "    nSmoothNormals 3;" << endl;
   file << "    nSmoothThickness 10;" << endl;
   file << "    maxFaceThicknessRatio 0.5;" << endl;
   file << "    maxThicknessToMedialRatio 0.3;" << endl;
   file << "    minMedianAxisAngle 90;" << endl;
   file << "    nBufferCellsNoExtrude 0;" << endl;
   file << "    nLayerIter 50;" << endl;
   file << "}" << endl;
   file << endl;
   // ---- meshQuality control ----
   file << "meshQualityControls" << endl;
   file << "{" << endl;
   file << "    #include \"meshQualityDict\"" << endl;
   file << "}" << endl;
   file << endl;
   // ---- Write flags ----
   file << "writeFlags" << endl;
   file << "(" << endl;
   file << "    scalarLevels" << endl;
   file << "    layerSets" << endl;
   file << "    layerFields" << endl;
   file << ");" << endl;
   file << endl;
   // ---- Merge tolerance ----
   file << "mergeTolerance 1E-6;" << endl;
   file << endl;
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writeMeshQualityDict() const {
   // Opens file.
   string name = "meshQualityDict";
   string fileName = dirSystem_ + "/" + name;
   ofstream file;
   openFile(fileName, file);
   // Writes data.
   file << writeOpenFoamHeader("system", name) << endl;
   file << endl;
   file << "#include \"$WM_PROJECT_DIR/etc/caseDicts/meshQualityDict\"" << endl;
   //	file << "    relaxed" << endl;
   //	file << "    {" << endl;
   //	file << "        maxNonOrtho 75;" << endl;
   //	file << "    }" << endl;
   file << "    nSmoothScale 4;" << endl;
   file << "    errorReduction 0.75;" << endl;
   // Closes file.
   file.close();
}

void
OutputOpenFoam::writeAllClean() const {
   string name = "Allclean";
   string fileName = getFilename() + "/" + name;
   ofstream file;
   openFile(fileName, file);
   file<< "#!/bin/bash" << endl;
   file << "source /opt/openfoam230/etc/bashrc" << endl;
   file << ". /opt/openfoam230/bin/tools/CleanFunctions" << endl;
   file << "rm -rf 0 > /dev/null 2>&1" << endl;
   file << "rm -rf constant/extendedFeatureEdgeMesh > /dev/null 2>&1" << endl;
   file << "rm -f constant/triSurface/*.eMesh > /dev/null 2>&1" << endl;
   file << "rm -f constant/polyMesh/boundary" << endl;
   file << "cleanCase" << endl;
   // Closes file.
   file.close();
   chmod(fileName.c_str(), ALLPERMS);
}

void
OutputOpenFoam::writeAllRun() const {
   string name = "Allrun";
   string fileName = getFilename() + "/" + name;
   ofstream file;
   openFile(fileName, file);
   file<< "#!/bin/bash" << endl;
   file<< "cd \"$(dirname \"$0\")\"" << endl;
   file<< "source /opt/openfoam230/etc/bashrc" << endl;
   file<< ". /opt/openfoam230/bin/tools/RunFunctions" << endl;
   file<< "runApplication blockMesh" << endl;
   //file<< "runApplication surfaceFeatureExtract" << endl;
   file<< "runApplication snappyHexMesh -overwrite" << endl;
   // Closes file.
   file.close();
   chmod(fileName.c_str(), ALLPERMS);
}

void
OutputOpenFoam::writeAllRunParallel() const {
   // blockMesh can not run in parallel. Because of this reason we have to
   // create an initial mesh with nCells/nProc processors and then
   // use refineMesh. Therefore the following equality must be true:
   //                 (nCellsX / nProc) % 2 == 0
   string name = "Allrun";
   string fileName = getFilename() + "/" + name;
   ofstream file;
   openFile(fileName, file);
   const uint nProc = smb_->gData->numberOfProcesses;
   file<< "#!/bin/bash" << endl;
   file<< "source /opt/openfoam230/etc/bashrc" << endl;
   file<< ". /opt/openfoam230/bin/tools/RunFunctions" << endl;
   file<< "runApplication blockMesh" << endl;
   //file<< "runApplication surfaceFeatureExtract" << endl;
   file<< "runApplication decomposePar" << endl;
   file<< "runParallel refineMesh " << nProc << " -overwrite" << endl;
   file<< "runParallel snappyHexMesh " << nProc << " -overwrite" << endl;
   file<< "runApplication reconstructParMesh -constant" << endl;
   // Closes file.
   file.close();
   chmod(fileName.c_str(), ALLPERMS);
}

uint
OutputOpenFoam::computeRefinableCellDim(
      const uint originalCellDim,
      const uint nProc) const {
   if (nProc == 1) {
      return originalCellDim;
   }
   uint res = originalCellDim;
   bool showWarning = false;
   while ((res/nProc) % 2 != 0) {
      res++;
      showWarning = true;
   }
   if (showWarning) {
      cout<< "WARNING @ computeRefinableCellDim: "
            << "The number of cells has changed to make possible parallelization."
            << "To avoid this message use a number of cells such that "
            << "NumberOfCellsInDim / numberOfProcessors can be divided by 2."
            << endl;
   }
   return (res/2);
}

string
OutputOpenFoam::intToStr(const uint i) const {
   stringstream ss;
   ss << i;
   return ss.str();
}

string
OutputOpenFoam::boolToStr(const bool constBool) const {
   if (constBool) {
      return string("true");
   } else {
      return string("false");
   }
}

CVecD3
OutputOpenFoam::computeLocationInMesh() const {
   if (smb_->ofParams->isLocationInMeshSet()) {
      return smb_->ofParams->getLocationInMesh();
   } else {
      const Grid3* grid = smb_->mesh->getGrid();
      return grid->getPositionOfNaturalCell(grid->getNumCells() - 1);
   }
}
