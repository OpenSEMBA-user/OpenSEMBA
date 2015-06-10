/*
 * SmbData.h
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#ifndef SMBDATA_H_
#define SMBDATA_H_

#include "electromagneticSources/GroupEMSources.h"
#include "geometry/MeshUnstructured.h"
#include "geometry/MeshStructured.h"
#include "geometry/Grid.h"
#include "physicalModel/GroupPhysicalModels.h"
#include "options/OptionsMesher.h"
#include "options/OptionsSolver.h"
#include "outRq/GroupOutRqs.h"

#include "ProjectFile.h"

#include "base/class/ClassBase.h"

class SmbData : public virtual ProjectFile,
                public virtual ClassBase {
public:
    Grid3*                 grid;
    Mesh*                  mesh;

    OptionsMesher*         mesherOptions;
    OptionsSolver*         solverOptions;

    GroupPhysicalModels<>* pMGroup;

    GroupEMSources<>*      emSources;
    GroupOutRqs<>*         outputRequests;

    SmbData();
    SmbData(const SmbData& rhs);
    virtual ~SmbData();

    DEFINE_CLONE(SmbData);

    SmbData& operator=(const SmbData& rhs);

    bool check() const;

    void printInfo() const;

private:
};

#endif /* SMBDATA_H_ */
