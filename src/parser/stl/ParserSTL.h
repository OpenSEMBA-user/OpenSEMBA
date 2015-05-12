/*
 * ParserSTL.h
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#ifndef PARSERSTL_H_
#define PARSERSTL_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "parser/Parser.h"
#include "SmbData.h"

class ParserSTL : public Parser, public ProjectFile {
    friend class SmbData;
public:
    ParserSTL();
    ParserSTL(const string& fn);
    virtual ~ParserSTL();

    virtual SmbData* read();

    void printInfo() const;
private:
    MeshUnstructured* mesh_;
    MeshUnstructured* readMesh();
    GroupLayers<> readLayers();
    GroupCoordinates<CoordR3> readCoordinates();
    GroupElements<ElemR> readElements(const GroupCoordinates<CoordR3>&);
    void readTri3Elements (const GroupCoordinates<CoordR3>& v,
                           vector<ElemR*>& elems);
    void init(const string& pTPath);


    inline long int **get_nodeTrig (void) const {return nodeTrig_; }
    inline long int *get_node      (void) const {return node_;     }
    inline double   *get_coordx    (void) const {return coordx_;   }
    inline double   *get_coordy    (void) const {return coordy_;   }
    inline double   *get_coordz    (void) const {return coordz_;   }
    inline long int  get_dimsCoord (void) const {return dimsCoord_;}
    inline long int  get_numTrig   (void) const {return numTrig_;  }

    inline vector <string>  get_solidName (void) const {return solidName_;}
    vector <long int>       get_solidId_  (void) const {return solidId_;  }

    bool stlParser (void);
    bool is_stl (void);

private:
    vector <string> solidName_;
    vector <long int> solidId_;
    long int *node_;
    long int **nodeTrig_;
    double *coordx_;
    double *coordy_;
    double *coordz_;
    long int dimsCoord_;
    long int numTrig_, numSolid_;
    long int NumColapsNodes_;
    double *areaTrig_; //numTrig
    double sumAreaTrig, meanAreaTrig;
    bool stlCheck_;

    void deleteFull (void);
    bool is_stl_ (void);
    bool is_stl_OnlyFirst_ (void);
    void preCounter_ (const string fileName);
    void setDefaultValues_(void);
    long int countFileLines_(void);
    void ltrim(string& s);
    bool parser_ (void);
    void addVertexLine (string &VertexLine,long int &counter);
    void collaps_ (void);
    double dst (const long int &n, const long int &m) const;
    bool intoSphere (const long int &n, const long int &m,
                       const double &radius)const;
    double lTrigMin (const double areaLimit)const;
    double areaTrig (const long int &nele)const;
    void buildAreaTrig (void);

    long int get_numTrig_(void);
    void read_numSolid_  (void);
    bool checkfacent_ (ifstream &fileId);
    bool leapBlank (ifstream &fileId, string &lastLine);
    bool leapBlanktrim (ifstream &fileId, string &lastLine);
};

#endif /* PARSERGID_H_ */
