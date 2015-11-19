/*
 * Output.h
 *
 *  Created on: Nov 18, 2015
 *      Author: luis
 */

#ifndef SRC_CORE_EXPORTER_OUTPUT_H_
#define SRC_CORE_EXPORTER_OUTPUT_H_

#include "base/class/ClassBase.h"
#include "outRq/OutRq.h"

class OutputBase : public ClassBase {
public:
    OutputBase(const OutRqBase* outRq);
    virtual ~OutputBase();
protected:
    const OutRqBase* outRq_;
};

template<class T>
class Output : public OutputBase {
public:
        Output( const OutRqBase* outRq,
            multimap<ElementId, vector<CVecR3>>& electric,
            multimap<ElementId, vector<CVecR3>>& magnetic);
    virtual ~Output();


private:
    multimap<ElementId, vector<T>> data_;
    //!< Stores several values for a single element which is needed when data
    //! is multivaluated (e.g. Discontinuous Galerkin). vector<T> must contain
    //!  data at the Gauss points of the element. e.g:
    //!      Cell Type    |   vector size
    //!      ------------ |   --------------
    //!      Node         |        1
    //!      Lin (0)      |        1
    //!      Lin (1)      |        2
    //!      Tet (1)      |        4
    //!      Tet (2)      |       10
};

#endif /* SRC_CORE_EXPORTER_OUTPUT_H_ */
