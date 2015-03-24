/*
 * Generator.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_

#include "EMSource.h"

class Generator : public EMSource<Nod> {
public:
    typedef enum {
        voltage = 1,
        current = 2
    } Type;
    typedef enum {
        soft,
        hard
    } Hardness;

    Generator();
    Generator(const Magnitude* magnitude,
              const ElementsGroup<Node<> >& elem,
              const Type& generatorType,
              const Hardness& hardness);
    Generator(const Generator& rhs);
    virtual ~Generator();

    virtual ClassBase* clone() const;

    Type getType() const;
    string getName() const;

    void printInfo() const;

private:
    Type type_;
    Hardness hardness_;

    string getTypeStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_ */
