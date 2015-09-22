/*
 * PMWireExtremes.h
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_
#define SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_

#include "PMWire.h"
#include "PMMultiport.h"

class PMWireExtremes: public PMWire {
public:
    PMWireExtremes(const PMWire& wire,
                   const PMMultiport* extremeL,
                   const PMMultiport* extremeR);
    PMWireExtremes(const PMWireExtremes& rhs);
    virtual ~PMWireExtremes();

    DEFINE_CLONE(PMWireExtremes);

    const PMMultiport *getExtreme(const UInt i) const { return extreme_[i]; }

    void setExtreme(const UInt i, const PMMultiport* extreme);

    void printInfo() const;

private:
    const PMMultiport* extreme_[2];
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_ */
