/*
 * Result.h
 *
 *  Created on: Jan 30, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_OUTPUT_RESULT_H_
#define SRC_COMMON_OUTPUT_RESULT_H_

#include "../math/Field.h"

class Result {
public:
   Result();
   virtual ~Result();
   virtual void write(
         const double time,
         const FieldD3& electric,
         const FieldD3& magnetic) const = 0;
};

#endif /* SRC_COMMON_OUTPUT_RESULT_H_ */
