/*
 * Result.h
 *
 *  Created on: Jan 30, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_EXPORTER_RESULT_H_
#define SRC_COMMON_EXPORTER_RESULT_H_

#include "math/Field.h"

class Result {
public:
   Result();
   virtual ~Result();
   virtual void write(
         const Real time,
         const FieldR3& electric,
         const FieldR3& magnetic) const = 0;
};

#endif /* SRC_COMMON_EXPORTER_RESULT_H_ */
