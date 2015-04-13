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

#include "base/class/ClassBase.h"

class SmbData : public virtual ProjectFile,
                public virtual ClassBase {
public:
    MesherOptions*        mesherOptions;
    Grid3*                grid;
    Mesh*                 mesh;

    SolverOptions*        solverOptions;
    PhysicalModelGroup<>* pMGroup;
    EMSourceGroup<>*      emSources;
    OutRqGroup<>*         outputRequests;

    SmbData();
    SmbData(const SmbData& rhs);
    virtual ~SmbData();

    DEFINE_CLONE(SmbData);

    SmbData& operator=(const SmbData& rhs);

    void applyGeometricScalingFactor();

    void printInfo() const;
};

#endif /* SMBDATA_H_ */
