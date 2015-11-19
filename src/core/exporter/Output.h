/*
 * Output.h
 *
 *  Created on: Nov 18, 2015
 *      Author: luis
 */

#ifndef SRC_CORE_EXPORTER_OUTPUT_H_
#define SRC_CORE_EXPORTER_OUTPUT_H_

#include "base/class/ClassBase.h"

class OutputBase : public ClassBase {
public:
    OutputBase(const OutRqBase* outRq);
    virtual ~OutputBase();
protected:
    const OutRqBase* outRq_;
};

template<class T>
class Output : public OutputBase {
    Output();
    virtual ~Output();
private:
    multimap<ElementId, vector<T>> data_;
};

#endif /* SRC_CORE_EXPORTER_OUTPUT_H_ */
