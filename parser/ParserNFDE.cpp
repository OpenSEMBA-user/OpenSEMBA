/*
 *        File: ParserNFDE.cpp
 * -------------------------------------------------------------------------
 *      Author: damarro
 *  Created on: Mar 4, 2014
 * Description:
 *
 *
 */

#include "ParserNFDE.h"
#include "ParserNFDETools.h"

ParserNFDE::ParserNFDE(const string &input)
:	file(),
 	nfde(new NFDEData) {
   
	file.exceptions(ifstream::failbit);
   
	try {
		file.open(input.c_str());
	}
	catch(exception &e) {
		cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "File not found: " << input << endl;
	}
}
ParserNFDE::~ParserNFDE() {
	file.close();
}

NFDEData *ParserNFDE::parseNFDE() {
	if(!file.is_open())
		return NULL;

	string line, name;
	bool foundtimesteps  = false;
	bool foundspacesteps = false;
	bool foundboundaries = false;

	try {
		while(getline(file,line)) {
			if(isComment(line) && (line.size() > 2)) {
				if((line[0] == '*') && (line[1] == ' ')) {
					name = line.substr(2);
					if(!getline(file,line))
						break;
				}
			}
         
			transform(line.begin(), line.end(), line.begin(), ::toupper);

         if((line.find("!TIMESTEP")!=string::npos) ||
                 (line.find("!TIMESTEPS")!=string::npos)) {

            foundtimesteps = true;
            parseTimeSteps();
         }
         else if((line.find("!NEW SPACESTEPS")!=string::npos) ||
                 (line.find("!NEWSPACESTEPS")!=string::npos)) {

            foundspacesteps = true;
            parseSpaceSteps();
         }
         else if(line.find("!BACKGROUND")!=string::npos) {
            parseBackground();
         }
         else if(line.find("!BOUNDARY CONDITION")!=string::npos) {
            foundboundaries = true;
            parseBoundaries();
         }
         else if(line.find("!PLANE WAVE SOURCE")!=string::npos) {
            parsePlaneWaveSource();
         }
         else if(line.find("!CURRENT DENSITY SOURCE")!=string::npos) {
            parseCurrentDensitySource();
         }
         else if(line.find("!FIELD SOURCE")!=string::npos) {
            parseFieldSource();
         }
         else if(line.find("!ISOTROPIC BODY")!=string::npos) {
            parseIsotropicBody(name);
         }
         else if(line.find("!ISOTROPIC SURFACE")!=string::npos) {
            parseIsotropicSurf(name);
         }
         else if(line.find("!ISOTROPIC LINE")!=string::npos) {
            parseIsotropicLine(name);
         }
         else if(line.find("!ANISOTROPIC BODY")!=string::npos) {
            parseAnisotropicBody(name);
         }
         else if(line.find("!ANISOTROPIC SURFACE")!=string::npos) {
            parseAnisotropicSurf(name);
         }
         else if(line.find("!ANISOTROPIC LINE")!=string::npos) {
            parseAnisotropicLine(name);
         }
         else if(line.find("!FREQUENCY DEPENDENT BODY")!=string::npos) {
            parseDispersiveBody(name);
         }
         else if(line.find("!FREQUENCY DEPENDENT SURFACE")!=string::npos) {
            parseDispersiveSurf(name);
         }
         else if(line.find("!FREQUENCY DEPENDENT LINE")!=string::npos) {
            parseDispersiveLine(name);
         }
         else if(line.find("!NEW THIN WIRE")!=string::npos) {
            parseThinWire(name);
         }
         else if(line.find("!THIN GAP")!=string::npos) {
            parseThinGap(name);
         }
         else if(line.find("!PROBE")!=string::npos) {
            parseTraditionalProbe(name);
         }
         else if(line.find("!NEW PROBE")!=string::npos) {
            parseNewProbe(name);
         }
         else if(line.find("!BULK CURRENT PROBE")!=string::npos) {
            parseBulkProbe(name);
         }
         else if(line.find("!SLICE PROBE")!=string::npos) {
            parseSliceProbe(name);
         }
         else if(line.find("!END")!=string::npos) {
            break;
         }
         
         name = "";
		}
	}
	catch(exception &e) {
      cout << "WARNING @ parseNFDE()" << endl;
		cout << "File ended without !END" << endl;
	}


	if(!foundtimesteps) {
		cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "TIMESTEPS not found" << endl;
		return NULL;
	}

	if(!foundspacesteps) {
		cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "SPACESTEPS not found" << endl;
		return NULL;
	}

	if(!foundboundaries) {
		cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "BOUNDARY CONDITIONS not found" << endl;
		return NULL;
	}

	return nfde;
}

void ParserNFDE::parseTimeSteps() {
   string line;
   double dt;
	long int nmax;
   
   getline(file, line);
	readLine("TIMESTEPS", line, dt, nmax);

	nfde->timesteps.dt   = dt;
	nfde->timesteps.nmax = nmax;
}

void ParserNFDE::parseSpaceSteps() {
	string type, line;

	bool found[3] = {false, false, false};

	while(!(found[x] && found[y] && found[z])) {
      getline(file, line);
		readLine("SPACESTEPS", line, type);

		int dir;
		bool cons;
		if(type.find("!!XCONS") != string::npos) {
			dir = x;
			cons = true;
		}
		else if(type.find("!!YCONS") != string::npos) {
			dir = y;
			cons = true;
		}
		else if(type.find("!!ZCONS") != string::npos) {
			dir = z;
			cons = true;
		}
		else if(type.find("!!XVARI") != string::npos) {
			dir = x;
			cons = false;
		}
		else if(type.find("!!YVARI") != string::npos) {
			dir = y;
			cons = false;
		}
		else if(type.find("!!ZVARI") != string::npos) {
			dir = z;
			cons = false;
		}
		else {
			continue;
		}
		found[dir] = true;

		if(!nfde->spacesteps[dir].d.empty())
			continue;

		double origin;
		int m, n;
		double diff;

		getline(file, line);
		readLine("SPACESTEPS", line, origin);
		nfde->spacesteps[dir].origin = origin;

		getline(file, line);
		readLine("SPACESTEPS", line, m, n);

		nfde->spacesteps[dir].m = m;
		nfde->spacesteps[dir].n = n;
      nfde->spacesteps[dir].cons = cons;
		nfde->spacesteps[dir].d.resize(n-m);

		getline(file, line);
		readLine("SPACESTEPS", line, diff);
		if(cons) {
			for(long int i = m; i < n; i++)
				nfde->spacesteps[dir].d[i-m] = diff;
		}
		else {
			for(long int i = m; i < n; i++) {
				nfde->spacesteps[dir].d[i-m] = diff;

				getline(file, line);
            readLine("SPACESTEPS", line, diff);
			}
		}
	}
}
void ParserNFDE::parseBackground() {
	string type, line;
	double sigma, eps, mu, sigmam;
	getline(file, line);
   readLine("BACKGROUND", line, type);

	if(type.find("!!NONLIN") != string::npos) {
		cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "BACKGROUND: NONLIN BACKGROUND not implemented" << endl;
		throw exception();
	}

	getline(file, line);
   readLine("BACKGROUND", line, sigma, eps, mu, sigmam);

	nfde->background.sigma	= sigma;
	nfde->background.eps		= eps;
	nfde->background.mu		= mu;
	nfde->background.sigmam	= sigmam;
}
void ParserNFDE::parseBoundaries() {
	string bctype[6], bcpos[6];
	string sptype, line;
	double lay, order, ref;
   
   getline(file, line);
   readLine("BOUNDARY", line, bctype[0], bcpos[0]);

	if(bcpos[x].find("ALL") != string::npos) {
		for(long int i = 1; i < 6; i++) {
			bctype[i] = bctype[0];
			bcpos[i] = bcpos[0];
		}
	}
	else
		for(long int i = 1; i < 6; i++) {
			getline(file, line);
         readLine("BOUNDARY", line, bctype[i], bcpos[i]);
		}

	if((bctype[0].find("SYMMETRIC") != string::npos) ||
	   (bctype[1].find("SYMMETRIC") != string::npos) ||
	   (bctype[2].find("SYMMETRIC") != string::npos) ||
	   (bctype[3].find("SYMMETRIC") != string::npos) ||
	   (bctype[4].find("SYMMETRIC") != string::npos) ||
	   (bctype[5].find("SYMMETRIC") != string::npos)) {

		getline(file, line);
      readLine("BOUNDARY", line, sptype);
	}
	else if((bctype[0].find("PML") != string::npos) ||
			(bctype[1].find("PML") != string::npos) ||
			(bctype[2].find("PML") != string::npos) ||
			(bctype[3].find("PML") != string::npos) ||
			(bctype[4].find("PML") != string::npos) ||
			(bctype[5].find("PML") != string::npos)) {

		getline(file, line);
      readLine("BOUNDARY", line, lay, order, ref);
	}

	long int dir;
	long int pos;
	for(long int i = 0; i < 6; i++) {
		if(bcpos[i].find("XL") != string::npos) {
			dir = x;
			pos = 0;
		}
		else if(bcpos[i].find("XU") != string::npos) {
			dir = x;
			pos = 1;
		}
		else if(bcpos[i].find("YL") != string::npos) {
			dir = y;
			pos = 0;
		}
		else if(bcpos[i].find("YU") != string::npos) {
			dir = y;
			pos = 1;
		}
		else if(bcpos[i].find("ZL") != string::npos) {
			dir = z;
			pos = 0;
		}
		else if(bcpos[i].find("ZU") != string::npos) {
			dir = z;
			pos = 1;
		}
		else {
			dir = i%3;
			pos = i/3;
		}
		if(nfde->boundaries[dir][pos].bctype != NFDEData::Boundary::Types::NONE)
			continue;
		if(bctype[i].find("SYMMETRIC") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::SYMMETRIC;
			if(sptype.find("ETAN") != string::npos)
				nfde->boundaries[dir][pos].sptype = NFDEData::Boundary::PeriodicTypes::ETAN;
			else if(sptype.find("HTAN") != string::npos)
				nfde->boundaries[dir][pos].sptype = NFDEData::Boundary::PeriodicTypes::HTAN;
			else {
				cerr << "WARNING @ parseNFDE()" << endl;
				cerr << "BOUNDARY CONDITIONS: sptype bad defined." << endl;
				cerr << "                     Ignoring boundary condition"<< endl;
				continue;
			}
		}
		else if(bctype[i].find("PML") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::PML;
			nfde->boundaries[dir][pos].lay 	 = (long int)lay;
			nfde->boundaries[dir][pos].order	 = order;
			nfde->boundaries[dir][pos].ref	 = ref;
		}
		else if(bctype[i].find("PEC") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::PEC;
		}
		else if(bctype[i].find("PMC") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::PMC;
		}
		else if(bctype[i].find("PERIODIC") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::PERIODIC;
		}
		else if(bctype[i].find("MUR1") != string::npos) {
			nfde->boundaries[dir][pos].bctype = NFDEData::Boundary::Types::MUR1;
		}
		else {
			cerr << "WARNING @ parseNFDE()" << endl;
			cerr << "BOUNDARY CONDITIONS: bctype bad defined" << endl;
			cerr << "                     Ignoring boundary condition"<< endl;
			continue;
		}
	}
}

void ParserNFDE::parsePlaneWaveSource() {
	NFDEData::PlaneWaveSource source;
	string attribute, line;
   getline(file, source.filename);
	getline(file, line);
	getline(file, line);
   readLine("PLANE WAVE SOURCE", line, source.coords.coords.first (x),
                                       source.coords.coords.first (y),
                                       source.coords.coords.first (z));
	getline(file, line);
   readLine("PLANE WAVE SOURCE", line, source.coords.coords.second(x),
                                       source.coords.coords.second(y),
                                       source.coords.coords.second(z));

	getline(file, line);
   readLine("PLANE WAVE SOURCE", line, source.theta,
                                       source.phi,
                                       source.alpha,
                                       source.beta);

	nfde->planeWaveSource.push_back(source);
}
void ParserNFDE::parseCurrentDensitySource() {
	NFDEData::CurrentDensitySource source;
	string type1, type2, line;
	bool pnt = false;
   getline(file, line);
   readLine("CURRENT DENSITY SOURCE", line, type1);
	if(type1.find("!!ELECT") != string::npos)
		source.type = NFDEData::CurrentDensitySource::Types::ELECT;
	else if(type1.find("!!MAGNE") != string::npos)
		source.type = NFDEData::CurrentDensitySource::Types::MAGNE;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "CURRENT DENSITY SOURCE: Incorrect type" << endl;
		cerr << "                        Ignoring source"<< endl;;
		return;
	}
	getline(file, line);
   readLine("CURRENT DENSITY SOURCE", line, type2);
   while(true) {
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "CURRENT DENSITY SOURCE: Bad points type" << endl;
         cerr << "                        Ignoring source"<< endl;
         return;
      }

      getline(file, source.filename);
      getline(file, line);
      NFDEData::CoordsMultiplier entity;
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("CURRENT DENSITY SOURCE", line, entity.coords.first(x)
                                                , entity.coords.first(y)
                                                , entity.coords.first(z)
                                                , entity.multiplier(x)
                                                , entity.multiplier(y)
                                                , entity.multiplier(z));
         if(pnt) {
            getline(file, line);
            readLine("CURRENT DENSITY SOURCE", line, entity.coords.second(x)
                                                   , entity.coords.second(y)
                                                   , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }
         
         source.entities.push_back(entity);
      }
      
      nfde->currentDensitySource.push_back(source);
      source.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("CURRENT DENSITY SOURCE", line, type2);
   }
}
void ParserNFDE::parseFieldSource() {
	NFDEData::FieldSource source;
	string type1, type2, line;
	bool pnt = false;
   
   getline(file, line);
   readLine("FIELD SOURCE", line, type1);
	if(type1.find("!!ELECT") != string::npos)
		source.type = NFDEData::FieldSource::Types::ELECT;
	else if(type1.find("!!MAGNE") != string::npos)
		source.type = NFDEData::FieldSource::Types::MAGNE;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "FIELD SOURCE: Incorrect type" << endl;
		cerr << "              Ignoring source"<< endl;
		return;
	}
	getline(file, line);
   readLine("FIELD SOURCE", line, type2);
   while(true) {
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "FIELD SOURCE: Bad points type" << endl;
         cerr << "              Ignoring source"<< endl;
         return;
      }

      getline(file, source.filename);
      getline(file, line);
      NFDEData::CoordsMultiplier entity;
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("FIELD SOURCE", line, entity.coords.first(x)
                                      , entity.coords.first(y)
                                      , entity.coords.first(z)
                                      , entity.multiplier(x)
                                      , entity.multiplier(y)
                                      , entity.multiplier(z));
         if(pnt) {
            getline(file, line);
            readLine("FIELD SOURCE", line, entity.coords.second(x)
                                         , entity.coords.second(y)
                                         , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         source.entities.push_back(entity);
      }
      
      nfde->fieldSource.push_back(source);
      source.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("FIELD SOURCE", line, type2);
   }
}

void ParserNFDE::parseIsotropicBody(const string &layer) {
	NFDEData::IsotropicBody body;
   body.layer = layer;
	NFDEData::Coords entity;
	string type1, type2, line;
	bool pnt = false;
   
   getline(file, line);
   readLine("ISOTROPIC BODY", line, type1);
	if(type1.find("!!METAL") != string::npos)
		body.type = NFDEData::MaterialTypes::METAL;
	else if(type1.find("!!MGMET") != string::npos)
		body.type = NFDEData::MaterialTypes::MGMET;
	else if(type1.find("!!NONME") != string::npos)
		body.type = NFDEData::MaterialTypes::NONME;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "ISOTROPIC BODY: Bad media type" << endl;
		cerr << "                Ignoring material." << endl;
		return;
	}

	getline(file, line);
   readLine("ISOTROPIC BODY", line, type2);
   while(true) {
      if(body.type == NFDEData::MaterialTypes::NONME) {
         getline(file, line);
         readLine("ISOTROPIC BODY", line, body.sigma,
                                          body.eps,
                                          body.mu,
                                          body.sigmam);
      }
      
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ISOTROPIC BODY: Bad points type" << endl;
         cerr << "                Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ISOTROPIC BODY", line, entity.coords.first(x)
                                        , entity.coords.first(y)
                                        , entity.coords.first(z));
         if(pnt) {
            getline(file, line);
            readLine("ISOTROPIC BODY", line, entity.coords.second(x)
                                           , entity.coords.second(y)
                                           , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         body.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->isotropicBody.push_back(body);
      body.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ISOTROPIC BODY", line, type2);
   }
}
void ParserNFDE::parseIsotropicSurf(const string &layer) {
   NFDEData::IsotropicSurf surf;
   surf.layer = layer;
	NFDEData::CoordsDir entity;
	string type1, type2, line, dir;
	bool pnt = false;
   
   getline(file, line);
   readLine("ISOTROPIC SURF", line, type1);
	if(type1.find("!!METAL") != string::npos)
		surf.type = NFDEData::MaterialTypes::METAL;
	else if(type1.find("!!MGMET") != string::npos)
		surf.type = NFDEData::MaterialTypes::MGMET;
	else if(type1.find("!!NONME") != string::npos)
		surf.type = NFDEData::MaterialTypes::NONME;
   else if(type1.find("!!COMPO") != string::npos)
      return parseCompositeSurf(layer);
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "ISOTROPIC SURF: Bad media type" << endl;
		cerr << "                Ignoring material." << endl;
		return;
	}

	getline(file, line);
   readLine("ISOTROPIC SURF", line, type2);
	while(true) {
      if(surf.type == NFDEData::MaterialTypes::NONME) {
         getline(file, line);
         readLine("ISOTROPIC SURF", line, surf.sigma,
                                          surf.eps,
                                          surf.mu,
                                          surf.sigmam);
      }
      
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ISOTROPIC SURF: Bad points type" << endl;
         cerr << "                Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ISOTROPIC SURF", line, entity.coords.first(x)
                                        , entity.coords.first(y)
                                        , entity.coords.first(z)
                                        , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "ISOTROPIC SURF: Bad norm direction" << endl;
            cerr << "                Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("ISOTROPIC SURF", line, entity.coords.second(x)
                                           , entity.coords.second(y)
                                           , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         surf.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->isotropicSurf.push_back(surf);
      surf.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ISOTROPIC SURF", line, type2);
   }
}
void ParserNFDE::parseIsotropicLine(const string &layer) {
	NFDEData::IsotropicLine lin;
   lin.layer = layer;
	NFDEData::CoordsDir entity;
	string type1, type2, line, dir;
	bool pnt = false;
   getline(file, line);
   readLine("ISOTROPIC LINE", line, type1);
	if(type1.find("!!METAL") != string::npos)
		lin.type = NFDEData::MaterialTypes::METAL;
	else if(type1.find("!!MGMET") != string::npos)
		lin.type = NFDEData::MaterialTypes::MGMET;
	else if(type1.find("!!NONME") != string::npos)
		lin.type = NFDEData::MaterialTypes::NONME;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "ISOTROPIC LINE: Bad media type" << endl;
		cerr << "                Ignoring material." << endl;
		return;
	}
   
	getline(file, line);
   readLine("ISOTROPIC LINE", line, type2);
	while(true) {
      if(lin.type == NFDEData::MaterialTypes::NONME) {
         getline(file, line);
         readLine("ISOTROPIC LINE", line, lin.sigma,
                                          lin.eps,
                                          lin.mu,
                                          lin.sigmam);
      }
      
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ISOTROPIC LINE: Bad points type" << endl;
         cerr << "                Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ISOTROPIC LINE", line, entity.coords.first(x)
                                        , entity.coords.first(y)
                                        , entity.coords.first(z)
                                        , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "ISOTROPIC LINE: Bad norm direction" << endl;
            cerr << "                Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("ISOTROPIC LINE", line, entity.coords.second(x)
                                           , entity.coords.second(y)
                                           , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         lin.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->isotropicLine.push_back(lin);
      lin.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ISOTROPIC LINE", line, type2);
   }
}

void ParserNFDE::parseAnisotropicBody(const string &layer) {
	NFDEData::AnisotropicBody body;
   body.layer = layer;
	NFDEData::Coords entity;
	string type, line;
	bool pnt = false;
   
	getline(file, line);
   readLine("ANISOTROPIC BODY", line, type);
	while(true) {
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC BODY", line, body.sigma[i][x]
                                          , body.sigma[i][y]
                                          , body.sigma[i][z]);
      }

      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC BODY", line, body.eps[i][x]
                                          , body.eps[i][y]
                                          , body.eps[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC BODY", line, body.mu[i][x]
                                          , body.mu[i][y]
                                          , body.mu[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC BODY", line, body.sigmam[i][x]
                                          , body.sigmam[i][y]
                                          , body.sigmam[i][z]);
      }
      
      if(type.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ANISOTROPIC BODY: Bad points type" << endl;
         cerr << "                  Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ANISOTROPIC BODY", line, entity.coords.first(x)
                                          , entity.coords.first(y)
                                          , entity.coords.first(z));
         
         if(pnt) {
            getline(file, line);
            readLine("ANISOTROPIC BODY", line, entity.coords.second(x)
                                             , entity.coords.second(y)
                                             , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         body.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->anisotropicBody.push_back(body);
      body.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ANISOTROPIC BODY", line, type);
   }
}
void ParserNFDE::parseAnisotropicSurf(const string &layer) {
	NFDEData::AnisotropicSurf surf;
   surf.layer = layer;
	NFDEData::CoordsDir entity;
	string type, line, dir;
	bool pnt = false;
   
	getline(file, line);
   readLine("ANISOTROPIC SURF", line, type);
	while(true) {
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC SURF", line, surf.sigma[i][x]
                                          , surf.sigma[i][y]
                                          , surf.sigma[i][z]);
      }

      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC SURF", line, surf.eps[i][x]
                                          , surf.eps[i][y]
                                          , surf.eps[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC SURF", line, surf.mu[i][x]
                                          , surf.mu[i][y]
                                          , surf.mu[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC SURF", line, surf.sigmam[i][x]
                                          , surf.sigmam[i][y]
                                          , surf.sigmam[i][z]);
      }
      
      if(type.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ANISOTROPIC SURF: Bad points type" << endl;
         cerr << "                  Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ANISOTROPIC SURF", line, entity.coords.first(x)
                                          , entity.coords.first(y)
                                          , entity.coords.first(z)
                                          , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "ANISOTROPIC SURF: Bad norm direction" << endl;
            cerr << "                  Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("ANISOTROPIC SURF", line, entity.coords.second(x)
                                             , entity.coords.second(y)
                                             , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         surf.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->anisotropicSurf.push_back(surf);
      surf.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ANISOTROPIC SURF", line, type);
   }
}
void ParserNFDE::parseAnisotropicLine(const string &layer) {
	NFDEData::AnisotropicLine lin;
   lin.layer = layer;
	NFDEData::CoordsDir entity;
	string type, line, dir;
	bool pnt = false;
   
	getline(file, line);
   readLine("ANISOTROPIC LINE", line, type);
	while(true) {
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC LINE", line, lin.sigma[i][x]
                                          , lin.sigma[i][y]
                                          , lin.sigma[i][z]);
      }

      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC LINE", line, lin.eps[i][x]
                                          , lin.eps[i][y]
                                          , lin.eps[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC LINE", line, lin.mu[i][x]
                                          , lin.mu[i][y]
                                          , lin.mu[i][z]);
      }
      
      for(long int i = 0; i < 3; i++) {
         getline(file, line);
         readLine("ANISOTROPIC LINE", line, lin.sigmam[i][x]
                                          , lin.sigmam[i][y]
                                          , lin.sigmam[i][z]);
      }
      
      if(type.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "ANISOTROPIC LINE: Bad points type" << endl;
         cerr << "                  Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("ANISOTROPIC LINE", line, entity.coords.first(x)
                                          , entity.coords.first(y)
                                          , entity.coords.first(z)
                                          , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "ANISOTROPIC LINE: Bad norm direction" << endl;
            cerr << "                  Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("ANISOTROPIC LINE", line, entity.coords.second(x)
                                             , entity.coords.second(y)
                                             , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         lin.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->anisotropicLine.push_back(lin);
      lin.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("ANISOTROPIC LINE", line, type);
   }
}

void ParserNFDE::parseDispersiveBody(const string &layer) {
	NFDEData::DispersiveBody body;
   body.layer = layer;
	NFDEData::Coords entity;
	string type, line;
	bool pnt = false;

	getline(file, line);
   readLine("DISPERSIVE BODY", line, type);
	while(true) {
      getline(file, line);
      readLine("DISPERSIVE BODY", line, body.sigma0,
                                        body.epsinf,
                                        body.muinf);
      
      getline(file, line);
      readLine("DISPERSIVE BODY", line, body.K);

      if(body.K <= 0) {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE BODY: K less than 1" << endl;
         body.K = 0;
      }
      else {
         body.a.resize(body.K);
         body.b.resize(body.K);
         
         for(int i = 0; i < body.K; i++) {
            getline(file, line);
            readLine("DISPERSIVE BODY", line, body.a[i], body.b[i]);
         }
      }
      
      if(type.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE BODY: Bad points type" << endl;
         cerr << "                 Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("DISPERSIVE BODY", line, entity.coords.first(x)
                                         , entity.coords.first(y)
                                         , entity.coords.first(z));
         
         if(pnt) {
            getline(file, line);
            readLine("DISPERSIVE BODY", line, entity.coords.second(x)
                                            , entity.coords.second(y)
                                            , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         body.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->dispersiveBody.push_back(body);
      body.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("DISPERSIVE BODY", line, type);
   }
}
void ParserNFDE::parseDispersiveSurf(const string &layer) {
	NFDEData::DispersiveSurf surf;
   surf.layer = layer;
	NFDEData::CoordsDir entity;
	string type1, type2, line, dir;
	bool pnt = false;

	getline(file, line);
   readLine("DISPERSIVE SURF", line, type2);
	while(true) {
      getline(file, line);
      readLine("DISPERSIVE SURF", line, surf.sigma0,
                                        surf.epsinf,
                                        surf.muinf);
      
      getline(file, line);
      readLine("DISPERSIVE SURF", line, surf.K);

      if(surf.K <= 0) {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE SURF: K less than 1" << endl;
         surf.K = 0;
      }
      else {
         surf.a.resize(surf.K);
         surf.b.resize(surf.K);
         
         for(int i = 0; i < surf.K; i++) {
            getline(file, line);
            readLine("DISPERSIVE SURF", line, surf.a[i], surf.b[i]);
         }
      }
      
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE SURF: Bad points type" << endl;
         cerr << "                 Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("DISPERSIVE SURF", line, entity.coords.first(x)
                                         , entity.coords.first(y)
                                         , entity.coords.first(z)
                                         , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "DISPERSIVE SURF: Bad norm direction" << endl;
            cerr << "                 Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("DISPERSIVE SURF", line, entity.coords.second(x)
                                           , entity.coords.second(y)
                                           , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         surf.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->dispersiveSurf.push_back(surf);
      surf.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("DISPERSIVE SURF", line, type2);
   }
}
void ParserNFDE::parseDispersiveLine(const string &layer) {
	NFDEData::DispersiveLine lin;
   lin.layer = layer;
	NFDEData::CoordsDir entity;
	string type1, type2, line, dir;
	bool pnt = false;

	getline(file, line);
   readLine("DISPERSIVE LINE", line, type2);
	while(true) {
      getline(file, line);
      readLine("DISPERSIVE LINE", line, lin.sigma0,
                                        lin.epsinf,
                                        lin.muinf);
      
      getline(file, line);
      readLine("DISPERSIVE LINE", line, lin.K);

      if(lin.K <= 0) {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE LINE: K less than 1, ignoring material" << endl;
         continue;
      }
      else {
         lin.a.resize(lin.K);
         lin.b.resize(lin.K);
         
         for(int i = 0; i < lin.K; i++) {
            getline(file, line);
            readLine("DISPERSIVE LINE", line, lin.a[i], lin.b[i]);
         }
      }
      
      if(type2.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type2.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "DISPERSIVE LINE: Bad points type" << endl;
         cerr << "                 Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("DISPERSIVE LINE", line, entity.coords.first(x)
                                         , entity.coords.first(y)
                                         , entity.coords.first(z)
                                         , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "DISPERSIVE LINE: Bad norm direction" << endl;
            cerr << "                 Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("DISPERSIVE LINE", line, entity.coords.second(x)
                                            , entity.coords.second(y)
                                            , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         lin.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->dispersiveLine.push_back(lin);
      lin.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
      
      readLine("DISPERSIVE LINE", line, type2);
   }
}

void ParserNFDE::parseCompositeSurf(const string &layer) {
   NFDEData::CompositeSurf surf;
   surf.layer = layer;
	NFDEData::CoordsDir entity;
	string type, line, dir, pnt_str;
	bool pnt = false;
   
	getline(file, pnt_str);
	while(true) {
      surf.numberoflayers = 1;
      if(pnt_str.size() > 9)
         readLine("COMPOSITE SURF", pnt_str, type, 
                                             surf.name,
                                             surf.numberoflayers);
      else
         readLine("COMPOSITE SURF", pnt_str, type);
      
      surf.sigma.resize(surf.numberoflayers);
      surf.eps.resize(surf.numberoflayers);
      surf.mu.resize(surf.numberoflayers);
      surf.sigmam.resize(surf.numberoflayers);
      surf.thk.resize(surf.numberoflayers);
      
      for(int i = 0; i < surf.numberoflayers; i++) {
         getline(file, line);
         readLine("COMPOSITE SURF", line, surf.sigma[i],
                                          surf.eps[i],
                                          surf.mu[i],
                                          surf.sigmam[i],
                                          surf.thk[i]);
         }
      
      if(type.find("!!!1PNT") != string::npos) {
         pnt = false;
      }
      else if(type.find("!!!2PNT") != string::npos)
         pnt = true;
      else {
         cerr << "WARNING @ parseNFDE()" << endl;
         cerr << "COMPOSITE SURF: Bad points type" << endl;
         cerr << "                Ignoring material"<< endl;
         return;
      }
      
      getline(file, line);
      while(!isHeader(line) && !isEmpty(line) && !isComment(line)) {
         readLine("COMPOSITE SURF", line, entity.coords.first(x)
                                        , entity.coords.first(y)
                                        , entity.coords.first(z)
                                        , dir);
         
         switch(dir[0]) {
         case 'X':
            entity.dir = x;
            break;
         case 'Y':
            entity.dir = y;
            break;
         case 'Z':
            entity.dir = z;
            break;
         default:
            cerr << "WARNING @ parseNFDE()" << endl;
            cerr << "COMPOSITE SURF: Bad norm direction" << endl;
            cerr << "                Ignoring material." << endl;
            continue;
         }
         
         if(pnt) {
            getline(file, line);
            readLine("COMPOSITE SURF", line, entity.coords.second(x)
                                           , entity.coords.second(y)
                                           , entity.coords.second(z));
         } else {
            entity.coords.second = entity.coords.first;
         }

         surf.entities.push_back(entity);
         
         getline(file, line);
      }
      
      nfde->compositeSurf.push_back(surf);
      surf.entities.clear();
      
      if(isEmpty(line) || isComment(line))
         break;
   }
}

void ParserNFDE::parseThinWire(const string &layer) {
   NFDEData::ThinWire wire;
   wire.layer = layer;
	string tl, tr, line;
   getline(file, line);
	readLine("THIN WIRE", line, wire.rad, wire.res, wire.ind, tl, tr);
	if(tl.find("MATERIAL") != string::npos)
		wire.tl = NFDEData::ThinWire::Extremes::MATERIAL;
	else if(tl.find("PARALLEL") != string::npos)
		wire.tl = NFDEData::ThinWire::Extremes::PARALLEL;
	else if(tl.find("SERIES") != string::npos)
		wire.tl = NFDEData::ThinWire::Extremes::SERIES;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "THIN WIRE: Incorrect tl" << endl;
		cerr << "           Ignoring wire"<< endl;
		return;
	}
	if(tr.find("MATERIAL") != string::npos)
		wire.tr = NFDEData::ThinWire::Extremes::MATERIAL;
	else if(tr.find("PARALLEL") != string::npos)
		wire.tr = NFDEData::ThinWire::Extremes::PARALLEL;
	else if(tr.find("SERIES") != string::npos)
		wire.tr = NFDEData::ThinWire::Extremes::SERIES;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "THIN WIRE: Incorrect tr" << endl;
		cerr << "           Ignoring wire"<< endl;
		return;
	}
	if((wire.tl == NFDEData::ThinWire::Extremes::PARALLEL) ||
	   (wire.tl == NFDEData::ThinWire::Extremes::SERIES)) {
      
		getline(file, line);
      readLine("THIN WIRE", line, wire.rtl, wire.itl, wire.ctl);
	}
	if((wire.tr == NFDEData::ThinWire::Extremes::PARALLEL) ||
	   (wire.tr == NFDEData::ThinWire::Extremes::SERIES)) {

		getline(file, line);
      readLine("THIN WIRE", line, wire.rtr, wire.itr, wire.ctr);
	}
	getline(file, line);
	readLine("THIN WIRE", line, wire.enl, wire.enr);

	NFDEData::ThinWire::Coords segment;
	string d, type;
	while(getline(file, line)) {
      if(isEmpty(line) || isComment(line) || isHeader(line))
         break;
      
      readLine("THIN WIRE", line, segment.coords(x)
                                , segment.coords(y)
	                             , segment.coords(z)
	                             , d, segment.multiplier, segment.node);
      
		if(d[0] == 'X')
			segment.dir = x;
		else if(d[0] == 'Y')
			segment.dir = y;
		else if(d[0] == 'Z')
			segment.dir = z;
		else {
			cerr << "WARNING @ parseNFDE()" << endl;
			cerr << "THIN WIRE: Incorrect dir" << endl;
			cerr << "           Ignoring wire"<< endl;
			return;
		}

		segment.srctype = NFDEData::ThinWire::Coords::Types::NONE;
		if(segment.multiplier != 0.0) {
			getline(file, line);
         readLine("THIN WIRE", line, type, segment.srcfile, false);
			if(type.find("CURR") != string::npos)
				segment.srctype = NFDEData::ThinWire::Coords::Types::CURR;
			else if(type.find("VOLT") != string::npos)
				segment.srctype = NFDEData::ThinWire::Coords::Types::VOLT;
			else {
				cerr << "WARNING @ parseNFDE()" << endl;
				cerr << "THIN WIRE: Incorrect srctype" << endl;
				cerr << "           Setting NONE by default" << endl;
				segment.srctype = NFDEData::ThinWire::Coords::Types::NONE;
			}
		}
		wire.segments.push_back(segment);
	}
	nfde->thinWire.push_back(wire);
}
void ParserNFDE::parseThinGap(const string &layer) {
	NFDEData::ThinGap gap;
   gap.layer = layer;
	string d, line;
   
   getline(file, line);
   readLine("THIN GAP", line, gap.width);
	NFDEData::ThinGap::Coords entity;
	while(getline(file, line)) {
      if(isEmpty(line) || isComment(line) || isHeader(line))
         break;
      
      readLine("THIN GAP", line, entity.coords(x)
	                            , entity.coords(y)
	                            , entity.coords(z)
	                            , d, entity.node);
		if(d[0] == 'X')
			entity.dir = x;
		else if(d[0] == 'Y')
			entity.dir = y;
		else if(d[0] == 'Z')
			entity.dir = z;
		else {
			cerr << "WARNING @ parseNFDE()" << endl;
			cerr << "THIN GAP: Incorrect dir" << endl;
			cerr << "          Ignoring gap"<< endl;
			return;
		}
      
		gap.gaps.push_back(entity);
	}
	nfde->thinGap.push_back(gap);
}

void ParserNFDE::parseTraditionalProbe(const string &layer) {
	NFDEData::TraditionalProbe probe;
	string str_type, line;
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, str_type);
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.layer);
   
   if(isComment(probe.layer) && (probe.layer.size() > 2)) {
      if((probe.layer[0] == '*') && (probe.layer[1] == ' '))
         probe.layer = probe.layer.substr(2);
   }
   if(probe.layer.size() > 5) {
      string log = probe.layer.substr(probe.layer.size()-5);
      transform(log.begin(), log.end(), log.begin(), ::tolower);
      if(log == "_log_") {
         probe.log = true;
         probe.layer.erase(probe.layer.size()-5);
      }
   }
   
	getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.fstart,
                                       probe.fstop,
                                       probe.fstep);
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.thStart,
                                       probe.thEnd,
                                       probe.thStep);
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.phStart,
                                       probe.phEnd,
                                       probe.phStep);
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.filename);
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.region.coords.first (x),
                                       probe.region.coords.first (y),
                                       probe.region.coords.first (z));
   getline(file, line);
	readLine("TRADITIONAL PROBE", line, probe.region.coords.second(x),
                                       probe.region.coords.second(y),
                                       probe.region.coords.second(z));

	nfde->traditionalProbe.push_back(probe);
}
void ParserNFDE::parseNewProbe(const string &layer) {
	NFDEData::NewProbe probe;
	probe.layer = layer;
   if(isComment(probe.layer) && (probe.layer.size() > 2)) {
      if((probe.layer[0] == '*') && (probe.layer[1] == ' '))
         probe.layer = probe.layer.substr(2);
   }
   if(probe.layer.size() > 5) {
      string log = probe.layer.substr(probe.layer.size()-5);
      transform(log.begin(), log.end(), log.begin(), ::tolower);
      if(log == "_log_") {
         probe.log = true;
         probe.layer.erase(probe.layer.size()-5);
      }
   }
   
	string type1, domain, line;
   getline(file, line);
	readLine("NEW PROBE", line, type1);

	getline(file, line);
	readLine("NEW PROBE", line, domain);
	if(domain.find("!!!TIME") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::TIME;
	else if(domain.find("!!!FREQ") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::FREQ;
	else if(domain.find("!!!TRAN") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::TRAN;
	else if(domain.find("!!!TIFR") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::TIFR;
	else if(domain.find("!!!TITR") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::TITR;
	else if(domain.find("!!!FRTR") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::FRTR;
	else if(domain.find("!!!ALL") != string::npos)
		probe.domain = NFDEData::NewProbe::DomainTypes::ALL;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "NEW PROBE: Incorrect type2." << endl;
		cerr << "           Ignoring probe"<< endl;
		return;
	}

	if((probe.domain == NFDEData::NewProbe::DomainTypes::TIME) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TIFR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TITR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::ALL)) {

		getline(file, line);
      readLine("NEW PROBE", line, probe.tstart, probe.tstop, probe.tstep);
   }
	if((probe.domain == NFDEData::NewProbe::DomainTypes::FREQ) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TRAN) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TIFR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TITR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::FRTR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::ALL)) {

		getline(file, line);
      readLine("NEW PROBE", line, probe.fstart, probe.fstop, probe.fstep);
   }
	if((probe.domain == NFDEData::NewProbe::DomainTypes::TRAN) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::FRTR) ||
	   (probe.domain == NFDEData::NewProbe::DomainTypes::TITR)) {
		
      getline(file, probe.filename);
   }

	NFDEData::NewProbe::Coords coords;
	string type;
   while(getline(file, line)) {
      if(isEmpty(line) || isComment(line) || isHeader(line))
         break;
      
      readLine("NEW PROBE", line, type, coords.coords(x)
	                                   , coords.coords(y)
	                                   , coords.coords(z));

		if(type.find("EX") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::EX;
		else if(type.find("EY") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::EY;
		else if(type.find("EZ") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::EZ;
		else if(type.find("HX") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::HX;
		else if(type.find("HY") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::HY;
		else if(type.find("HZ") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::HZ;
		else if(type.find("IW") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::IW;
		else if(type.find("VG") != string::npos)
			coords.type = NFDEData::NewProbe::Coords::Types::VG;
		else {
			cerr << "WARNING @ parseNFDE()" << endl;
			cerr << "NEW PROBE: Incorrect type" << endl;
			cerr << "           Ignoring probe"<< endl;
			continue;
		}
      
		probe.probes.push_back(coords);
	}
   
	nfde->newProbe.push_back(probe);
}
void ParserNFDE::parseBulkProbe(const string &layer) {
   NFDEData::BulkProbe probe;
   probe.layer = layer;
   if(isComment(probe.layer) && (probe.layer.size() > 2)) {
      if((probe.layer[0] == '*') && (probe.layer[1] == ' '))
         probe.layer = probe.layer.substr(2);
   }
   if(probe.layer.size() > 5) {
      string log = probe.layer.substr(probe.layer.size()-5);
      transform(log.begin(), log.end(), log.begin(), ::tolower);
      if(log == "_log_") {
         probe.log = true;
         probe.layer.erase(probe.layer.size()-5);
      }
   }
   
   string type, nml, line;
   
   getline(file, line);
	readLine("BULK PROBE", line, type);
   
   if(type.find("ELECT") != string::npos)
		probe.type = NFDEData::BulkProbe::Types::ELECT;
	else if(type.find("MAGNE") != string::npos)
		probe.type = NFDEData::BulkProbe::Types::MAGNE;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "BULK PROBE: Incorrect type." << endl;
		cerr << "            Ignoring probe." << endl;
		return;
	}
   
   getline(file, line);
	readLine("BULK PROBE", line, probe.tstart, probe.tstop, probe.tstep);
   
   getline(file, line);
	readLine("BULK PROBE", line, probe.coord.coords.first (x),
                                probe.coord.coords.first (y),
                                probe.coord.coords.first (z), nml);
   getline(file, line);
	readLine("BULK PROBE", line, probe.coord.coords.second(x),
                                probe.coord.coords.second(y),
                                probe.coord.coords.second(z));
   switch(nml[0]) {
	case 'X':
		probe.coord.dir = x;
		break;
	case 'Y':
		probe.coord.dir = y;
		break;
	case 'Z':
		probe.coord.dir = z;
		break;
	default:
   	cerr << "ERROR @ parseNFDE()" << endl;
		cerr << "BULK PROBE: Incorrect plane" << endl;
      cerr << "            Ignoring probe" << endl;
      return;
	}
   
   nfde->bulkProbe.push_back(probe);
}
void ParserNFDE::parseSliceProbe(const string &layer) {
   NFDEData::SliceProbe probe;
   probe.layer = layer;
   if(isComment(probe.layer) && (probe.layer.size() > 2)) {
      if((probe.layer[0] == '*') && (probe.layer[1] == ' '))
         probe.layer = probe.layer.substr(2);
   }
   if(probe.layer.size() > 5) {
      string log = probe.layer.substr(probe.layer.size()-5);
      transform(log.begin(), log.end(), log.begin(), ::tolower);
      if(log == "_log_") {
         probe.log = true;
         probe.layer.erase(probe.layer.size()-5);
      }
   }
   
   string field, domain, line;
   
   getline(file, line);
	readLine("SLICE PROBE", line, field);
   if(field.find("!!BC") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::BC;
	else if(field.find("!!BCX") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::BCX;
	else if(field.find("!!BCY") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::BCY;
	else if(field.find("!!BCZ") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::BCZ;
	else if(field.find("!!ME") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::ME;
	else if(field.find("!!MH") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::MH;
	else if(field.find("!!EX") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::EX;
	else if(field.find("!!EY") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::EY;
	else if(field.find("!!EZ") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::EZ;
	else if(field.find("!!HX") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::HX;
	else if(field.find("!!HY") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::HY;
	else if(field.find("!!HZ") != string::npos)
		probe.field = NFDEData::SliceProbe::FieldTypes::HZ;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "SLICE PROBE: Incorrect field." << endl;
		cerr << "             Ignoring probe." << endl;
		return;
	}
   getline(file, line);
	readLine("SLICE PROBE", line, domain);
   if(domain.find("!!!TIME") != string::npos)
		probe.domain = NFDEData::SliceProbe::DomainTypes::TIME;
	else if(domain.find("!!!FREQ") != string::npos)
		probe.domain = NFDEData::SliceProbe::DomainTypes::FREQ;
	else if(domain.find("!!!TRAN") != string::npos)
		probe.domain = NFDEData::SliceProbe::DomainTypes::TRAN;
	else {
		cerr << "WARNING @ parseNFDE()" << endl;
		cerr << "SLICE PROBE: Incorrect domain." << endl;
		cerr << "             Ignoring probe." << endl;
		return;
	}
   
   
   if(probe.domain == NFDEData::SliceProbe::DomainTypes::TIME) {
		getline(file, line);
      readLine("SLICE PROBE", line, probe.tstart, probe.tstop, probe.tstep);
   }
	if((probe.domain == NFDEData::SliceProbe::DomainTypes::FREQ) ||
	   (probe.domain == NFDEData::SliceProbe::DomainTypes::TRAN)) {

		getline(file, line);
      readLine("SLICE PROBE", line, probe.fstart, probe.fstop, probe.fstep);
   }
	if(probe.domain == NFDEData::SliceProbe::DomainTypes::TRAN) {
		getline(file, line);
      readLine("SLICE PROBE", line, probe.filename, false);
   }
   
   getline(file, line);
	readLine("SLICE PROBE", line, probe.region.coords.first (x),
                                 probe.region.coords.first (y),
                                 probe.region.coords.first (z));
   getline(file, line);
	readLine("SLICE PROBE", line, probe.region.coords.second(x),
                                 probe.region.coords.second(y),
                                 probe.region.coords.second(z));

   
   nfde->sliceProbe.push_back(probe);
}
