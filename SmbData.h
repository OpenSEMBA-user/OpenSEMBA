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
#include "EMSourceGroup.h"
#include "PhysicalModelGroup.h"
#include "OutRqGroup.h"
#include "ProjectFile.h"

#include "base/class/ClassBase.h"
#include "options/OptionsMesher.h"
#include "options/OptionsSolver.h"

class SmbData : public virtual ProjectFile,
                public virtual ClassBase {
public:
    Grid3*                grid;
    Mesh*                 mesh;

    OptionsMesher*        mesherOptions;
    OptionsSolver*        solverOptions;

    PhysicalModelGroup<>* pMGroup;

    EMSourceGroup<>*      emSources;
    OutRqGroup<>*         outputRequests;

    SmbData();
    SmbData(const SmbData& rhs);
    virtual ~SmbData();

    DEFINE_CLONE(SmbData);

    SmbData& operator=(const SmbData& rhs);

    bool isSimilar(const SmbData& rhs) const;

    void applyGeometricScalingFactor();

    void printInfo() const;

private:
};

#endif /* SMBDATA_H_ */
