/*
 * PMSMA.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMSMA_H_
#define PMSMA_H_

#include "PMPredefined.h"

class PMSMA : public PMPredefined {
public:
    virtual ~PMSMA();
    PMSMA(const unsigned int id_, const string name_);
    bool isSMA() const;
    Condition::Type getConditionType() const;
    void printInfo() const;
};

#endif /* PMSMA_H_ */
