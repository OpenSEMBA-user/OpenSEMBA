/*
 * GiDResult.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */
#ifndef GIDRESULT_H_
#include "ResultGiD.h"
#endif

ResultGiD::ResultGiD() {
   mesh_ = NULL;
   dg_ = NULL;
}

ResultGiD::~ResultGiD() {

}

ResultGiD::ResultGiD(
      const OutRq* p,
      int& coordCounter,
      int& elemCounter,
      const DG* dg,
      const Mesh* mesh) : OutRq(*p) {
   dg_ = dg;
   mesh_ = mesh;
   switch (p->getElementType()) {
   case Element::NODE:
      writePointProbeMesh(coordCounter, elemCounter);
      break;
   case Element::surface:
      writeTriProbeMesh(coordCounter, elemCounter);
      break;
   case Element::volume:
      writeTetProbeMesh(coordCounter, elemCounter);
      break;
   default:
      cerr << endl << "ERROR @ GiDResult ctor:" << endl;
      assert(false);
      break;
   }
}

void
ResultGiD::writePointProbeMesh(int& coordCounter, int& elemCounter) {
   OutputGiD::beginMesh(getName(), GiD_3D, GiD_Point, 1);
   GiD_BeginCoordinates();
   assert(getElem().size() == 1);
   const CVecD3 pos = mesh_->v.getPtrToId(getElem()[0])->pos();
   GiD_WriteCoordinates(++coordCounter, pos(0), pos(1), pos(2));
   GiD_EndCoordinates();
   GiD_BeginElements();
   GiD_WriteElement(++elemCounter, &coordCounter);
   GiD_EndElements();
   GiD_EndMesh();
   coord_.push_back(coordCounter);
   assert(elem_.size() > 0);
   vector<pair<const Element*, uint> > vertex =
         mesh_->getElementsWithVertex(elem_[0], Element::volume);
   assert(vertex.size() > 0);
   solverNode_.push_back(dg_->getGlobalFieldPosOfVertex(vertex[0]));
}

void
ResultGiD::writeTriProbeMesh(
      int& coordCounter,
      int& elemCounter) {
   uint nV;
   mesh_->isLinear() ? nV = 3 : nV = 6;
   OutputGiD::beginMesh(getName(), GiD_3D, GiD_Triangle, nV);
   GiD_BeginCoordinates();
   int tmpCounter = coordCounter;
   static const uint GiDTriOrder[6] = {0, 3, 5, 1, 4, 2};
   for (uint i = 0; i < elem_.size(); i++) {
      pair<const Volume*, uint> surf = mesh_->map.getInnerFace(elem_[i]);
      const Volume* vol = surf.first;
      uint f = surf.second;
      for (uint i = 0; i < nV; i++) {
         CVecD3 vertex;
         if (mesh_->isLinear()) {
            vertex = vol->getSideVertex(f,i)->pos();
         } else {
            vertex = vol->getSideV(f, GiDTriOrder[i])->pos();
         }
         GiD_WriteCoordinates(++coordCounter, vertex(0), vertex(1), vertex(2));
      }
   }
   GiD_EndCoordinates();
   GiD_BeginElements();
   vector<int> nId(nV);
   for (uint j = 0; j < elem_.size(); j++) {
      for (uint i = 0; i < nId.size(); i++) {
         nId[i] = ++tmpCounter;
         coord_.push_back(nId[i]);
      }
      GiD_WriteElement(++elemCounter, &nId[0]);
   }
   GiD_EndElements();
   GiD_EndMesh();
   for (uint i = 0; i < elem_.size(); i++) {
      pair<const Volume*, uint> face = mesh_->map.getInnerFace(elem_[i]);
      vector<uint> node = dg_->getGlobalFieldPosOfFace(face);
      solverNode_.insert(solverNode_.end(), node.begin(), node.end());
   }
}

void
ResultGiD::writeTetProbeMesh(int& coordCounter, int& elemCounter) {
   uint nV;
   mesh_->isLinear() ? nV = 4 : nV = 10;
   assert(nV == 4); // TODO Implemented only for linear meshes.
   OutputGiD::beginMesh(getName(), GiD_3D, GiD_Tetrahedra, nV);
   GiD_BeginCoordinates();
   int tmpCounter = coordCounter;
   for (uint j = 0; j < elem_.size(); j++) {
      for (uint i = 0; i < nV; i++ ) {
         const CVecD3 pos
         = mesh_->getElementWithId(elem_[i])->getVertex(i)->pos();
         GiD_WriteCoordinates(++coordCounter, pos(0), pos(1), pos(2));
      }
   }
   GiD_EndCoordinates();
   GiD_BeginElements();
   vector<int> nId(nV);
   for (uint j = 0; j < elem_.size(); j++) {
      for (uint i = 0; i < nId.size(); i++) {
         nId[i] = ++tmpCounter;
         coord_.push_back(nId[i]);
      }
      GiD_WriteElement(++elemCounter, &nId[0]);
   }
   GiD_EndElements();
   GiD_EndMesh();
   for (uint i = 0; i < elem_.size(); i++) {
      vector<uint> node = dg_->getGlobalFieldPosOfVolume((elem_[i]));
      solverNode_.insert(solverNode_.end(), node.begin(), node.end());
   }
}

void
ResultGiD::write(
      const double time,
      const FieldD3& electric,
      const FieldD3& magnetic) const {
   OutputGiD::beginResult(
         getName() + " "  + outputTypeStr(),
         "Time", time, getGiDResultType(),
         getGiDResultLocation(), getGiDGaussPointType(), getComponentNames());
   for (uint k = 0; k < solverNode_.size(); k++) {
      const uint j = solverNode_[k];
      const CVecD3 vec
      = getOutputValueFromFields(electric.getCVec(j), magnetic.getCVec(j));
      GiD_WriteVectorModule(coord_[k], vec(0), vec(1), vec(2), vec.norm());
   }
   GiD_EndResult();
}

CVecD3
ResultGiD::getOutputValueFromFields(
      const CVecD3& electric,
      const CVecD3& magnetic) const {
   switch (getOutputType()) {
   case OutRq::electricField:
      return electric;
      break;
   case OutRq::magneticField:
      return magnetic;
      break;
   default:
      cerr << endl << "ERROR @ ResultGiD::getOutputValueFromFields: "
      << "Not implemented for output type: " << outputTypeStr() << endl;
      return CVecD3();
   }
}

string ResultGiD::getGiDGaussPointType() const {
   switch (getElementType()) {
   case Element::NODE:
      return string("gp_point");
   case Element::surface:
      return string("gp_tri");
   case Element::volume:
      return string("gp_tet");
   default:
      assert(false);
      return string();
   }
}

vector<string> ResultGiD::getComponentNames() const {
   string letter;
   switch (getOutputType()) {
   case OutRq::electricField:
      letter = 'E';
      break;
   case OutRq::magneticField:
      letter = 'H';
      break;
   default:
      letter = 'X';
      break;
   }
   vector<string> res;
   switch (getGiDResultType()) {
   case GiD_ResultType::GiD_Vector:
      res.resize(4);
      res[0] = letter + 'x';
      res[1] = letter + 'y';
      res[2] = letter + 'z';
      res[3] = '|' + letter + '|';
      break;
   default:
      res.resize(1);
      res[0] = letter;
      break;
   }
   return res;
}
