/*
 * ParserSTL.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#include "ParserSTL.h"

ParserSTL::ParserSTL() {
    setDefaultValues_();
    mesh_ = NULL;
}

ParserSTL::ParserSTL(const string& fn)
:   ProjectFile(fn) {
    setDefaultValues_();
    if (!is_stl ()) {
        cerr << endl << "ERROR @ ParserSTL: "
                << "File is not an STL." << endl;
    }
}

ParserSTL::~ParserSTL() {

}

SmbData*
ParserSTL::read() {
    SmbData* res = new SmbData();
    res->setFilename(getFilename());
    mesh_ = readMesh();
    res->mesh = mesh_;
    return res;
}

void
ParserSTL::printInfo() const {

}

MeshUnstructured*
ParserSTL::readMesh() {
    GroupLayers<> lG = readLayers();
    GroupCoordinates<CoordR3> cG = readCoordinates();
    GroupElements<ElemR> eG = readElements(cG);
    return new MeshUnstructured(cG, eG, lG);
}

GroupLayers<> ParserSTL::readLayers() {
//    bool finished = false;
//    bool found = false;
//    string label, value;
//    LayerId id;
    GroupLayers<> res;
//    while (!found && !f_in.eof() ) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Layers")==0) {
//            found = true;
//            while(!finished && !f_in.eof()) {
//                string line;
//                getline(f_in, line);
//                if (line.find("End of layers") != line.npos) {
//                    finished = true;
//                } else {
//                    stringstream ss(line);
//                    ss >> id >> value;
//                    res.add(new Layer(id, value));
//                }
//            }
//        }
//    }
//    if (!found) {
//        cerr << endl << "ERROR @ Parsing layers: "
//                << "Layers label was not found." << endl;
//        return res;
//    }
    return res;
}

GroupCoordinates<CoordR3> ParserSTL::readCoordinates() {
    string line;
    CoordinateId id;
    CVecR3 pos;
    vector<CoordR3*> coord;
//    coord.reserve(pSize_.v);
//    bool finished = false;
//    bool found = false;
//    while (!found && !f_in.eof() && !finished) {
//        getline(f_in, line);
//        if (line.find("Coordinates:") != line.npos) {
//            found = true;
//            // Reads coordinates.
//            for (UInt i = 0; i < pSize_.v; i++) {
//                f_in >> id >> pos(0) >> pos(1) >> pos(2);
//                coord.push_back(new CoordR3(id, pos));
//            }
//            // Checks "end of coordinates" label.
//            finished = false;
//            while(!finished && !f_in.eof()) {
//                getline(f_in, line);
//                if (line.find("End of coordinates") != line.npos) {
//                    finished = true;
//                }
//            }
//        }
//    }
//    if (!found) {
//        cerr << endl << "ERROR @ GiDParser::readCoordinates(): "
//                << "Coordinates label was not found." << endl;
//    }
//    if (!finished) {
//        cerr << endl << "ERROR @ GiDParser::readCoordinates(): "
//                << "End of coordinates label not found." << endl;
//    }
    return GroupCoordinates<CoordR3>(coord);
}

GroupElements<ElemR> ParserSTL::readElements(
        const GroupCoordinates<CoordR3>& v) {
//    string line, label;
//    bool finished = false;
//    bool found = false;
    vector<ElemR*> elems;
//    while (!finished && !f_in.eof()) {
//        getline(f_in, line);
//        if (line.find("Elements:") != line.npos) {
//            found = true;
//        }
//        readTri3Elements(v, elems);
//    }
    return GroupElements<ElemR>(elems);
}

void ParserSTL::readTri3Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {

}

void ParserSTL::setDefaultValues_(void){
    nodeTrig_   = NULL;
    node_       = NULL;
    coordx_     = NULL;
    coordy_     = NULL;
    coordz_     = NULL;
    dimsCoord_  = 0;
    numTrig_    = 0;
    areaTrig_   = NULL;
    numSolid_   = 0;
}


void ParserSTL::deleteFull (void){
    if(node_   !=NULL) { delete [] node_;   }
    if(coordx_ !=NULL) { delete [] coordx_; }
    if(coordy_ !=NULL) { delete [] coordy_; }
    if(coordz_ !=NULL) { delete [] coordz_; }
    if(nodeTrig_!=NULL){
        for(long int n=0; n<numTrig_; n++){
            if(nodeTrig_[n]!=NULL){
                delete [] nodeTrig_[n];
            }
        }
        delete [] nodeTrig_;
    }
    if(areaTrig_!=NULL){delete[] areaTrig_;}
    setDefaultValues_();
}

bool ParserSTL::stlParser (void){
    return parser_ ();
}

bool ParserSTL::parser_(void){

    if(is_stl ()){}else{return false;}

    dimsCoord_ = numTrig_*3;
    node_      = new long int [dimsCoord_];
    coordx_    = new double   [dimsCoord_];
    coordy_    = new double   [dimsCoord_];
    coordz_    = new double   [dimsCoord_];

    solidId_.reserve(numTrig_+1);

    ifstream fileId;
    string   fileLine;

    fileId.open (getFilename().c_str());
    if(fileId.is_open()){}else{
        return false;
    }

    long int counter; counter=0;
    long int solidCounter; solidCounter =0;


    while(leapBlank(fileId, fileLine)){
        if(fileLine.compare(0,5,"solid")==0){
            string subSt1, subSt2;
            std::size_t pos1, pos2;

            subSt1 = fileLine.substr(5);
            pos1 = subSt1.find_first_not_of (" \f\n\r\t\v.");

            if(pos1>subSt1.length()){
                stringstream name;
                name<<"layer_"<<solidCounter<<endl;
                subSt2 = name.str();
            }else{
                pos2 = subSt1.find_last_not_of  (" \f\n\r\t\v.");
                if(pos2>subSt1.length()){
                    pos2 = subSt1.length();
                }
                subSt2 = subSt1.substr(pos1,pos2-pos1+1);
            }
            solidName_.push_back(subSt2);
        }else{
            break;
        }
        while(leapBlanktrim(fileId, fileLine)){
            if(fileLine.compare(0,8,"endsolid")==0){
                solidCounter ++ ;
                break;
            }
            if(fileLine.compare(0,12,"facet normal")==0){
                solidId_.push_back(solidCounter);
                if(leapBlanktrim(fileId, fileLine)){}else {
                    return false;
                }
                if(fileLine.compare(0,10,"outer loop")==0){
                }else {
                    return false;
                }

                short int nVertex = 0;
                if(leapBlanktrim(fileId, fileLine)){}else {
                    return false;
                }
                while(fileLine.compare(0,6,"vertex")==0){

                    stringstream newline(fileLine);
                    string cad;
                    newline>>cad>>coordx_[counter]>>
                    coordy_[counter]>>coordz_[counter];
                    node_ [counter] = counter;
                    counter ++;

                    if(leapBlanktrim(fileId, fileLine)){}else {
                        return false;
                    }
                    nVertex ++;
                }
                if(nVertex==3){}else{return false;}
                if(fileLine.compare(0,7,"endloop")==0){
                }else {
                    return false;
                }

                if(leapBlanktrim(fileId, fileLine)){
                }else {
                    return false;
                }
                if(fileLine.compare(0,8,"endfacet")==0){
                }else {
                    return false;
                }
            }else{
                return false;
            }
        }
    }
    collaps_ ();
    fileId.close();
    return true;

}

//=============================================================================
//------ collaps nodes  -------------------------------------------------------
//=============================================================================

void ParserSTL::collaps_ (void){

    double lmin,areaLimit;

    buildAreaTrig();

    areaLimit = meanAreaTrig*1.0e-4;
    lmin = lTrigMin (areaLimit);

    NumColapsNodes_ = 0;
    if(false){ //topologicalMeshflag
      for(long int n=0; n<dimsCoord_; n++){
         for(long int m=n+1; m<dimsCoord_; m++){
            if(node_[m]>=m){
               if(intoSphere(n,m,lmin)){
                  node_[m] = node_[n];
                  NumColapsNodes_ ++;
               }
            }
         }
      }
    }

    nodeTrig_ = new long int* [numTrig_];
    for (long int n=0; n<numTrig_; n++){
        nodeTrig_[n] = new long int [4];
    }

    for (long int n=0; n<numTrig_; n++){
        nodeTrig_[n][0] = node_ [n*3  ];
        nodeTrig_[n][1] = node_ [n*3+1];
        nodeTrig_[n][2] = node_ [n*3+2];
        nodeTrig_[n][3] = 0; //label
    }

    long int nodesMax;
    nodesMax = 0;
    for(long int n=0; n<dimsCoord_; n++){
        if(node_[n]>nodesMax){nodesMax = node_[n];}
    }

    for(long int n=1; n<dimsCoord_; n++){
        coordx_[node_[n]] = coordx_[n];
        coordy_[node_[n]] = coordy_[n];
        coordz_[node_[n]] = coordz_[n];
    }
    dimsCoord_ = nodesMax+1;

    return;

}

bool ParserSTL::intoSphere (const long int &n, const long int &m, const double &radius)const{

    double dx, dy, dz, dr;
    dx = coordx_[n]-coordx_[m];
        if(dx>radius){return false;}
    dy = coordy_[n]-coordy_[m];
        if(dy>radius){return false;}
    dz = coordz_[n]-coordz_[m];
        if(dz>radius){return false;}

    dr = sqrt(dx*dx+dy*dy+dz*dz);
    if(dr<radius){
        return true;
    }else {
        return false;
    }
}

double ParserSTL::dst (const long int &n, const long int &m) const{
    double dx, dy, dz, dr;
    dx = coordx_[n]-coordx_[m];
    dy = coordy_[n]-coordy_[m];
    dz = coordz_[n]-coordz_[m];

    dr = sqrt(dx*dx+dy*dy+dz*dz);
    return dr;
}

double ParserSTL::lTrigMin (const double areaLimit)const {
    double lmin;
    double l[3], lminLoc;
    //long int ni,nf;
    //ni=0; nf=1;
    lmin =1e50;
    for(long int n=0; n<numTrig_; n++){
        if(areaTrig_[n]>areaLimit){
            long int n0,n1,n2;

            n0 = 3*n; n1 = 3*n+1; n2 = 3*n+2;
            l[0] = dst(n0,n1);
            l[1] = dst(n0,n2);
            l[2] = dst(n1,n2);
            lminLoc = +1e15;
            if(l[0]<lminLoc) { if(l[0]>0.0)   {lminLoc = l[0];    } }
            if(l[1]<lminLoc) { if(l[1]>0.0)   {lminLoc = l[1];    } }
            if(l[2]<lminLoc) { if(l[2]>0.0)   {lminLoc = l[2];    } }
            if(lminLoc<lmin){ if(lminLoc>0.0) {lmin    = lminLoc; } }
        }
    }

    return lmin;
}

double ParserSTL::areaTrig (const long int &nele)const{
    double v01x_, v02x_, v01y_, v02y_, v01z_, v02z_, module;
    double n[3];
    long int n0,n1,n2;
    n0 = 3*nele; n1 = 3*nele+1; n2 = 3*nele+2;
    v01x_ =coordx_[n0]-coordx_[n1];
    v01y_ =coordy_[n0]-coordy_[n1];
    v01z_ =coordz_[n0]-coordz_[n1];
    v02x_ =coordx_[n0]-coordx_[n2];
    v02y_ =coordy_[n0]-coordy_[n2];
    v02z_ =coordz_[n0]-coordz_[n2];
    n[0] = v01y_*v02z_-v01z_*v02y_;
    n[1] = v01z_*v02x_-v01x_*v02z_;
    n[2] = v01x_*v02y_-v01y_*v02x_;
    module = sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2])*0.5;
    return module;
}

void ParserSTL::buildAreaTrig(void){

areaTrig_ = new double [numTrig_];

#ifdef compileWithOMP
#pragma omp parallel for
#endif
    for(long int n=0; n<numTrig_; n++){
        areaTrig_[n] = areaTrig(n);
    }
    sumAreaTrig = 0.0;
    for(long int n=0; n<numTrig_; n++){
        sumAreaTrig = sumAreaTrig + areaTrig_[n];
    }
    meanAreaTrig = sumAreaTrig/(double)numTrig_;
}

//=============================================================================
//------ stl file check   -----------------------------------------------------
//=============================================================================
bool ParserSTL::is_stl_ (void){
    ifstream fileId;
    string   fileLine;

    fileId.open (getFilename().c_str());
    if(fileId.is_open()){}else{fileId.close(); return false;}

    bool loopflag_;
    loopflag_ = false;
    numTrig_ = 0;
    numSolid_ = 0;


    while(leapBlanktrim(fileId, fileLine)){
        if(fileLine.compare(0,5,"solid")==0){
            loopflag_ = true;
        }else{
            break;
        }
        while(leapBlanktrim(fileId, fileLine)){
            if(fileLine.compare(0,8,"endsolid")==0){
                numSolid_ ++;
                break;
            }
            if(fileLine.compare(0,12,"facet normal")==0){

                if(leapBlanktrim(fileId, fileLine)){
                }else {
                    fileId.close();
                    return false;
                }
                if(fileLine.compare(0,10,"outer loop")==0){
                }else {
                    fileId.close();
                    return false;
                }

                short int nVertex = 0;
                if(leapBlanktrim(fileId, fileLine)){}else {
                    fileId.close();
                    return false;
                }
                while(fileLine.compare(0,6,"vertex")==0){
                    if(leapBlanktrim(fileId, fileLine)){}else {
                        fileId.close();
                        return false;
                    }
                    nVertex ++;
                }
                if(nVertex==3){
                    numTrig_++;
                }else{
                    fileId.close();
                    return false;
                }
                if(fileLine.compare(0,7,"endloop")==0){
                }else {
                    fileId.close();
                    return false;
                }

                if(leapBlanktrim(fileId, fileLine)){
                }else {
                    fileId.close();
                    return false;
                }
                if(fileLine.compare(0,8,"endfacet")==0){
                }else {
                    fileId.close();
                    return false;
                }
            }else{return false;}
        }
    }


fileId.close();
return loopflag_;

}

//=============================================================================
bool ParserSTL::checkfacent_ (ifstream &fileId){
    string   fileLine;


    if(fileId.is_open()){}else{return false;}

    numTrig_ = 0;


    if(leapBlanktrim(fileId, fileLine)){}else {return false;}
    if(fileLine.compare(0,5,"solid")==0){
        if(leapBlanktrim(fileId, fileLine)){}else {return false;}
        if(fileLine.compare(0,12,"facet normal")==0){
        }else {
            return false;
        }
    }else if(fileLine.compare(0,12,"facet normal")==0){

    }else {
        return false;
    }

    if(leapBlanktrim(fileId, fileLine)){}else {return false;}
    if(fileLine.compare(0,10,"outer loop")==0){}else {return false;}

    short int nVertex = 0;
    if(leapBlanktrim(fileId, fileLine)){}else {return false;}
    while(fileLine.compare(0,6,"vertex")==0){
        if(leapBlanktrim(fileId, fileLine)){}else {return false;}
        nVertex ++;
    }

    if(nVertex==3){}else{return false;}

    if(leapBlanktrim(fileId, fileLine)){}else {return false;}
    if(fileLine.compare(0,7,"endloop")==0){}else {return false;}

    if(leapBlanktrim(fileId, fileLine)){}else {return false;}
    if(fileLine.compare(0,8,"endfacet")==0){}else {return false;}


return true;

}

bool ParserSTL::is_stl_OnlyFirst_ (void){
    ifstream fileId;
    string   fileLine;
    bool retBool;
    fileId.open (getFilename().c_str());
    if(fileId.is_open()){}else{return false;}
    retBool = checkfacent_ (fileId);
    fileId.close();
    return retBool;
}

bool ParserSTL::is_stl (void){
    return is_stl_();
}

long int ParserSTL::get_numTrig_(void) {
    if(!is_stl()){
        numTrig_ = 0;
    }
    return numTrig_;
}

void ParserSTL::read_numSolid_ (void){
    ifstream fileId;
    string   fileLine;

    fileId.open (getFilename().c_str());

    numSolid_ = 0;
    bool intSolidFlag_;
    intSolidFlag_ = false;

    //if(leapBlank(fileId, fileLine)){}else {return false;}
    //if(fileLine.compare(0,5,"solid")==0){
        //---------------------------------------------------------------------
        while(leapBlanktrim(fileId, fileLine)){
            if(fileLine.compare(0,5,"solid"   ) ==0) {
                intSolidFlag_ = true;
                numSolid_ ++;
            }
            if(fileLine.compare(0,8,"endsolid") ==0) {
                if (intSolidFlag_) {
                    intSolidFlag_ = false;
                }
            }
        }
        //---------------------------------------------------------------------
    //}

}

long int ParserSTL::countFileLines_(void){

    std::ifstream file (getFilename().c_str());
    char c;
    long int i = 0;
    while (file.get(c)){
        if (c == '\n'){++i;}
    }
    return i;
}

void ParserSTL::ltrim(string& s)
{
    //string delimiters = " \f\n\r\t\v";
    //s.erase(0, s.find_first_not_of(" \f\n\r\t\v") );
    s.erase(0, s.find_first_not_of(" "));
}

bool ParserSTL::leapBlank (ifstream &fileId, string &lastLine){
    string auxLine;
    while(getline (fileId,lastLine)){
        auxLine = lastLine;
        std::remove(auxLine.begin(), auxLine.end(), ' ');
        if(lastLine!=""){return true;}
    }
    return false;
}

bool ParserSTL::leapBlanktrim (ifstream &fileId, string &lastLine){
   string auxLine;
   while(getline (fileId,lastLine)){
      auxLine = lastLine;
      std::remove(auxLine.begin(), auxLine.end(), ' ');
      if(lastLine!=""){trim(lastLine); return true;}
   }
   trim(lastLine);
   return false;
}
