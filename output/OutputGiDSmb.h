/*
 * OutputGiDSmb.h
 *
 *  Created on: Mar 2, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_
#define SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_

#include "OutputGiD.h"
#include "../SmbData.h"

class OutputGiDSmb : public OutputGiD {
public:
    OutputGiDSmb(const SmbData* smb);
    OutputGiDSmb(const SmbData* smb, const string& fn);
    virtual ~OutputGiDSmb();
private:
    const SmbData* smb_;
    void writeMesh();
    void writeMeshWithIds(
            const vector<vector<ElementId> >& ids,
            string& name);
    void writeMeshWithIds(
            const vector<vector<ElementId> >& ids,
            const vector<string>& name);
    void writeMeshWithIds(
            const vector<ElementId>& ids, string& name);
    void writeOutputRequestsMesh();
    void writeElements(
            const ElementsGroup<>& entities,
            const string& name,
            const GiD_ElementType type,
            const Int nV);
};

#endif /* SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_ */
