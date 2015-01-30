/*
 * Result.h
 *
 *  Created on: Jan 30, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_OUTPUT_RESULT_H_
#define SRC_COMMON_OUTPUT_RESULT_H_

class Result {
public:
   Result();
   virtual ~Result();
   virtual bool
    isSamplingTime(const double time) const;
};

#endif /* SRC_COMMON_OUTPUT_RESULT_H_ */
