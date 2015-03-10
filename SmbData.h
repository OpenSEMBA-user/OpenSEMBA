/*
 * SmbData.h
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#ifndef SMBDATA_H_
#define SMBDATA_H_

#include "ProjectFile.h"
#include "Mesh.h"
#include "LayerGroup.h"
#include "inputs/electromagneticSources/EMSourceGroup.h"
#include "inputs/MesherOptions.h"
#include "inputs/physicalModel/PhysicalModelGroup.h"
#include "inputs/outputRequest/OutRqGroup.h"
#include "inputs/SolverOptions.h"

class SmbData : public ProjectFile {
public:
    SolverOptions* solverOptions;
    MesherOptions* mesherOptions;
    PhysicalModelGroup<>* pMGroup;
    EMSourceGroup<>* emSources;
    OutRqGroup<>* outputRequests;
    LayerGroup<>* layers;
    Mesh* mesh;
    SmbData();
    virtual ~SmbData();
    virtual SmbData& operator=(const SmbData& rhs);
    void applyGeometricScalingFactor();
    virtual void printInfo() const;
};

#endif /* SMBDATA_H_ */
