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
#include "../exporter/Exporter.h"

Exporter::Exporter()  {
}

Exporter::Exporter(const string& name) : ProjectFile(name) {

}

Exporter::~Exporter() {
    for (UInt i = 0; i < result_.size(); i++) {
        delete result_[i];
    }
}

//void
//Output::writeResumeFile(
//        const Real time, const FieldR3& electric, const FieldR3& magnetic) {
//    string auxStr = getFolder() + getProjectName() + ".resume";
//    char *auxChar;
//    auxChar = new char[auxStr.length() + 1];
//    strcpy(auxChar, auxStr.c_str());
//    GiD_OpenPostResultFile(auxChar, GiD_PostAscii);
//    {
//        const char * compv[] = {"Ex", "Ey", "Ez", "|E|"};
//        const char * fName = "Electric Field";
//        const char * tName = "Time";
//        GiD_BeginResult(fName, tName, time,
//                GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
//        writeAllFields(electric);
//        GiD_EndResult();
//    }
//    {
//        const char * compv[] = {"Hx", "Hy", "Hz", "|H|"};
//        const char * fName = "Magnetic Field";
//        const char * tName = "Time";
//        GiD_BeginResult(fName, tName, time,
//                GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
//        writeAllFields(magnetic);
//        GiD_EndResult();
//    }
//    GiD_ClosePostResultFile();
//    cout << " - File for resuming was succesfully written." << endl;
//}

void Exporter::process(
        const Real time,
        const FieldR3& electric,
        const FieldR3& magnetic) {
    for (UInt i = 0; i < result_.size(); i++) {
        result_[i]->write(time, electric, magnetic);
    }
}

void Exporter::deleteExistentOutputFiles() const {
    string file;
    file = getFilename() + ".post.msh";
    remove(file.c_str());
    file = getFilename() + ".post.res";
    remove(file.c_str());
}

void Exporter::printInfo() const {
    cout << " --- Exporter info --- " << endl;
    cout << " --- End of Exporter info --- " << endl;
}

//void Output::writeAllFields(const FieldR3& field) const {
//    for (UInt i = 0; i < field.size(); i++) {
//        CVecR3 vec = field.getCVec(i);
//        GiD_WriteVector(i, vec(0), vec(1), vec(2));
//    }
//}

string Exporter::getOutputfilename() const {
    string folder = getFolder();
    string output = getOutputName();
    return folder + output;
}


Group<ElemR> Exporter::getBoundary(
        const CartesianAxis dir,
        const CartesianBound pos,
        CoordR3Group& cG,
        const Grid3* grid,
        const Mesh* mesh,
        const OptionsMesher* opts) const {
    BoxR3 box;
    if (grid != NULL) {
        box = grid->getFullDomainBoundingBox();
    } else {
        box = mesh->getBoundingBox();
    }
    Group<ElemR> elem;
    BoxR3 quadBox = box.getBoundAsBox(dir,pos);
    elem.add(new QuaR4(cG, ElementId(0), quadBox));
    assert(elem.size() != 0);
    return elem;
}

string Exporter::getBoundaryName(
        const OptionsMesher* opts,
        const UInt i,
        const UInt j) {
    OptionsMesher::BoundType boundType = opts->getBoundTermination(i, j);
    const string boundName = OptionsMesher::toStr(boundType);
    stringstream name;
    name << boundName + "@Boundary" << i << j;
    return name.str();
}

Group<ElemR> Exporter::getGridElems(
        CoordR3Group& cG,
        const Grid3* grid) const {
    if (grid == NULL) {
        return Group<ElemR>();
    }
    Group<ElemR> elem;
    BoxR3 box = grid->getFullDomainBoundingBox();
    if (grid != NULL) {
        for (UInt d = 0; d < 3; d++) {
            // Generates grid as quads.
//            vector<BoxR3> quadBoxes =
//                    box.getBoundAsBox(CartesianAxis(d),L).chop(*grid);
//            vector<QuaR4*> quads(quadBoxes.size());
//            for (UInt i = 0; i < quadBoxes.size(); i++) {
//                quads[i] = new QuaR4(cG, ElementId(0), quadBoxes[i]);
//            }
//            elem.addId(quads);
            // Generates grid as lines.
            for (UInt i = 0; i < 2; i++) {
                vector<Real> pos = grid->getPos((d+i+1)%3);
                for (UInt j = 0; j < pos.size(); j++) {
                    CVecR3 pMin, pMax;
                    pMin(d) = grid->getPos(d,L);
                    pMin((d+i+1)%3) = pos[j];
                    pMax = pMin;
                    pMin((d-i+2)%3) = grid->getPos((d-i+2)%3).front();
                    pMax((d-i+2)%3) = grid->getPos((d-i+2)%3).back();
                    elem.add(new LinR2(cG, ElementId(0), BoxR3(pMin,pMax)));
                }
            }
        }
    } else {
        elem.add(new QuaR4(cG, ElementId(0), box));
    }
    elem.add(new HexR8(cG, ElementId(0), box));
    return elem;
}
