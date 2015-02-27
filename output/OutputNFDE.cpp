#include "OutputNFDE.h"

const string OutputNFDE::space = " ";

OutputNFDE::OutputNFDE(const NFDEData *nfde)
:	nfde(nfde) {

}

OutputNFDE::~OutputNFDE() {
    outputNfde.close();
    outputConf.close();
    outputCmsh.close();
}

void OutputNFDE::exportNFDE(const string &file,
        bool outputHeaders,
        bool outputEnd) {

    outputNfde.exceptions(ifstream::failbit);

    nfdeName = file + ".nfde";
    confName = file + ".conf";
    cmshName = file + ".cmsh";
    try {
        outputNfde.open(nfdeName.c_str());
        if (nfde->conformalLines.size() == 1) {
            outputConf.open(confName.c_str());
            outputCmsh.open(cmshName.c_str());
        }
    }
    catch(exception &e) {
        cerr << "ERROR @ OutputNFDE::export()" << endl;
        cerr << "File can't be opened: " << file << endl;
        exit(1);
    }

    time_t rawtime;
    tm 	*timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string date(buffer);

    outputNfde << "* " << file << endl;
    outputNfde << "* " << date << endl;
    outputNfde << endl;

    if (outputHeaders) {
        outputNfde << "************************* GENERAL **************************";
        outputNfde << endl << endl;
        exportTimeSteps();

        outputNfde << "*********************** SPACE STEPS ************************";
        outputNfde << endl << endl;
        exportSpaceSteps();

        outputNfde << "************************ BACKGROUND ************************";
        outputNfde << endl << endl;
        exportBackground();

        outputNfde << "******************* BOUNDARY CONDITIONS ********************";
        outputNfde << endl << endl;
        exportBoundaries();
    }

    if (!nfde->planeWaveSource.empty()) {

        outputNfde << "********************* EXTENDED SOURCES *********************";
        outputNfde << endl << endl;
        exportPlaneWaveSource();
    }

    if (!nfde->currentDensitySource.empty() ||
        !nfde->fieldSource.empty()) {

        outputNfde << "********************** NODAL SOURCES ***********************";
        outputNfde << endl << endl;
        exportCurrentDensitySource();
        exportFieldSource();
    }

    if (!nfde->isotropicBody.empty() ||
        !nfde->isotropicSurf.empty() ||
        !nfde->isotropicLine.empty()) {

        outputNfde << "******************* ISOTROPIC MATERIALS ********************";
        outputNfde << endl << endl;
        exportIsotropicBody();
        exportIsotropicSurf();
        exportIsotropicLine();
    }

    if (!nfde->anisotropicBody.empty() ||
        !nfde->anisotropicSurf.empty() ||
        !nfde->anisotropicLine.empty()) {

        outputNfde << "******************* ANISOTROPIC MATERIALS ******************";
        outputNfde << endl << endl;
        exportAnisotropicBody();
        exportAnisotropicSurf();
        exportAnisotropicLine();
    }


    if (!nfde->dispersiveBody.empty() ||
        !nfde->dispersiveSurf.empty() ||
        !nfde->dispersiveLine.empty()) {

        outputNfde << "************** FREQUENCY DEPENDENT MATERIALS ***************";
        outputNfde << endl << endl;
        exportDispersiveBody();
        exportDispersiveSurf();
        exportDispersiveLine();
    }

    if(!nfde->compositeSurf.empty()) {

        outputNfde << "*************** COMPOSITE SURFACE MATERIALS ****************";
        outputNfde << endl << endl;
        exportCompositeSurf();
    }

    if(!nfde->thinWire.empty()) {

        outputNfde << "************************* THIN WIRES ***********************";
        outputNfde << endl << endl;
        exportThinWire();
    }

//    if(!nfde->thinGap.empty()) {
//
//        outputNfde << "************************* THIN GAPS ************************";
//        outputNfde << endl << endl;
//        exportThinGap();
//    }

    if (!nfde->traditionalProbe.empty() ||
        !nfde->newProbe.empty() ||
        !nfde->bulkProbe.empty() ||
        !nfde->sliceProbe.empty()) {

        outputNfde << "************************** PROBES **************************";
        outputNfde << endl << endl;
        if(!nfde->traditionalProbe.empty()) {
            outputNfde << "** TRADITIONAL PROBES **" << endl;
            outputNfde << endl;
            exportTraditionalProbe();
        }
        if(!nfde->newProbe.empty()) {
            outputNfde << "** NEW PROBES **" << endl;
            outputNfde << endl;
            exportNewProbe();
        }
        if(!nfde->bulkProbe.empty()) {
            outputNfde << "** BULK CURRENT PROBES **" << endl;
            outputNfde << endl;
            exportBulkProbes();
        }
        if(!nfde->sliceProbe.empty()) {
            outputNfde << "** SLICE PROBES **" << endl;
            outputNfde << endl;
            exportSliceProbes();
        }
    }

    if(outputEnd)
        outputNfde << "!END" << endl;
    
    exportConformalLines();
}


inline string OutputNFDE::toString1PNT(const NFDEData::Coords coord) {
    stringstream res;
    res << toString(coord.coords.second) << endl;
    return res.str();
}

inline string OutputNFDE::toString1PNT(const NFDEData::CoordsLine coord) {
    stringstream res;
    res << toString(coord.coords) << endl;
    return res.str();
}

inline string OutputNFDE::toString2PNT(
        const NFDEData::CoordsDir coord,
        int skip) {
    stringstream res;
    res << toString(coord.coords.first);
    switch (coord.dir) {
    case x:
        res << "X";
        break;
    case y:
        res << "Y";
        break;
    case z:
        res << "Z";
        break;
    }
    if (skip != 0) {
        res << " " << skip;
    }
    res << endl;
    res << toString(coord.coords.second) << endl;
    return res.str();
}

inline string OutputNFDE::toString2PNT(const NFDEData::Coords coord) {
    stringstream res;
    res << toString(coord.coords.first) << endl << toString(coord.coords.second)
            << endl;
    return res.str();
}

inline string OutputNFDE::toString(
        const NFDEData::Boundary& boundary,
        const uint d,
        const uint p) {
    stringstream res;
    bool pml = false;
    double lay = 0.0, order = 0.0, ref = 0.0;
    bool sym = false;
    string sp;
    switch (boundary.bctype) {
    case NFDEData::Boundary::Types::PEC:
        res << "PEC";
        break;
    case NFDEData::Boundary::Types::PMC:
        res << "PMC";
        break;
    case NFDEData::Boundary::Types::SYMMETRIC:
        sym = true;
        switch (boundary.bctype) {
        case NFDEData::Boundary::PeriodicTypes::ETAN:
            res << "SYMMETRIC";
            sp = "ETAN";
            break;
        case NFDEData::Boundary::PeriodicTypes::HTAN:
            res << "SYMMETRIC";
            sp = "HTAN";
            break;
        default:
            res << "PEC" << endl;
            sym = false;
            break;
        }
        break;
    case NFDEData::Boundary::Types::PERIODIC:
        res << "PERIODIC";
        break;
    case NFDEData::Boundary::Types::MUR1:
        res << "MUR1";
        break;
    case NFDEData::Boundary::Types::PML:
        res << "PML";
        pml = true;
        lay = boundary.lay;
        order = boundary.order;
        ref = boundary.ref;
        break;
    default:
        res << "PEC";
        break;
    }
    if (p == 2) {
        res << " " << "ALL" << endl;
    } else {
        res << toString(CartesianBound(p)) << endl;
    }
    if (sym) {
        res << sp << endl;
    }
    if (pml) {
        res << lay << " " << order << " " << ref << endl;
    }
    if (d != 3) {
        res << space << toString(CartesianAxis(d));
    }
    return res.str();
}

string OutputNFDE::toString(const NFDEData::CoordsMultiplier coord) {
    stringstream res;
    res << toString(coord.coords.first) << toString(coord.multiplier) << endl;
    res << toString(coord.coords.second) << endl;
    return res.str();
}

string OutputNFDE::toString(CartesianBound pos) {
    switch (pos) {
    case 0:
        return "L";
    case 1:
        return "U";
    default:
        return "";
    }
}

string OutputNFDE::toString(CartesianAxis dir) {
    switch (dir) {
    case x:
        return "X";
    case y:
        return "Y";
    case z:
        return "Z";
    default:
        return "";
    }
}

string OutputNFDE::toString(const NFDEData::MaterialTypes mat) {
    switch (mat) {
    case NFDEData::MaterialTypes::METAL:
        return "!!METAL";
    case NFDEData::MaterialTypes::MGMET:
        return "!!MGMET";
    case NFDEData::MaterialTypes::NONME:
        return "!!NONME";
    default:
        cerr << "ERROR @ OutputNFDE: " << "Undefined material type." << endl;
        return "!!NONE";
    }
}

string OutputNFDE::toString(const NFDEData::CoordsNewProbe::Types type) {
    switch (type) {
    case NFDEData::CoordsNewProbe::Types::EX:
        return "EX";
    case NFDEData::CoordsNewProbe::Types::EY:
        return "EY";
    case NFDEData::CoordsNewProbe::Types::EZ:
        return "EZ";
    case NFDEData::CoordsNewProbe::Types::HX:
        return "HX";
    case NFDEData::CoordsNewProbe::Types::HY:
        return "HY";
    case NFDEData::CoordsNewProbe::Types::HZ:
        return "HZ";
    case NFDEData::CoordsNewProbe::Types::IW:
        return "IW";
    case NFDEData::CoordsNewProbe::Types::VG:
        return "VG";
    default:
        return "EX";
    }
}

void OutputNFDE::exportTimeSteps() {
    outputNfde << "!TIMESTEPS" << endl;
    outputNfde << nfde->timesteps.dt << space << nfde->timesteps.nmax << endl;
    outputNfde << endl;
}

void OutputNFDE::exportSpaceSteps() {
    outputNfde << "!NEWSPACESTEPS" << endl;
    for(int d = 0; d < 3; d++) {
        switch(d) {
        case x:
            outputNfde << "!!X";
            break;
        case y:
            outputNfde << "!!Y";
            break;
        case z:
            outputNfde << "!!Z";
            break;
        }

        if(nfde->spacesteps[d].cons)
            outputNfde << "CONS";
        else
            outputNfde << "VARI";
        outputNfde << endl;
        outputNfde << nfde->spacesteps[d].origin << endl;
        outputNfde << nfde->spacesteps[d].m << space << nfde->spacesteps[d].n << endl;
        outputNfde << nfde->spacesteps[d].d[0] << endl;
        if (!nfde->spacesteps[d].cons) {
            for (uint i = 1; i < nfde->spacesteps[d].d.size(); i++) {
                outputNfde << nfde->spacesteps[d].d[i] << endl;
            }
        }
    }
    outputNfde << endl;
}

void OutputNFDE::exportBackground() {
    outputNfde << "!BACKGROUND" << endl;
    outputNfde << "!!LINEAR" << endl;
    outputNfde << nfde->background.sigma << space
            << nfde->background.eps << space
            << nfde->background.mu << space
            << nfde->background.sigmam << endl;

    outputNfde << endl;
}
void OutputNFDE::exportBoundaries() {
    outputNfde << "!BOUNDARY CONDITION" << endl;
    if((nfde->boundaries[x][0].bctype == nfde->boundaries[x][1].bctype) ||
            (nfde->boundaries[x][0].bctype == nfde->boundaries[y][0].bctype) ||
            (nfde->boundaries[x][0].bctype == nfde->boundaries[y][1].bctype) ||
            (nfde->boundaries[x][0].bctype == nfde->boundaries[z][0].bctype) ||
            (nfde->boundaries[x][0].bctype == nfde->boundaries[z][1].bctype)) {
        outputNfde << toString(nfde->boundaries[x][0]);
    } else {
        for(int d = 0; d < 3; d++)
            for(int p = 0; p < 2; p++) {
                outputNfde << toString(nfde->boundaries[d][p], d, p);
            }
    }
    outputNfde << endl;
}

void OutputNFDE::exportPlaneWaveSource() {
    for (uint i = 0; i < nfde->planeWaveSource.size(); i++) {
        const NFDEData::PlaneWaveSource* ent = &nfde->planeWaveSource[i];
        outputNfde << "!PLANE WAVE SOURCE" << endl;
        outputNfde << ent->filename << endl;
        outputNfde << "LOCKED" << endl;
        outputNfde << toString2PNT(ent->coords);
        outputNfde << ent->theta << space << ent->phi << space
                << ent->alpha << space << ent->beta << endl;
        outputNfde << endl;
    }
}

void OutputNFDE::exportCurrentDensitySource() {
    for(uint i = 0; i < nfde->currentDensitySource.size(); i++) {
        const NFDEData::CurrentDensitySource& ent =
                nfde->currentDensitySource[i];
        outputNfde << "!CURRENT DENSITY SOURCE" << endl;
        switch(ent.type) {
        case NFDEData::CurrentDensitySource::Types::ELECT:
            outputNfde << "!!ELECT" << endl;
            break;
        default:
            outputNfde << "!!MAGNE" << endl;
        }
        outputNfde << "!!!2PNT" << endl;
        outputNfde << ent.filename << endl;
        for(uint j = 0; j < ent.entities.size(); j++)
            outputNfde << toString(ent.entities[j]);

        outputNfde << endl;
    }
}
void OutputNFDE::exportFieldSource() {
    for(uint i = 0; i < nfde->fieldSource.size(); i++) {
        const NFDEData::FieldSource& ent = nfde->fieldSource[i];
        outputNfde << "!CURRENT DENSITY SOURCE" << endl;
        switch(ent.type) {
        case NFDEData::FieldSource::Types::ELECT:
            outputNfde << "!!ELECT" << endl;
            break;
        default:
            outputNfde << "!!MAGNE" << endl;
        }
        outputNfde << "!!!2PNT" << endl;
        outputNfde << ent.filename << endl;
        for(uint j = 0; j < ent.entities.size(); j++)
            outputNfde << toString(ent.entities[j]);

        outputNfde << endl;
    }
}

void OutputNFDE::exportConformalLines() {
    if(nfde->conformalLines.size() != 1)
        return;

    outputConf << "<mesh file name>" << endl;
    outputConf << cmshName << endl;
    outputConf << "<mesh file name>" << endl;


    outputCmsh << "* Generate by ugrMesher (University of Granada)." <<endl;
    outputCmsh << "* version: " + string(APP_VERSION)
               << endl;
    outputCmsh << "************************ CONFORMAL NODES LIST *************************"
               << endl;

    outputCmsh << "!CONFORMAL NODES LIST" <<endl;
    outputCmsh << nfde->conformalLines[0].nodes.size() << endl;
    for(unsigned n=0; n < nfde->conformalLines[0].nodes.size(); n++) {
        outputCmsh << nfde->conformalLines[0].nodes[n].coords(x) << space
                   << nfde->conformalLines[0].nodes[n].coords(y) << space
                   << nfde->conformalLines[0].nodes[n].coords(z) << space;
        switch(nfde->conformalLines[0].nodes[n].dir) {
        case 0:
            outputCmsh << "0" << space;
            break;
        case 1:
            outputCmsh << "x" << space;
            break;
        case 2:
            outputCmsh << "y" << space;
            break;
        case 3:
            outputCmsh << "z" << space;
            break;
        default:
            outputCmsh << "0" << space;
        }
        outputCmsh << nfde->conformalLines[0].nodes[n].len << endl;
    }

    //--------------------------------------------------------------------------
    outputCmsh << "!object" << endl;
    outputCmsh<< nfde->conformalLines[0].lines.size() <<endl;
    for (unsigned n=0; n< nfde->conformalLines[0].lines.size(); n++){
        outputCmsh<< nfde->conformalLines[0].lines[n].node[0]+1 << space
                  << nfde->conformalLines[0].lines[n].node[1]+1 << space
                  << nfde->conformalLines[0].lines[n].norm(x)   << space
                  << nfde->conformalLines[0].lines[n].norm(y)   << space
                  << nfde->conformalLines[0].lines[n].norm(z)   << space
                  << nfde->conformalLines[0].lines[n].label
                  << endl;
    }
}

void OutputNFDE::exportIsotropicBody() {
    for(uint i = 0; i < nfde->isotropicBody.size(); i++) {
        const NFDEData::IsotropicBody* ent = &nfde->isotropicBody[i];
        if(!ent->layer.empty() || !ent->name.empty()) {
            outputNfde << "* " << ent->name << "@"<< ent->layer << endl;
        }
        outputNfde << "!ISOTROPIC BODY" << endl;
        outputNfde << toString(ent->type) << endl;
        outputNfde << "!!!1PNT" << endl;
        if(ent->type == NFDEData::MaterialTypes::NONME) {
            outputNfde << ent->sigma << space
                    << ent->eps << space
                    << ent->mu << space
                    << ent->sigmam << endl;
        }
        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString1PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportIsotropicSurf() {
    for(uint i = 0; i < nfde->isotropicSurf.size(); i++) {
        const NFDEData::IsotropicSurf* ent = &nfde->isotropicSurf[i];
        if(!ent->layer.empty() || !ent->name.empty() ) {
            outputNfde << "* " << ent->name << "@"<< ent->layer << endl;
        }
        outputNfde << "!ISOTROPIC SURFACE" << endl;
        outputNfde << toString(ent->type) << endl;
        outputNfde << "!!!2PNT" << endl;
        if(ent->type == NFDEData::MaterialTypes::NONME) {
            outputNfde << ent->sigma << space << ent->eps << space
                    << ent->mu << space << ent->sigmam << endl;
        }
        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportIsotropicLine() {
    for(uint i = 0; i < nfde->isotropicLine.size(); i++) {
        const NFDEData::IsotropicLine* ent = &nfde->isotropicLine[i];
        if(ent->entities.empty())
            continue;
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!ISOTROPIC LINE" << endl;
        outputNfde << toString(nfde->isotropicLine[i].type) << endl;
        outputNfde << "!!!2PNT" << endl;
        if(ent->type == NFDEData::MaterialTypes::NONME) {
            outputNfde << ent->sigma << space
                    << ent->eps << space
                    << ent->mu << space
                    << ent->sigmam << endl;
        }
        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }

        outputNfde << endl;
    }
}

void OutputNFDE::exportAnisotropicBody() {
    for(uint i = 0; i < nfde->anisotropicBody.size(); i++) {
        const NFDEData::AnisotropicBody* ent = &nfde->anisotropicBody[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!ANISOTROPIC BODY" << endl;
        outputNfde << "!!1PNT" << endl;

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigma[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->eps[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->mu[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigmam[d1][d2] << space;
            outputNfde << endl;
        }

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString1PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportAnisotropicSurf() {
    for(uint i = 0; i < nfde->anisotropicSurf.size(); i++) {
        const NFDEData::AnisotropicSurf* ent = &nfde->anisotropicSurf[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!ANISOTROPIC SURF" << endl;
        outputNfde << "!!2PNT" << endl;

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigma[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->eps[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->mu[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigmam[d1][d2] << space;
            outputNfde << endl;
        }

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportAnisotropicLine() {
    for(uint i = 0; i < nfde->anisotropicLine.size(); i++) {
        const NFDEData::AnisotropicLine* ent = &nfde->anisotropicLine[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!ANISOTROPIC LINE" << endl;
        outputNfde << "!!2PNT" << endl;

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigma[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->eps[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->mu[d1][d2] << space;
            outputNfde << endl;
        }

        for(int d1 = 0; d1 < 3; d1++) {
            for(int d2 = 0; d2 < 3; d2++)
                outputNfde << ent->sigmam[d1][d2] << space;
            outputNfde << endl;
        }

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}

void OutputNFDE::exportDispersiveBody() {
    for(uint i = 0; i < nfde->dispersiveBody.size(); i++) {
        const NFDEData::DispersiveBody* ent = &nfde->dispersiveBody[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!FREQUENCY DEPENDENT BODY" << endl;
        outputNfde << "!!1PNT" << endl;

        outputNfde << ent->sigma0 << space
                << ent->epsinf << space
                << ent->muinf << space
                << 0.0 << endl;

        outputNfde << ent->K << space << 0 << space << 0 << space << 0 << endl;

        for(int k = 0; k < ent->K; k++)
            outputNfde << ent->a[k] << space << ent->b[k] << endl;

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString1PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportDispersiveSurf() {
    for(uint i = 0; i < nfde->dispersiveSurf.size(); i++) {
        const NFDEData::DispersiveSurf* ent = &nfde->dispersiveSurf[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!FREQUENCY DEPENDENT SURF" << endl;
        outputNfde << "!!2PNT" << endl;

        outputNfde << ent->sigma0 << space
                << ent->epsinf << space
                << ent->muinf << space
                << 0.0 << endl;

        outputNfde << ent->K << space << 0 << space << 0 << space << 0 << endl;

        for(int k = 0; k < ent->K; k++)
            outputNfde << ent->a[k] << space << ent->b[k] << endl;

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}
void OutputNFDE::exportDispersiveLine() {
    for(uint i = 0; i < nfde->dispersiveLine.size(); i++) {
        const NFDEData::DispersiveLine* ent = &nfde->dispersiveLine[i];
        if(!ent->layer.empty())
            outputNfde << "* " << ent->layer << endl;
        outputNfde << "!FREQUENCY DEPENDENT LINE" << endl;
        outputNfde << "!!2PNT" << endl;

        outputNfde << ent->sigma0 << space
                << ent->epsinf << space
                << ent->muinf << space
                << 0.0 << endl;

        outputNfde << ent->K << space << 0 << space << 0 << space << 0 << endl;

        for(int k = 0; k < ent->K; k++)
            outputNfde << ent->a[k] << space << ent->b[k] << endl;

        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}

void OutputNFDE::exportCompositeSurf() {
    for(uint i = 0; i < nfde->compositeSurf.size(); i++) {
        const NFDEData::CompositeSurf* ent = &nfde->compositeSurf[i];
        if(!ent->layer.empty()) {
            outputNfde << "* " << ent->name << "@" << ent->layer << endl;
        }
        outputNfde << "!ISOTROPIC SURFACE" << endl;
        outputNfde << "!!COMPO" << endl;
        outputNfde << "!!!2PNT";
        if(!ent->name.empty()) {
            outputNfde << space << ent->name;
        } else {
            outputNfde << space << "undefinedName";
        }
        outputNfde << space << ent->numberoflayers << endl;
        for (int j = 0; j < ent->numberoflayers; j++) {
            outputNfde << ent->sigma[i] << space
                    << ent->eps[i] << space
                    << ent->mu[i] << space
                    << ent->sigmam[i] << space
                    << ent->thk[i] << endl;
        }
        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString2PNT(ent->entities[j]);
        }
        outputNfde << endl;
    }
}

string OutputNFDE::toString(const NFDEData::ThinWire::Extremes extreme) {
    switch(extreme) {
    case NFDEData::ThinWire::Extremes::MATERIAL:
        return "MATERIAL";
    case NFDEData::ThinWire::Extremes::SERIES:
        return "SERIES";
    case NFDEData::ThinWire::Extremes::PARALLEL:
        return "PARALLEL";
    default:
        return "MATERIAL";
    }
}

void OutputNFDE::exportThinWire() {
    for(uint i = 0; i < nfde->thinWire.size(); i++) {
        if(nfde->thinWire[i].segments.size() < 1)
            continue;

        if(!nfde->thinWire[i].layer.empty()) {
            outputNfde << "* " << nfde->thinWire[i].getNameAtLayer() << endl;
        }
        outputNfde << "!NEW THIN WIRE" << endl;
        outputNfde << nfde->thinWire[i].rad << space
               << nfde->thinWire[i].res << space
               << nfde->thinWire[i].ind << space;

        outputNfde << toString(nfde->thinWire[i].tl) << space
               << toString(nfde->thinWire[i].tr) << endl;

        if ((nfde->thinWire[i].tl == NFDEData::ThinWire::Extremes::SERIES) ||
            (nfde->thinWire[i].tl == NFDEData::ThinWire::Extremes::PARALLEL))

            outputNfde << nfde->thinWire[i].rtl << space
            << nfde->thinWire[i].itl << space
            << nfde->thinWire[i].ctl << endl;

        if ((nfde->thinWire[i].tr == NFDEData::ThinWire::Extremes::SERIES) ||
            (nfde->thinWire[i].tr == NFDEData::ThinWire::Extremes::PARALLEL))

            outputNfde << nfde->thinWire[i].rtr << space
            << nfde->thinWire[i].itr << space
            << nfde->thinWire[i].ctr << endl;

        outputNfde << nfde->thinWire[i].enl << space
                << nfde->thinWire[i].enr << endl;

        for(uint j = 0; j < nfde->thinWire[i].segments.size(); j++) {

            outputNfde << nfde->thinWire[i].segments[j].coords(x) << space
                   << nfde->thinWire[i].segments[j].coords(y) << space
                   << nfde->thinWire[i].segments[j].coords(z) << space
                   << toString(nfde->thinWire[i].segments[j].dir) << space
                   << nfde->thinWire[i].segments[j].multiplier << space
                   << nfde->thinWire[i].segments[j].node << endl;

            if(abs(nfde->thinWire[i].segments[j].multiplier) > 1e-4) {
                switch(nfde->thinWire[i].segments[j].srctype) {
                case NFDEData::CoordsWire::Types::CURR:
                    outputNfde << "CURR";
                    break;
                case NFDEData::CoordsWire::Types::VOLT:
                    outputNfde << "VOLT";
                    break;
                default:
                    outputNfde << "CURR";
                }

                outputNfde << space << nfde->thinWire[i].segments[j].srcfile << endl;
            }
        }

        outputNfde << endl;
    }
}

//void OutputNFDE::exportThinGap() {
//    for(uint i = 0; i < nfde->thinGap.size(); i++) {
//        const NFDEData::ThinGap* ent = &nfde->thinGap[i];
//        if(!ent->layer.empty())
//            outputNfde << "* " << ent->layer << endl;
//        outputNfde << "!THIN GAP" << endl;
//
//        outputNfde << ent->width << endl;
//
//        for(uint j = 0; j < ent->gaps.size(); j++) {
//            outputNfde << ent->gaps[j].coords(x) << space
//                    << ent->gaps[j].coords(y) << space
//                    << ent->gaps[j].coords(z) << space
//                    << toString(ent->gaps[j].dir) << space
//                    << ent->gaps[j].node << endl;
//        }
//        outputNfde << endl;
//    }
//}

void OutputNFDE::exportTraditionalProbe() {
    // Only supports far field electric probes.
    for (uint i = 0; i < nfde->traditionalProbe.size(); i++) {
        const NFDEData::TraditionalProbe* ent = &nfde->traditionalProbe[i];
        outputNfde << "!PROBE" << endl;
        outputNfde << "!!FF" << endl;
        outputNfde << "* " << probeName(ent->name, ent->layer, ent->log) << endl;
        outputNfde << ent->thStart << " " << ent->thEnd << " " << ent->thStep << endl;
        outputNfde << ent->phStart << " " << ent->phEnd << " " << ent->phStep << endl;
        outputNfde << ent->filename << endl;
        outputNfde << toString1PNT(ent->entities) << endl;
    }
}

void OutputNFDE::exportNewProbe() {
    for(uint i = 0; i < nfde->newProbe.size(); i++) {
        const NFDEData::NewProbe* ent = &nfde->newProbe[i];
        outputNfde << "* " << probeName(ent->name, ent->layer, ent->log) << endl;
        bool time = false;
        bool freq = false;
        bool tran = false;
        outputNfde << "!NEW PROBE" << endl;
        outputNfde << "!!NUMER" << endl;

        switch(ent->domain) {
        case NFDEData::NewProbe::DomainTypes::TIME:
            outputNfde << "!!!TIME" << endl;
            time = true;
            break;
        case NFDEData::NewProbe::DomainTypes::FREQ:
            outputNfde << "!!!FREQ" << endl;
            freq = true;
            break;
        case NFDEData::NewProbe::DomainTypes::TRAN:
            outputNfde << "!!!TRAN" << endl;
            freq = true;
            tran = true;
            break;
        case NFDEData::NewProbe::DomainTypes::TIFR:
            outputNfde << "!!!TIFR" << endl;
            time = true;
            freq = true;
            break;
        case NFDEData::NewProbe::DomainTypes::TITR:
            outputNfde << "!!!TITR" << endl;
            time = true;
            freq = true;
            break;
        case NFDEData::NewProbe::DomainTypes::FRTR:
            outputNfde << "!!!FRTR" << endl;
            freq = true;
            break;
        case NFDEData::NewProbe::DomainTypes::ALL:
            outputNfde << "!!!ALL" << endl;
            time = true;
            freq = true;
            break;
        default:
            outputNfde << "!!!TIME" << endl;
            time = true;
            break;
        }

        if(time) {
            outputNfde << ent->tstart << space
                    << ent->tstop << space
                    << ent->tstep << endl;
        }

        if(freq) {
            outputNfde << ent->fstart << space
                    << ent->fstop << space
                    << ent->fstep << endl;
        }

        if(tran) {
            outputNfde << ent->filename << endl;
        }
        for(uint j = 0; j < ent->entities.size(); j++) {
            outputNfde << toString(ent->entities[j].type);
            outputNfde << space << toString(ent->entities[j].coords) << endl;
        }
        outputNfde << endl;
    }
}

string OutputNFDE::toString(const NFDEData::BulkProbe::Types type) {
    switch (type) {
    case NFDEData::BulkProbe::Types::ELECT:
        return "ELECT";
        break;
    case NFDEData::BulkProbe::Types::MAGNE:
        return "MAGNE";
        break;
    default:
        cerr << "ERROR @ exportBulkProbe: "
             << "Undefined type." << endl;
        break;
    }
    return "ELECT";
}

void OutputNFDE::exportBulkProbes() {
    for(uint i = 0; i < nfde->bulkProbe.size(); i++) {
        const NFDEData::BulkProbe* ent = &nfde->bulkProbe[i];
        outputNfde << "* " << probeName(ent->name, ent->layer, ent->log) << endl;
        outputNfde << "!BULK CURRENT PROBE" << endl;
        outputNfde << "!!" << toString(ent->type) << endl;
        outputNfde << ent->tstart << space << ent->tstop << space <<
                ent->tstep << endl;
        outputNfde << toString2PNT(ent->entities, ent->skip) << endl;
        outputNfde << endl;
    }
}
void OutputNFDE::exportSliceProbes() {
    for(uint i = 0; i < nfde->sliceProbe.size(); i++) {
        const NFDEData::SliceProbe* ent = &nfde->sliceProbe[i];
        outputNfde << "* " << probeName(ent->name, ent->layer, ent->log) << endl;
        outputNfde << "!SLICE PROBE" << endl;
        switch (ent->field) {
        case NFDEData::SliceProbe::FieldTypes::BC:
            outputNfde << "!!BC" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::BCX:
            outputNfde << "!!BCX" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::BCY:
            outputNfde << "!!BCY" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::BCZ:
            outputNfde << "!!BCZ" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::ME:
            outputNfde << "!!ME" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::MH:
            outputNfde << "!!MH" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::EX:
            outputNfde << "!!EX" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::EY:
            outputNfde << "!!EY" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::EZ:
            outputNfde << "!!EZ" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::HX:
            outputNfde << "!!HX" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::HY:
            outputNfde << "!!HY" << endl;
            break;
        case NFDEData::SliceProbe::FieldTypes::HZ:
            outputNfde << "!!HZ" << endl;
            break;
        default:
            cerr << "ERROR @ exportSliceProbe: "
            << "Undefined type." << endl;
            break;
        }
        switch(ent->domain) {
        case NFDEData::SliceProbe::DomainTypes::TIME:
            outputNfde << "!!!TIME" << endl;
            outputNfde << ent->tstart << space
                    << ent->tstop  << space
                    << ent->tstep  << endl;
            break;
        case NFDEData::SliceProbe::DomainTypes::FREQ:
            outputNfde << "!!!FREQ" << endl;
            outputNfde << ent->fstart << space
                    << ent->fstop  << space
                    << ent->fstep  << endl;
            break;
        case NFDEData::SliceProbe::DomainTypes::TRAN:
            outputNfde << "!!!TRAN" << endl;
            outputNfde << ent->fstart << space
                    << ent->fstop  << space
                    << ent->fstep  << endl;
            outputNfde << ent->filename << endl;
            break;
        default:
            cerr << "ERROR @ exportSliceProbe: "
            << "Undefined type." << endl;
            break;
        }
        outputNfde << "CUTAWAY" << endl;
        outputNfde << toString1PNT(ent->entities) << endl;
    }
}

string
OutputNFDE::probeName(const string& name, const string& layer, const bool log) const {
    string res = name;
    if (!layer.empty()) {
        res += "@" + layer;
    }
    if (log) {
        res += "_log_";
    }
    return res;
}
