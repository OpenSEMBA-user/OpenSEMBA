/*
 * ArgumentsSmbToNFDE.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: mdebi
 */

#include "ArgumentsMesher.h"

ArgumentsMesher::ArgumentsMesher(
        const int argc,
        const char* argv[]) :
    Arguments(argc, argv) {
}

void
ArgumentsMesher::set(
        MesherOptions* params,
        const Int argc,
        const char* argv[]) const {
    for (Int i = 1; i < argc; i++) {
        string str = argv[i];
        if (!str.compare("-i")) {
            i++;
        } else if (!str.compare("-h") || !str.compare("--help")) {
            printHelp();
            abort(EXIT_SUCCESS);
        } else  if (!str.compare("--openfoam")) {
            params->setMesher(MesherOptions::openfoam);
        } else if (!str.compare("--bruteForceVolumes")) {
            params->setBruteForceVolumes(true);
        } else if (!str.compare("--Structured")          ||
                !str.compare("--structured")          ||
                !str.compare("-structured")           ||
                !str.compare("-structuredWireFrame")  ||
                !str.compare("-StructuredWireFrame")  ||
                !str.compare("--structuredWireFrame") ||
                !str.compare("--StructuredWireFrame")) {
#ifndef compileOnlyWithStructured
#ifdef compileWithstaircase
            params->setMode(MesherOptions::structured);
#endif
#endif
        } else if (!str.compare("--eff") ||
                !str.compare("-eff")) {
#ifdef effectiveParameterMethods
            effectiveParameter_ = true;
            th_    = getArgvpp(++i, *argv++);
            sigma_ = getArgvpp(++i, *argv++);
#endif
        } else if (!str.compare("--slanted") ||
                !str.compare("--Slanted") ||
                !str.compare("-Slanted")  ||
                !str.compare("-slanted")) {
#ifndef compileOnlyWithStructured
            if(params->getMode() == MesherOptions::slanted ||
                    params->getMode() == MesherOptions::relaxed) {
                ++i;
                continue;
            }
            params->setMode(MesherOptions::slanted);
            params->setEdgeFraction(getArgvpp(++i, argc, *argv++));
#endif
        } else if (!str.compare("--relaxed") ||
                !str.compare("--Relaxed") ||
                !str.compare("-relaxed")  ||
                !str.compare("-Relaxed")) {
#ifndef compileOnlyWithStructured
            if(params->getMode() == MesherOptions::slanted ||
                    params->getMode() == MesherOptions::relaxed) {
                ++i;
                continue;
            }
            params->setMode(MesherOptions::relaxed);
            params->setEdgeFraction(getArgvpp(++i, argc, *argv++));
#endif
        } else if (!str.compare("-scaleFactor")  ||
                !str.compare("-scalefactor")  ||
                !str.compare("-ScaleFactor")  ||
                !str.compare("-Scalefactor")  ||
                !str.compare("--scaleFactor") ||
                !str.compare("--ScaleFactor") ||
                !str.compare("--scalefactor") ||
                !str.compare("--Scalefactor")) {
#ifdef compileWithTesting
            scaleFactor_ = true;
            scaleFactorValue_ = getArgvpp(++i, *argv++);
#endif
        } else if (!str.compare("-swff") ||
                !str.compare("--swff") ||
                !str.compare("--swfforze")  ||
                !str.compare("-swfforze")) {
            params->setSwfForze(getArgvpp(++i, argc, *argv++));
        } else {
            cerr<< endl << "ERROR @ ArgumentsSmbToNFDE: "
                << "Input argument not reckognized: " << str << endl;
            printHelp();
            abort(EXIT_FAILURE);
        }
    }
}

ArgumentsMesher::~ArgumentsMesher() {

}

void ArgumentsMesher::printInfo() const {
    cout << " --- ArgumentsSmbToNFDE Info --- " << endl;
    Arguments::printInfo();
    cout << "-- End of Arguments info --" << endl;
}

void ArgumentsMesher::printHelp() const {
    Arguments::printHelp();
}
