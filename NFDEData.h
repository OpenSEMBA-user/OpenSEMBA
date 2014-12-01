#ifndef NFDEDATA_H_
#define NFDEDATA_H_

#include <complex>
#include <string>
#include <vector>

using namespace std;

#include "CartesianVector.h"
#include "Constants.h"

struct NFDEData {
//	AUXILIARY TYPES
   struct MaterialTypes {
      enum value {
         NONE, METAL, MGMET, NONME
      };
   };

   struct Coords {
      pair<CVecI3, CVecI3> coords;
      Coords() {
         for(int d = 0; d < 3; d++) {
            coords.first (d) = 0;
            coords.second(d) = -1;
         }
      }
   };

   struct CoordsMultiplier : public Coords {
      CVecD3 multiplier;
      CoordsMultiplier() : Coords() {
         for(int d = 0; d < 3; d++) {
            multiplier(d) = 0.0;
         }
      }
   };

   struct CoordsDir : public Coords {
      CartesianAxis dir;
      CoordsDir()
      : Coords(), dir(x) {}
   };

   struct ObjectInLayer {
      string layer;
      ObjectInLayer()
      :  layer("") {}
   };
   
   struct Probe : public ObjectInLayer {
      bool log;
      Probe()
      :  log(false) {}
   };
   
//	MAIN TYPES
	struct TimeSteps {
		double dt;
		long nmax;
		TimeSteps()
		: 	dt(1.0),
		  	nmax(-1) {}
	};

	struct SpaceSteps {
		double origin;
		long int m, n;
		bool cons;
		vector<double> d;
		SpaceSteps()
		:	origin(-1.0),
		 	m(0),
		 	n(-1),
		 	cons(false),
		 	d() {}
	};
	struct Background {
		double sigma, eps, mu, sigmam;
		Background()
		:	sigma(0.0),
		 	eps(VACUUM_PERMITTIVITY),
		 	mu(VACUUM_PERMEABILITY),
		 	sigmam(0.0) {}
	};
	struct Boundary {
		struct Types {
			enum value {
				NONE, PEC, PMC, SYMMETRIC, 
            PERIODIC, MUR1, MUR2, PML
			};
		};
		struct PeriodicTypes {
			enum value {
				NONE, ETAN, HTAN
			};
		};
		Types::value bctype;
		PeriodicTypes::value sptype;
		int lay;
		double order, ref;
		Boundary()
		:	bctype(Types::NONE),
		 	sptype(PeriodicTypes::NONE),
		 	lay(0),
		 	order(-1.0),
		 	ref(-1.0) {}
	};

	struct PlaneWaveSource {
		string filename;
		Coords coords;
		double theta, phi, alpha, beta;
		PlaneWaveSource()
		:	filename(""),
			coords(),
			theta(0.0),
		 	phi(0.0),
		 	alpha(0.0),
		 	beta(0.0) {}
	};

	struct CurrentDensitySource {
		struct Types {
			enum value {
				NONE, ELECT, MAGNE
			};
		};
		Types::value type;
		string filename;
		vector<CoordsMultiplier> entities;
		CurrentDensitySource()
		:	type(Types::NONE),
		 	filename(),
		 	entities() {}
	};
	struct FieldSource {
		struct Types {
			enum value {
				NONE, ELECT, MAGNE
			};
		};
		Types::value type;
		string filename;
		vector<CoordsMultiplier> entities;
		FieldSource()
		:	type(Types::NONE),
		 	filename(),
		 	entities() {}
	};

	struct IsotropicBody : public ObjectInLayer {
		MaterialTypes::value type;
		double sigma, eps, mu, sigmam;
		vector<Coords> entities;
		IsotropicBody()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(VACUUM_PERMITTIVITY),
		 	mu(VACUUM_PERMEABILITY),
		 	sigmam(0.0),
		 	entities() {}
	};

	struct IsotropicSurf : public ObjectInLayer {
		MaterialTypes::value type;
		double sigma, eps, mu, sigmam;
		vector<CoordsDir> entities;
		IsotropicSurf()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(VACUUM_PERMITTIVITY),
		 	mu(VACUUM_PERMEABILITY),
		 	sigmam(0.0) {}
	};

	struct IsotropicLine : public ObjectInLayer {
		MaterialTypes::value type;
		double sigma, eps, mu, sigmam;
		vector<CoordsDir> entities;
		IsotropicLine()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(VACUUM_PERMITTIVITY),
		 	mu(VACUUM_PERMEABILITY),
		 	sigmam(0.0) {}
	};

	struct AnisotropicBody : public ObjectInLayer {
		double sigma[3][3];
		double eps[3][3];
		double mu[3][3];
		double sigmam[3][3];
		vector<Coords> entities;
		AnisotropicBody()
		:	entities() {
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = VACUUM_PERMITTIVITY;
				mu[i][i] = VACUUM_PERMEABILITY;
			}
		}
	};
	struct AnisotropicSurf : public ObjectInLayer {
		double sigma[3][3];
		double eps[3][3];
		double mu[3][3];
		double sigmam[3][3];
		vector<CoordsDir> entities;
		AnisotropicSurf()
		:	entities() {
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = VACUUM_PERMITTIVITY;
				mu[i][i] = VACUUM_PERMEABILITY;
			}
		}
	};
	struct AnisotropicLine : public ObjectInLayer {
		double sigma[3][3];
		double eps[3][3];
		double mu[3][3];
		double sigmam[3][3];
		vector<CoordsDir> entities;
		AnisotropicLine()
		:	entities() {
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = VACUUM_PERMITTIVITY;
				mu[i][i] = VACUUM_PERMEABILITY;
			}
		}
	};

	struct DispersiveBody : public ObjectInLayer {
		double sigma0, epsinf, muinf;
		int K;
		vector< complex<double> > a, b;
		vector<Coords> entities;
		DispersiveBody()
		:	sigma0(0.0),
         epsinf(VACUUM_PERMITTIVITY),
         muinf(VACUUM_PERMEABILITY),
		 	K(0),
		 	a(), b() {}
	};

	struct DispersiveSurf : public ObjectInLayer {
		double sigma0, epsinf, muinf;
		int K;
		vector< complex<double> > a, b;
		vector<CoordsDir> entities;
		DispersiveSurf()
		:	sigma0(0.0),
         epsinf(VACUUM_PERMITTIVITY),
         muinf(VACUUM_PERMEABILITY),
		 	K(0),
		 	a(), b() {}
	};
	struct DispersiveLine : public ObjectInLayer {
		double sigma0, epsinf, muinf;
		int K;
		vector< complex<double> > a, b;
		vector<CoordsDir> entities;
		DispersiveLine()
		:	sigma0(0.0),
         epsinf(VACUUM_PERMITTIVITY),
         muinf(VACUUM_PERMEABILITY),
		 	K(0),
		 	a(), b() {}
	};

	struct CompositeSurf : public ObjectInLayer {
		double sigma, eps, mu, sigmam, thk;
      string name;
      int numberoflayers;
		vector<CoordsDir> entities;
		CompositeSurf()
		:	sigma(0.0),
		 	eps(VACUUM_PERMITTIVITY),
		 	mu(VACUUM_PERMEABILITY),
		 	sigmam(0.0),
		 	thk(-1.0),
         name(""),
         numberoflayers(1),
		 	entities() {}
	};

	struct ThinWire : public ObjectInLayer {
		struct Extremes {
			enum value {
				NONE, MATERIAL, PARALLEL, SERIES
			};
		};
		struct Coords {
			struct Types {
				enum value {
					NONE, CURR, VOLT
				};
			};
			CVecI3 coords;
			CartesianAxis dir;
			double multiplier;
			int node;
			Types::value srctype;
			string srcfile;
			Coords()
			:	dir(z),
			 	multiplier(0.0),
			 	node(-1),
			 	srctype(Types::NONE),
			 	srcfile("") {

				for(int i = 0; i < 3; i++)
					coords(i) = -1;
			}
		};
		double rad, res, ind;
		Extremes::value tl, tr;
		double rtl, itl, ctl;
		double rtr, itr, ctr;
		int enl, enr;
		vector<Coords> segments;
		ThinWire()
		:	rad(-1.0), res(0.0), ind(0.0),
		 	tl(Extremes::NONE), tr(Extremes::NONE),
		 	rtl(0.0), itl(0.0), ctl(0.0),
		 	rtr(0.0), itr(0.0), ctr(0.0),
		 	enl(-1), enr(-1),
		 	segments() {

		}
	};
	struct ThinGap : public ObjectInLayer {
		struct Coords {
			CVecI3 coords;
			CartesianAxis dir;
			int node;
			Coords()
			:	dir(x),
			 	node(-1) {

				for(int i = 0; i < 3; i++)
					coords(i) = -1;
			}
		};
		double width;
		vector<Coords> gaps;
		ThinGap()
		:	width(0.0),
		 	gaps() {}
	};

	struct TraditionalProbe : public Probe {
		double fstart, fstop, fstep;
		double thStart, thEnd, thStep;
		double phStart, phEnd, phStep;
      string filename;
      Coords region;
		TraditionalProbe()
		:	fstart(0.0), fstop(-1.0), fstep(0.0),
			thStart(0.0), thEnd(-1.0), thStep(0.0),
			phStart(0.0), phEnd(-1.0), phStep(0.0),
         filename(""),
         region() {}
	};
	
	struct NewProbe : public Probe {
		struct DomainTypes {
			enum value {
				NONE, TIME, FREQ, TRAN, TIFR, TITR, FRTR, ALL
			};
		};
		struct Coords {
			struct Types {
				enum value {
					NONE, EX, EY, EZ, HX, HY, HZ, IW, VG
				};
			};
			Types::value type;
			CVecI3 coords;
			Coords()
			:	type(Types::NONE) {

				for(int i = 0; i < 3; i++)
					coords(i) = -1;
			}
		};
		DomainTypes::value domain;
		double tstart, tstop, tstep;
		double fstart, fstop, fstep;
		string filename, transFilename;
		vector<Coords> probes;
		NewProbe()
		: 	domain(DomainTypes::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
		 	fstart(0.0), fstop(-1.0), fstep(0.0),
		 	filename(""),
		 	probes() {

		}
	};
	struct BulkProbe : public Probe {
		struct Types {
			enum value {
				NONE, ELECT, MAGNE
			};
		};
		Types::value type;
		double tstart, tstop, tstep;
		CoordsDir coord;
		int skip;
		BulkProbe()
		:	tstart(0.0), tstop(-1.0), tstep(0.0),
		 	coord(), skip(0) {}
	};
   
	struct SliceProbe : public Probe {
		struct FieldTypes {
			enum value {
				NONE, BC, BCX, BCY, BCZ, ME, MH, EX, EY,EZ, HX, HY, HZ
			};
		};
		struct DomainTypes {
			enum value {
				NONE, TIME, FREQ, TRAN
			};
		};
		FieldTypes::value field;
		DomainTypes::value domain;
		double tstart, tstop, tstep;
		double fstart, fstop, fstep;
		string filename;
      Coords region;
		SliceProbe()
		:	field(FieldTypes::NONE),
		 	domain(DomainTypes::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
			fstart(0.0), fstop(-1.0), fstep(0.0),
         filename(""),
         region() {}
	};
   
	//	MAIN

	//	GENERAL INFORMATION
	TimeSteps  timesteps;
	//	SPATIAL INFORMATION
	SpaceSteps spacesteps[3];
	Background background;
	Boundary   boundaries[3][2];
	//	SOURCES
	vector<PlaneWaveSource>      planeWaveSource;
	vector<CurrentDensitySource> currentDensitySource;
	vector<FieldSource>          fieldSource;
	//	MATERIALS
	vector<IsotropicBody> isotropicBody;
	vector<IsotropicSurf> isotropicSurf;
	vector<IsotropicLine> isotropicLine;
   
	vector<AnisotropicBody> anisotropicBody;
	vector<AnisotropicSurf> anisotropicSurf;
	vector<AnisotropicLine> anisotropicLine;
   
	vector<DispersiveBody> dispersiveBody;
	vector<DispersiveSurf> dispersiveSurf;
	vector<DispersiveLine> dispersiveLine;
   
	vector<CompositeSurf> compositeSurf;
	//	THIN MATERIALS
	vector<ThinWire> thinWire;
	vector<ThinGap>  thinGap;
	//	PROBES
	vector<TraditionalProbe> traditionalProbe;
	vector<NewProbe>         newProbe;
	vector<BulkProbe>        bulkProbe;
   vector<SliceProbe>       sliceProbe;

	NFDEData() {}
};

#endif /* NFDEDATA_H_ */
