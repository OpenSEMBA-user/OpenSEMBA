/*
 * PhysicalModelGroup.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef PHYSICALMODELGROUP_H_
#define PHYSICALMODELGROUP_H_

#include <math.h>
#include <cmath>
#include <vector>
#include <assert.h>
#include "../../geometry/MeshVolume.h"
#include "PhysicalModel.h"
#include "PMPEC.h"
#include "PMPMC.h"
#include "PMSMA.h"
#include "PMVolume.h"
#include "PMVolumeDispersive.h"
#include "PMVolumePML.h"
#include "PMMultiportPredefined.h"
#include "PMMultiportRLC.h"
#include "PMSurfaceSIBC.h"
#include "PMSurfaceMultilayer.h"
#include "PMWire.h"

class PhysicalModelGroup {
public:
	typedef enum {
		vacuum = 1,
		PEC = 2,
		PMC = 3,
		SMA = 4,
		classic = 5,
		elecDispersive = 6,
		isotropicsibc = 7,
		PML = 8,
		wire = 9,
		multiport = 10,
		undefined = 0
	} Type;
	vector<PhysicalModel*> pm;
	vector<PMVolume> volume;
	vector<PMVolumeDispersive> dispersive;
	vector<PMSurface*> surface;
	vector<PMVolumePML*> pml_;
	vector<PMMultiport*> multiport_;
	vector<PMWire*> wire_;
	PMVolume* vacuumMat_;
	PMPEC pec;
	PMSMA sma;
	PMPMC pmc;
	PhysicalModelGroup();
	virtual ~PhysicalModelGroup();
	PhysicalModelGroup(
 	 const vector<const PMPredefined*>& predefined,
	 const vector<PMVolume>& volume,
	 const vector<PMVolumeDispersive>& dispersive,
	 const vector<PMVolumePML*> pml,
	 const vector<PMSurface*>& surface);
    PhysicalModelGroup(
	 const vector<const PMPredefined*>& predefined,
	 const vector<PMVolume>& volume,
	 const vector<PMVolumeDispersive>& dispersive,
	 const vector<PMVolumePML*> pml,
	 const vector<PMSurface*>& surface,
	 const vector<PMMultiport*> multiport,
	 const vector<PMWire*> wire);
	PhysicalModelGroup&
	 operator=(const PhysicalModelGroup& rhs);
	const PhysicalModel*
	 operator()(const unsigned int i) const {
		return pm[i];
	 }
	const PhysicalModel*
	 get(const unsigned int i) const {
		return pm[i];
	 }
	const PMVolume*
	 getVacuum() const;
	const PMVolumePML*
	 getPML() const;
	const PMVolumePML*
	 getPML(const unsigned int i) const;
	const PMVolumePML*
	 getAssignedPML(const unsigned int i) const;
	const PhysicalModel*
	 getPMWithId(const unsigned int id) const;
	void
	 removePMLs();
	const PMVolume*
	 getPMVolumeWithId(const unsigned int id) const;
	const PMVolumeDispersive*
	 getPMVolumeDispersiveWithId(const unsigned int id) const;
	const PMSurface*
	 getPMSurface(const unsigned int i) const;
	unsigned int
	 count() const {
		return pm.size();
	 }
	unsigned int
	 countSIBC() const {
		unsigned int res = 0;
		for (unsigned int i = 0; i < count(); i++) {
			if (pm[i]->isSurfaceImpedance()) {
				res++;
			}
		}
		return res;
	 }
	unsigned int
	 countClassic() const {
	   unsigned int res = 0;
	   for (unsigned int i = 0; i < count(); i++) {
	      if (pm[i]->isClassic()) {
	         res++;
	      }
	   }
	   return res;
	 }
	unsigned int
	 countDispersiveVolumic() const {
		unsigned int res = 0;
		for (unsigned int i = 0; i < count(); i++) {
			if (pm[i]->isDispersive()) {
				res++;
			}
		}
		return res;
	}
	unsigned int
	 countPML() const {
		unsigned int res = 0;
		for (unsigned int i = 0; i < count(); i++) {
			if (pm[i]->isPML()) {
				res++;
			}
		}
	 	return res;
	 }
	unsigned int
	 countAssignedPML() const {
		unsigned int res = 0;
		for (unsigned int i = 0; i < pml_.size(); i++) {
			if (pml_[i]->getOrientation() != PMVolumePML::undefined) {
				res++;
			}
		}
	 	return res;
	 }
	void
	 detectAndAssignPMLRegions(
	  MeshVolume* mesh);
	vector<uint>
	 getVolumicMatIds() const;
	vector<uint>
	 getSurfaceMatIds() const;
	void
	 printInfo() const;
private:
	unsigned int offsetId;
	void
	 init(
	  const vector<const PMPredefined*>& predefined,
	  const vector<PMVolume>& volume,
	  const vector<PMVolumeDispersive>& dispersive,
	  const vector<PMVolumePML*> pml,
	  const vector<PMSurface*>& surface,
	  const vector<PMMultiport*> multiport,
	  const vector<PMWire*> wire);
	void
	 updatePointers();
	void
	 createAndAssignPML(
	  const PMVolumePML::Direction direction[3],
	  const vector<pair<const Tet*, unsigned int> >& internalBorder,
	  MeshVolume* mesh);
	void
	 getDirection(
	  PMVolumePML::Direction direction[3],
	  const unsigned int i) const;
	PMVolumePML::Direction
	 getDirectionFromInt(const unsigned int i) const;
};

#endif /* PHYSICALMODELGROUP_H_ */
