/*
 *        File: File.hpp
 * -------------------------------------------------------------------------
 *      Author: damarro
 *  Created on: Mar 11, 2014
 * Description:	Handler of a NFDE file
 *              
 */
#ifndef NFDEDATA_H_
#define NFDEDATA_H_

#include <string>
#include <vector>
using namespace std;

namespace NFDE {

typedef int Integer;
typedef double Real;

const Real eps0 = (Real)8.8541878176203898505365630317107502606083701665994498081024171524053950954599821142852891607182008932e-12L;
const Real mu0  = (Real)1.2566370614359172953850573533118011536788677597500423283899778369231265625144835994512139301368468271e-6L;

enum Dirs {
	iX = 0, iY = 1, iZ = 2,
	dirs_beg = iX, dirs_end = iZ
};
const Integer dirs_size = 3;

enum Pos {
	start = 0, final = 1,
	pos_beg = start, pos_end = final
};

const Integer pos_size = 2;

struct Border {
	Integer beg;
	Integer end;
	Border()
	:	beg(0),
	 	end(-1) {
	}
	Border(const Border &other)
	:	beg(other.beg),
	 	end(other.end) {

	}
};

struct MaterialTypes {
	enum value {
		NONE, METAL, MGMET, NONME
	};
};

struct Coords {
	Border coords[dirs_size];
	Coords() {
		for(Integer d = dirs_beg; d <= dirs_end; d++) {
			coords[d].beg = 0;
			coords[d].end = -1;
		}
	}
};

struct CoordsMultiplier : public Coords {
	Real multiplier[dirs_size];
	CoordsMultiplier() : Coords() {
		for(Integer d = dirs_beg; d <= dirs_end; d++) {
			multiplier[d] = 0.0;
		}
	}
};

struct CoordsDir : public Coords {
	Dirs dir;
	CoordsDir()
	: Coords(), dir(iX) {
	}
};

struct NFDEData {
	//	AUXILIARY TYPES

	struct TwoDimensions {
		struct Confs {
			enum value {
				NONE, ENORM, HNORM
			};
		};
		Dirs plane;
		Integer index;
		Confs::value conf;
		TwoDimensions()
		:	plane(iX),
		 	index(-1),
		 	conf(Confs::NONE) {

		}
	};
	struct TimeSteps {
		Real dt;
		Integer nmax;
		TimeSteps()
		: 	dt(1.0),
		  	nmax(-1) {

		}
	};

	struct SpaceSteps { // TODO Esto no está bien documentado.
		Real origin;
		Integer m, n; // TODO Estos nombres no aportan información de lo que se espera que se almacene en estas variables.
		bool cons;
		vector<Real> d;
		SpaceSteps()
		:	origin(-1.0),
		 	m(0),
		 	n(-1),
		 	cons(false),
		 	d() {

		}
	};
	struct Background {
		Real sigma, eps, mu, sigmam;
		Real rho, p;
		Background()
		:	sigma(0.0),
		 	eps(eps0),
		 	mu(mu0),
		 	sigmam(0.0),
		 	rho(0.0),
		 	p(0.0) {

		}
	};
	struct Boundary {
		struct Types {
			enum value {
				NONE, PEC, PMC, SYMMETRIC, PERIODIC,
				LOFREQ, ELOFREQ, MUR1, MUR2, FANG_MEI, PML
			};
		};
		struct PeriodicTypes {
			enum value {
				NONE, ETAN, HTAN
			};
		};
		Types::value bctype;
		PeriodicTypes::value sptype;
		Integer lay;
		Real order, ref;
		Boundary()
		:	bctype(Types::NONE),
		 	sptype(PeriodicTypes::NONE),
		 	lay(0),
		 	order(-1.0),
		 	ref(-1.0) {

		}
	};

	struct PlaneWaveSource {
		struct Attributes {
			enum value {
				NONE, MODIFY, LOCKED
			};
		};

		string filename;
		Attributes::value attribute;
		Coords coords;
		Real theta, phi, alpha, beta;
		PlaneWaveSource()
		:	filename(""),
		 	attribute(Attributes::NONE),
			coords(),
			theta(0.0),
		 	phi(0.0),
		 	alpha(0.0),
		 	beta(0.0)
			 {
		}
	};
	struct BoxInSource {
		string filename;
		Border coords[dirs_size];
		BoxInSource()
		:	filename("") {

			for(Integer d = dirs_beg; d <= dirs_end; d++) {
				coords[d].beg = 0;
				coords[d].end = -1;
			}
		}
	};
	struct GammaSource {
		string filename;
		Real theta, phi;
		GammaSource()
		:	theta(0.0),
		 	phi(0.0) {

		}
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
		 	entities() {
		}
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
		 	entities() {

		}
	};

	struct IsotropicBody {
		MaterialTypes::value type;
		Real sigma, eps, mu, sigmam;
		vector<Coords> entities;
		IsotropicBody()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(eps0),
		 	mu(mu0),
		 	sigmam(0.0),
		 	entities() {

		}
	};

	struct IsotropicSurf {
		MaterialTypes::value type;
		Real sigma, eps, mu, sigmam;
		vector<CoordsDir> entities;
		IsotropicSurf()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(eps0),
		 	mu(mu0),
		 	sigmam(0.0) {}
	};

	struct IsotropicLine {
		MaterialTypes::value type;
		Real sigma, eps, mu, sigmam;
		vector<CoordsDir> entities;
		IsotropicLine()
		:	type(MaterialTypes::NONE),
		 	sigma(0.0),
		 	eps(eps0),
		 	mu(mu0),
		 	sigmam(0.0) {}
	};

	struct AnisotropicBody {
		Real sigma[dirs_size][dirs_size];
		Real eps[dirs_size][dirs_size];
		Real mu[dirs_size][dirs_size];
		Real sigmam[dirs_size][dirs_size];
		vector<Coords> entities;
		AnisotropicBody()
		:	entities() {
			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				for(Integer j = dirs_beg; j <= dirs_end; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = eps0;
				mu[i][i] = mu0;
			}
		}
	};
	struct AnisotropicSurf {
		Real sigma[dirs_size][dirs_size];
		Real eps[dirs_size][dirs_size];
		Real mu[dirs_size][dirs_size];
		Real sigmam[dirs_size][dirs_size];
		vector<CoordsDir> entities;
		AnisotropicSurf()
		:	entities() {
			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				for(Integer j = dirs_beg; j <= dirs_end; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = eps0;
				mu[i][i] = mu0;
			}
		}
	};
	struct AnisotropicLine {
		Real sigma[dirs_size][dirs_size];
		Real eps[dirs_size][dirs_size];
		Real mu[dirs_size][dirs_size];
		Real sigmam[dirs_size][dirs_size];
		vector<CoordsDir> entities;
		AnisotropicLine()
		:	entities() {
			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				for(Integer j = dirs_beg; j <= dirs_end; j++) {
					sigma[i][j] = 0.0;
					eps[i][j] = 0.0;
					mu[i][j] = 0.0;
					sigmam[i][j] = 0.0;
				}
				eps[i][i] = eps0;
				mu[i][i] = mu0;
			}
		}
	};

	struct DispersiveBody {
		Real sigma0, epsinf, muinf, sigmam0;
		Integer K, L, Km, Lm;
		vector<Real> a, b;
		vector<Real> alfa, beta, gamma;
		vector<Real> am, bm;
		vector<Real> alfam, betam, gammam;
		vector<Coords> entities;
		DispersiveBody()
		:	sigma0(0.0), epsinf(eps0), muinf(mu0), sigmam0(0.0),
		 	K(0), L(0), Km(0), Lm(0),
		 	a(), b(),
		 	alfa(), beta(), gamma(),
		 	am(), bm(),
		 	alfam(), betam(), gammam() {}
	};

	struct DispersiveSurf {
		Real sigma0, epsinf, muinf, sigmam0;
		Integer K, L, Km, Lm;
		vector<Real> a, b;
		vector<Real> alfa, beta, gamma;
		vector<Real> am, bm;
		vector<Real> alfam, betam, gammam;
		vector<CoordsDir> entities;
		DispersiveSurf()
		:	sigma0(0.0), epsinf(eps0), muinf(mu0), sigmam0(0.0),
		 	K(0), L(0), Km(0), Lm(0),
		 	a(), b(),
		 	alfa(), beta(), gamma(),
		 	am(), bm(),
		 	alfam(), betam(), gammam(),
		 	entities() {

		}
	};
	struct DispersiveLine {
		Real sigma0, epsinf, muinf, sigmam0;
		Integer K, L, Km, Lm;
		vector<Real> a, b;
		vector<Real> alfa, beta, gamma;
		vector<Real> am, bm;
		vector<Real> alfam, betam, gammam;
		vector<CoordsDir> entities;
		DispersiveLine()
		:	sigma0(0.0), epsinf(eps0), muinf(mu0), sigmam0(0.0),
		 	K(0), L(0), Km(0), Lm(0),
		 	a(), b(),
		 	alfa(), beta(), gamma(),
		 	am(), bm(),
		 	alfam(), betam(), gammam(),
		 	entities() {

		}
	};

	struct CompositeSurf {
		MaterialTypes::value type;
		Real sigma, eps, mu, sigmam, thk;
		vector<CoordsDir> entities;
		CompositeSurf()
		:	type(MaterialTypes::NONME),
		 	sigma(0.0),
		 	eps(eps0),
		 	mu(mu0),
		 	sigmam(0.0),
		 	thk(-1.0),
		 	entities() {

		}
	};

	struct ThinWire {
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
			Integer coords[dirs_size];
			Dirs dir;
			Real multiplier;
			Integer node;
			Types::value srctype;
			string srcfile;
			Coords()
			:	dir(iZ),
			 	multiplier(0.0),
			 	node(-1),
			 	srctype(Types::NONE),
			 	srcfile("") {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		Real rad, res, ind;
		Extremes::value tl, tr;
		Real rtl, itl, ctl;
		Real rtr, itr, ctr;
		Integer enl, enr;
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
	struct ThinGap {
		struct Coords {
			Integer coords[dirs_size];
			Dirs dir;
			Integer node;
			Coords()
			:	dir(iX),
			 	node(-1) {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		Real width;
		vector<Coords> gaps;
		ThinGap()
		:	width(0.0),
		 	gaps() {

		}
	};

	struct TraditionalProbe {
		struct Types {
			enum value {
				NONE, EF, HF, NE, NH, JS, MS, IW, VG, BO, FE, FH
			};
		};
		struct Caltypes {
			enum value {
				NONE, FULL, PART
			};
		};

		struct FieldCoords {
			Integer coords[dirs_size];
			Integer node;
			FieldCoords()
			:	node(-1) {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		struct SurfaceCoords {
			Integer coords[dirs_size];
			Integer node;
			Dirs nor;
			SurfaceCoords()
			:	node(-1),
			 	nor(iX) {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		struct NodeCoords {
			Integer node;
			NodeCoords()
			:	node(-1) {

			}
		};
		struct FarfieldCoords {
			Real coords[dirs_size];
			string name;
			FarfieldCoords()
			:	name("") {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		Types::value type;
		string grname;
		string filename;             // Not in EMIN. Needed by SGG.
		string transferFilename;	 // Not in EMIN. Needed by SGG.
		Real tstart, tstop, tstep;
		Real fstart, fstop, fstep;   // Not in EMIN. Needed by SGG.
		Caltypes::value caltype;
		Coords region;
		vector<   FieldCoords> field_coords;
		vector< SurfaceCoords> surface_coords;
		vector<    NodeCoords> node_coords;
		vector<FarfieldCoords> farfield_coords;
		Real thStart, thEnd, thStep; // Not in EMIN. Needed by SGG.
		Real phStart, phEnd, phStep; // Not in EMIN. Needed by SGG.
		TraditionalProbe()
		:	type(Types::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
			fstart(0.0), fstop(-1.0), fstep(0.0),
		 	caltype(Caltypes::NONE),
			region(),
		 	field_coords(),
		 	surface_coords(),
		 	node_coords(),
		 	farfield_coords(),
			thStart(0.0), thEnd(-1.0), thStep(0.0),
			phStart(0.0), phEnd(-1.0), phStep(0.0)
			{}
	};

	struct ScreenProbe {
		struct Coords {
			struct Types {
				enum value {
					NONE, EX, EY, EZ, HX, HY, HZ, IW, VG
				};
			};
			Types::value type;
			Integer coords[dirs_size];
			Coords()
			:	type(Types::NONE) {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		Real tstart, tstop, tstep;
		vector<Coords> probes;
		ScreenProbe()
		:	tstart(0.0), tstop(-1.0), tstep(0.0),
		 	probes() {

		}
	};
	struct NewProbe {
		struct DataTypes {
			enum value {
				NONE, NUMER, PRONY, BOTH
			};
		};
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
			Integer coords[dirs_size];
			Coords()
			:	type(Types::NONE) {

				for(Integer i = dirs_beg; i <= dirs_end; i++)
					coords[i] = -1;
			}
		};
		DataTypes::value type1;
		DomainTypes::value type2;
		Real tstart, tstop, tstep;
		Real fstart, fstop, fstep;
		string filename, transFilename;
		vector<Coords> probes;
		NewProbe()
		:	type1(DataTypes::NONE),
		 	type2(DomainTypes::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
		 	fstart(0.0), fstop(-1.0), fstep(0.0),
		 	probes() {

		}
	};
	struct BulkProbe {
		struct Types {
			enum value {
				NONE, ELECT, MAGNE
			};
		};
		Types::value type;
		Real tstart, tstop, tstep;
		CoordsDir coord;
		Integer skip;
		BulkProbe()
		:	type(Types::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
		 	coord(), skip(0) {}
	};
	struct StructureProbe {
		struct PageOrientations {
			enum value {
				NONE, H, V
			};
		};
		struct ShadeTypes {
			enum value {
				NONE, BW, CO
			};
		};
		struct CoordinateTypes {
			enum value {
				NONE, CARTESIAN, SPHERICAL
			};
		};
		struct MeshOptions {
			enum value {
				NONE, MESHLINES, NOMESHLINES
			};
		};
		struct CutawayOptions {
			enum value {
				NONE, CUTAWAY, NOCUTAWAY
			};
		};
		struct Figure {
			struct Materials {
				enum value {
					NONE, PEC, PMC, ISO, ANI,
					FRD, COM, THW, ALL
				};
			};
			struct Colors {
				enum value {
					NONE, B, C, G, Y, O, R, L, K
				};
			};
			Materials::value mat;
			Colors::value col;
			Figure()
			:	mat(Materials::NONE),
			 	col(Colors::NONE) {

			}
		};
		PageOrientations::value pgor;
		ShadeTypes::value shty;
		CoordinateTypes::value coty;
		Real observer[dirs_size];
		MeshOptions::value ms;
		Border region[dirs_size];
		CutawayOptions::value cut;
		Border cut_region[dirs_size];
		vector<Figure> figures;
		StructureProbe()
		:	pgor(PageOrientations::NONE),
		 	shty(ShadeTypes::NONE),
		 	coty(CoordinateTypes::NONE),
		 	ms(MeshOptions::NONE),
		 	cut(CutawayOptions::NONE),
		 	figures() {

			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				observer[i] = 0.0;
				region[i].beg = 0;
				region[i].end = -1;
				cut_region[i].beg = 0;
				cut_region[i].end = -1;
			}
		}
	};
	struct FineStructureProbe {
		struct Types {
			enum value {
				NONE, NE, NH, JS, MS
			};
		};
		Types::value type;
		Dirs coorac, coordn;
		Border coords[dirs_size];
		FineStructureProbe()
		:	type(Types::NONE),
		 	coorac(iX), coordn(iX) {

			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				coords[i].beg = 0;
				coords[i].end = -1;
			}
		}
	};
	struct PictureProbe {
		struct Types {
			enum value {
				NONE, NE, NH, JS, MS
			};
		};
		struct PictureTypes {
			enum value {
				NONE, TIME, PEAK, FREQ, TRAN
			};
		};
		struct PageOrientations {
			enum value {
				NONE, H, V
			};
		};
		struct ShadeTypes {
			enum value {
				NONE, BW, CO
			};
		};
		struct ScaleTypes {
			enum value {
				NONE, LIN, LOG
			};
		};
		struct ReferencePositions {
			enum value {
				NONE, TOP, CENTER, BOTTOM, ADJUST
			};
		};
		struct CoordinateTypes {
			enum value {
				NONE, CARTESIAN, SPHERICAL
			};
		};
		struct ArrowOptions {
			enum value {
				NONE, ARROWS, NOARROWS
			};
		};
		struct MeshOptions {
			enum value {
				NONE, MESHLINES, NOMESHLINES
			};
		};
		struct CutawayOptions{
			enum value {
				NONE, CUTAWAY, NOCUTAWAY
			};
		};
		struct Figure {
			struct Materials {
				enum value {
					NONE, PEC, PMC, ISO, ANI,
					FRD, COM, THW, ALL
				};
			};
			struct Colors {
				enum value {
					NONE, B, C, G, Y, O, R, L, K
				};
			};
			Materials::value mat;
			Colors::value col;
			Figure()
			:	mat(Materials::NONE),
			 	col(Colors::NONE) {

			}
		};
		Types::value type1;
		PictureTypes::value type2;
		Real start, stop, step;
		string filename;
		PageOrientations::value pgor;
		ShadeTypes::value shty;
		ScaleTypes::value scty;
		Real fact, ref;
		ReferencePositions::value refpos;
		CoordinateTypes::value coty;
		Real observer[dirs_size];
		ArrowOptions::value ar;
		MeshOptions::value ms;
		Border region[dirs_size];
		CutawayOptions::value cut;
		Border cut_region[dirs_size];
		vector<Figure> figures;
		PictureProbe()
		:	type1(Types::NONE),
		 	type2(PictureTypes::NONE),
		 	start(0.0), stop(-1.0), step(0.0),
		 	filename(""),
		 	pgor(PageOrientations::NONE),
		 	shty(ShadeTypes::NONE),
		 	scty(ScaleTypes::NONE),
		 	fact(0.0), ref(0.0),
		 	refpos(ReferencePositions::NONE),
		 	coty(CoordinateTypes::NONE),
		 	ar(ArrowOptions::NONE),
		 	ms(MeshOptions::NONE),
		 	cut(CutawayOptions::NONE),
		 	figures() {

			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				observer[i] = 0.0;
				region[i].beg = 0;
				region[i].end = -1;
				cut_region[i].beg = 0;
				cut_region[i].end = -1;
			}
		}
	};

	struct SliceProbe {
		struct Types {
			enum value {
				NONE, NE, NH, TE, TH, JS, MS
			};
		};
		struct PictureTypes {
			enum value {
				NONE, TIME, PEAK, FREQ, TRAN
			};
		};
		struct PageOrientations {
			enum value {
				NONE, H, V
			};
		};
		struct ShadeTypes {
			enum value {
				NONE, BW, CO
			};
		};
		struct ScaleTypes {
			enum value {
				NONE, LIN, LOG
			};
		};
		struct ReferencePositions {
			enum value {
				NONE, TOP, CENTER, BOTTOM, ADJUST
			};
		};
		struct CoordinateTypes {
			enum value {
				NONE, CARTESIAN, SPHERICAL
			};
		};
		struct ArrowOptions {
			enum value {
				NONE, ARROWS, NOARROWS
			};
		};
		struct MeshOptions {
			enum value {
				NONE, MESHLINES, NOMESHLINES
			};
		};
		struct CutawayOptions{
			enum value {
				NONE, CUTAWAY, NOCUTAWAY
			};
		};
		struct MaterialOptions {
			enum value {
				NONE, MATERIAL, NOMATERIAL
			};
		};
		struct Figure {
			struct Materials {
				enum value {
					NONE, PEC, PMC, ISO, ANI,
					FRD, COM, THW, ALL
				};
			};
			struct Colors {
				enum value {
					NONE, B, C, G, Y, O, R, L, K
				};
			};
			Materials::value mat;
			Colors::value col;
			Figure()
			:	mat(Materials::NONE),
			 	col(Colors::NONE) {

			}
		};
		Types::value type;
		PictureTypes::value pictureType;
		Real tstart, tstop, tstep;
		Real fstart, fstop, fstep;
		string filename, transFilename;
		PageOrientations::value pgor;
		ShadeTypes::value shty;
		ScaleTypes::value scty;
		Real fact, ref;
		ReferencePositions::value refpos;
		CoordinateTypes::value coty;
		Real observer[dirs_size];
		ArrowOptions::value ar;
		MeshOptions::value sliceml;
		Coords region;
		CutawayOptions::value cut;
		Coords cut_region;
		MaterialOptions::value mar;
		MeshOptions::value matml;
		vector<Figure> figures;
		SliceProbe()
		:	type(Types::NONE),
		 	pictureType(PictureTypes::NONE),
		 	tstart(0.0), tstop(-1.0), tstep(0.0),
			fstart(0.0), fstop(-1.0), fstep(0.0),
		 	pgor(PageOrientations::NONE),
		 	shty(ShadeTypes::NONE),
		 	scty(ScaleTypes::NONE),
		 	fact(0.0), ref(0.0),
		 	refpos(ReferencePositions::NONE),
		 	coty(CoordinateTypes::NONE),
		 	ar(ArrowOptions::NONE),
		 	sliceml(MeshOptions::NONE),
			region(),
		 	cut(CutawayOptions::NONE),
			cut_region(),
		 	mar(MaterialOptions::NONE),
		 	matml(MeshOptions::NONE),
		 	figures() {

			for(Integer i = dirs_beg; i <= dirs_end; i++) {
				observer[i] = 0.0;
			}
		}
	};

	//	MAIN
	//	GENERAL INFORMATION
	TwoDimensions two_dimensions;
	TimeSteps timesteps;
	//	SPATIAL INFORMATION
	SpaceSteps spacesteps[dirs_size];
	Background background;
	Boundary boundaries[dirs_size][pos_size];
	//	SOURCES
	vector<PlaneWaveSource> plane_wave_source;
	vector<BoxInSource> box_in_sources;
	vector<GammaSource> gamma_sources;
	vector<CurrentDensitySource> current_density_sources;
	vector<FieldSource> field_sources;
	//	MATERIALS
	vector<IsotropicBody> isotropic_body;
	vector<IsotropicSurf> isotropic_surf;
	vector<IsotropicLine> isotropic_lines;
	vector<AnisotropicBody> anisotropic_bodies;
	vector<AnisotropicSurf> anisotropic_surfs;
	vector<AnisotropicLine> anisotropic_lines;
	vector<DispersiveBody> dispersive_body;
	vector<DispersiveSurf> dispersive_surf;
	vector<DispersiveLine> dispersive_lines;
	vector<CompositeSurf> composite_surf;
	//	THIN MATERIALS
	vector<ThinWire> thin_wires;
	vector<ThinGap> thin_gaps;
	//	PROBES
	vector<TraditionalProbe> traditional_probe;
	vector<ScreenProbe> screen_probes;
	vector<NewProbe> new_probe;
	vector<BulkProbe> bulk_probe;
	vector<StructureProbe> structure_probes;
	vector<FineStructureProbe> fine_structure_probes;
	vector<PictureProbe> picture_probe;
	vector<SliceProbe> slice_probe;

	NFDEData()
	:	two_dimensions(),
	 	timesteps(),
	 	background(),
	 	plane_wave_source(),
	 	box_in_sources(),
	 	gamma_sources(),
	 	current_density_sources(),
	 	field_sources(),
	 	isotropic_body(),
	 	isotropic_surf(),
	 	isotropic_lines(),
	 	anisotropic_bodies(),
	 	anisotropic_surfs(),
	 	anisotropic_lines(),
	 	dispersive_body(),
	 	dispersive_surf(),
	 	dispersive_lines(),
	 	composite_surf(),
	 	thin_wires(),
	 	thin_gaps(),
	 	traditional_probe(),
	 	screen_probes(),
	 	new_probe(),
	 	bulk_probe(),
	 	structure_probes(),
	 	fine_structure_probes(),
	 	picture_probe(),
	 	slice_probe() {
	}
};

}

#endif /* NFDEDATA_H_ */
