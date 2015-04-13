/*
 * Nodal.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_

#include "EMSource.h"

class SourceOnLine : public EMSource<Lin> {
public:
    typedef enum {
        electric = 1,
        magnetic = 2
    } Type;
    typedef enum {
        hard = 1,
        soft = 2
    } Hardness;
    SourceOnLine(const Magnitude* magnitude,
                 const ElementsGroup<Lin>& elem,
                 const Type& sourceType,
                 const Hardness& sourceHardness);
    SourceOnLine(const SourceOnLine& rhs);
    virtual ~SourceOnLine();

    DEFINE_CLONE(SourceOnLine);

    const string& getName() const;
    Type getType() const;
    Hardness getHardness() const;

    virtual void printInfo() const;
private:
    Type type_;
    Hardness hardness_;

    string getTypeStr() const;
    string getHardnessStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_ */
