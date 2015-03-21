/*
 * SmbData.h
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#ifndef SMBDATA_H_
#define SMBDATA_H_

#include "MeshUnstructured.h"
#include "MeshStructured.h"
#include "Grid.h"
#include "inputs/electromagneticSources/EMSourceGroup.h"
#include "inputs/physicalModel/PhysicalModelGroup.h"
#include "inputs/MesherOptions.h"
#include "inputs/outRq/OutRqGroup.h"
#include "inputs/SolverOptions.h"

#include "ProjectFile.h"

class SmbData : public virtual ProjectFile {
public:
    SolverOptions*        solverOptions;
    MesherOptions*        mesherOptions;
    PhysicalModelGroup<>* pMGroup;
    EMSourceGroup<>*      emSources;
    OutRqGroup<>*         outputRequests;
    Mesh*                 mesh;
    Grid3*                grid;

    SmbData();
    SmbData(const SmbData& rhs);
    virtual ~SmbData();

    ClassBase* clone() const;

    SmbData& operator=(const SmbData& rhs);

    void applyGeometricScalingFactor();

    void printInfo() const;
};

#endif /* SMBDATA_H_ */
