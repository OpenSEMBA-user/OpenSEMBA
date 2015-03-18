/*
 * Nodal.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_

#include "EMSource.h"

class SourceOnLine : public EMSource, public ElementsGroup<Line<> > {
public:
    typedef enum {
        electric = 1,
        magnetic = 2
    } Type;
    typedef enum {
        hard = 1,
        soft = 2
    } Hardness;
    SourceOnLine(
            const ElementsGroup<Line<> >& elem,
            const Magnitude* magnitude,
            const Type& sourceType,
            const Hardness& sourceHardness);
    virtual ~SourceOnLine();

    virtual ClassBase* clone() const;

    Type getType() const;
    Hardness getHardness() const;
    string getTypeStr() const;

    virtual void printInfo() const;
private:
    Type type_;
    Hardness hardness_;
    string hardnessToStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_ */
