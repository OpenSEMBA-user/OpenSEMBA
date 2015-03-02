#ifndef NFDEDATA_H_
#define NFDEDATA_H_

#include <complex>
#include <string>
#include <vector>

#include "ProjectFile.h"
#include "math/CartesianVector.h"
#include "geometry/Box.h"
#include "math/Constants.h"

using namespace std;

struct NFDEData : public ProjectFile {
    //	AUXILIARY TYPES
    typedef enum  {
        NONE, METAL, MGMET, NONME
    } MaterialTypes;

    struct CoordsNode {
        CVecI3 coords;
        CoordsNode() {
            coords = CVecI3(-1,-1,-1);
        }
    };

    struct CoordsNewProbe : public CoordsNode {
        typedef enum {
            NONE, EX, EY, EZ, HX, HY, HZ, IW, VG
        } Types;
        Types type;
        CoordsNewProbe()
        :  CoordsNode(), type(Types::NONE) {}
    };

    struct CoordsConfNode : public CoordsNode {
        unsigned dir;
        double len;
        CoordsConfNode() {
            dir = 0;
            len = 0.0;
        }
    };

    struct CoordsConfLine {
        unsigned long node[2];
        CVecD3 norm;
        long label;
        CoordsConfLine() {
            node[0] = node[1] = -1;
            label = 0;
        }
    };

    struct Coords {
        pair<CVecI3, CVecI3> coords;
        Coords() {
            coords.first = CVecI3(0,0,0);
            coords.second = CVecI3(-1,-1,-1);
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
        CoordsDir(pair<CVecI3, CVecI3> c, CartesianAxis d) {
            dir = d;
            coords = c;
        }
        vector<CVecD3> getPos() const {
            vector<CVecI3> pos(4);
            uint rX = dir;
            uint rY = (dir + 1) % 3;
            uint rZ = (dir + 2) % 3;

            pos[0] = coords.first;

            pos[1](rX) = coords.first(rX);
            pos[1](rY) = coords.first(rY);
            pos[1](rZ) = coords.second(rZ);

            pos[2](rX) = coords.first(rX);
            pos[2](rY) = coords.second(rY);
            pos[2](rZ) = coords.second(rZ);

            pos[3](rX) = coords.first(rX);
            pos[3](rY) = coords.second(rY);
            pos[3](rZ) = coords.first(rZ);

            vector<CVecD3> res(4);
            for (uint i = 0; i < 4; i++) {
                for (uint d = 0; d < 3; d++) {
                    res[i](d) = (double) pos[i](d);
                }
            }
            return res;
        }
    };

    struct CoordsLine : public CoordsNode {
        CartesianAxis dir;
        CoordsLine(CoordsMultiplier c) {
            coords = c.coords.first;
            dir = z;
            for (uint d = 0; d < 3; d++) {
                if (c.multiplier(d) != 0.0) {
                    dir = CartesianAxis(d);
                }
            }
        }
        CoordsLine(CoordsDir c) {
            coords = c.coords.first;
            dir = c.dir;
        }
        CoordsLine(CVecI3 c, CartesianAxis d) {
            coords = c;
            dir = d;
        }
        CoordsLine() {
            dir = z;
        }
    };

    struct CoordsWire : public CoordsLine {
        struct Types {
            enum value {
                NONE, CURR, VOLT
            };
        };
        double multiplier;
        int node;
        Types::value srctype;
        string srcfile;
        CoordsWire()
        :   CoordsLine(),
            multiplier(0.0),
            node(-1),
            srctype(Types::NONE),
            srcfile("") {
        }
    };

    struct ObjectInLayer {
        string layer;
        string name;
        string getNameAtLayer() const {
            return name + "@" + layer;
        }
    };

    struct Line : public ObjectInLayer {
        vector<CoordsDir> entities;
    };

    struct Surf : public ObjectInLayer {
        vector<CoordsDir> entities;
    };

    struct Body : public ObjectInLayer {
        vector<Coords> entities;
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

    struct CurrentDensitySource : public ObjectInLayer {
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

    struct FieldSource : public ObjectInLayer {
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

    struct ConformalLines : public ObjectInLayer {
        vector<CoordsConfNode> nodes;
        vector<CoordsConfLine> lines;
    };

    struct IsotropicLine :public Line {
        MaterialTypes type;
        double sigma, eps, mu, sigmam;
        IsotropicLine()
        :	Line(),
         	type(MaterialTypes::NONE),
         	sigma(0.0),
         	eps(VACUUM_PERMITTIVITY),
         	mu(VACUUM_PERMEABILITY),
         	sigmam(0.0) {}
    };

    struct AnisotropicLine : public Line  {
        double sigma[3][3];
        double eps[3][3];
        double mu[3][3];
        double sigmam[3][3];
        AnisotropicLine() {
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

    struct DispersiveLine : public Line {
        double sigma0, epsinf, muinf;
        int K;
        vector< complex<double> > a, b;
        DispersiveLine()
        :	sigma0(0.0),
         	epsinf(VACUUM_PERMITTIVITY),
         	muinf(VACUUM_PERMEABILITY),
         	K(0),
         	a(), b() {}
    };

    struct IsotropicSurf : public Surf {
        MaterialTypes type;
        double sigma, eps, mu, sigmam;
        IsotropicSurf()
        :	type(MaterialTypes::NONE),
         	sigma(0.0),
         	eps(VACUUM_PERMITTIVITY),
         	mu(VACUUM_PERMEABILITY),
         	sigmam(0.0) {}
    };

    struct AnisotropicSurf : public Surf {
        double sigma[3][3];
        double eps[3][3];
        double mu[3][3];
        double sigmam[3][3];
        AnisotropicSurf() {
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
    struct DispersiveSurf : public Surf {
        double sigma0, epsinf, muinf;
        int K;
        vector< complex<double> > a, b;
        DispersiveSurf()
        :	sigma0(0.0),
         	epsinf(VACUUM_PERMITTIVITY),
         	muinf(VACUUM_PERMEABILITY),
         	K(0),
         	a(), b() {}
    };
    struct CompositeSurf : public Surf {
        vector<double> sigma, eps, mu, sigmam, thk;
        int numberoflayers;
        CompositeSurf()
        :	numberoflayers(0) {}
    };

    struct IsotropicBody : public Body {
        MaterialTypes type;
        double sigma, eps, mu, sigmam;
        IsotropicBody()
        :	type(MaterialTypes::NONE),
         	sigma(0.0),
         	eps(VACUUM_PERMITTIVITY),
         	mu(VACUUM_PERMEABILITY),
         	sigmam(0.0) {}
    };

    struct AnisotropicBody : public Body {
        double sigma[3][3];
        double eps[3][3];
        double mu[3][3];
        double sigmam[3][3];
        AnisotropicBody() {
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
    struct DispersiveBody : public Body {
        double sigma0, epsinf, muinf;
        int K;
        vector< complex<double> > a, b;
        DispersiveBody()
        :	sigma0(0.0),
         	epsinf(VACUUM_PERMITTIVITY),
         	muinf(VACUUM_PERMEABILITY),
         	K(0),
         	a(), b() {}
    };

    struct ThinWire : public ObjectInLayer {
        typedef enum  {
            NONE, MATERIAL, PARALLEL, SERIES
        } Extremes;
        double rad, res, ind;
        Extremes tl, tr;
        double rtl, itl, ctl;
        double rtr, itr, ctr;
        int enl, enr;
        vector<CoordsWire> segments;
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
        struct Coords : public CoordsLine {
            int node;
            Coords() :
                CoordsLine(),
                node(-1) {}
        };
        double width;
        vector<Coords> gaps;
        ThinGap()
        :	width(0.0),
         	gaps() {}
    };

    struct NewProbe : public Probe {
        struct DomainTypes {
            enum value {
                NONE, TIME, FREQ, TRAN, TIFR, TITR, FRTR, ALL
            };
        };
        DomainTypes::value domain;
        double tstart, tstop, tstep;
        double fstart, fstop, fstep;
        string filename, transFilename;
        vector<CoordsNewProbe> entities;
        NewProbe()
        : 	domain(DomainTypes::NONE),
          	tstart(0.0), tstop(-1.0), tstep(0.0),
          	fstart(0.0), fstop(-1.0), fstep(0.0),
          	filename(""),
          	entities() {

        }
    };
    struct BulkProbe : public Probe {
        typedef enum {
            NONE, ELECT, MAGNE
        } Types;
        Types type;
        double tstart, tstop, tstep;
        CoordsDir entities;
        int skip;
        BulkProbe()
        :	type(Types::NONE), tstart(0.0), tstop(-1.0), tstep(0.0),
         	entities(), skip(0) {}
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
        Coords entities;
        SliceProbe()
        :	field(FieldTypes::NONE),
         	domain(DomainTypes::NONE),
         	tstart(0.0), tstop(-1.0), tstep(0.0),
         	fstart(0.0), fstop(-1.0), fstep(0.0),
         	filename(""),
         	entities() {}
    };

    struct TraditionalProbe : public Probe {
        double fstart, fstop, fstep;
        double thStart, thEnd, thStep;
        double phStart, phEnd, phStep;
        string filename;
        Coords entities;
        TraditionalProbe()
        :	fstart(0.0), fstop(-1.0), fstep(0.0),
         	thStart(0.0), thEnd(-1.0), thStep(0.0),
         	phStart(0.0), phEnd(-1.0), phStep(0.0),
         	filename(""),
         	entities() {}
    };

    TimeSteps  timesteps;
    SpaceSteps spacesteps[3];
    Background background;
    Boundary   boundaries[3][2];

    vector<PlaneWaveSource>      planeWaveSource;
    vector<CurrentDensitySource> currentDensitySource;
    vector<FieldSource>          fieldSource;

    vector<ConformalLines> conformalLines;

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

    vector<ThinWire> thinWire;
    //    vector<ThinGap> thinGap;

    vector<TraditionalProbe> traditionalProbe;
    vector<NewProbe>         newProbe;
    vector<BulkProbe>        bulkProbe;
    vector<SliceProbe>       sliceProbe;

    NFDEData() {}

    // Helper functions
    BoxI3 getFullDomainBox() const {
        CVecI3 min(spacesteps[x].m, spacesteps[y].m, spacesteps[z].m);
        CVecI3 max(spacesteps[x].n, spacesteps[y].n, spacesteps[z].n);
        return BoxI3(min,max);
    }

};

#endif /* NFDEDATA_H_ */
