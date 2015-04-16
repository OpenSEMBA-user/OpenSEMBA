/*
 * Ordering.h
 *
 *  Created on: Jun 12, 2013
 *      Author: luis
 */

#ifndef ORDERING_H_
#define ORDERING_H_

#include "stdlib.h"
#include <assert.h>
#include <iostream>
#include <vector>
#include "../../../common/math/DynMatrix.h"

using namespace std;
typedef unsigned int uint;

class Ordering {
public:
	uint
	 getGlobalSize() const;
	uint
	 getIdOfGlobalRelPos(const uint rp) const;
	uint
	 getGlobalRelPosOfId(const uint id) const;
	uint
	 getIdOfRelPos(const uint rp) const;
	uint
	 getRelPosOfId(const uint id) const;
	uint
	 getLocalSize() const;
	bool
	 checkRelPosOfId() const;
	bool
	 isLocalId(const uint id) const;
protected:
	Ordering();
	virtual ~Ordering();
	void
	 setGlobalSize(const uint globalSize_);
	void
	 setLocalSizeAndOffset(
	  const uint localSize,
	  const uint localOffset);
	void
	 buildRelPosOfIds(
	  const DynMatrix<uint>& list);
	void
	 printOrderingInfo() const;
private:
	static uint globalSize;
	static uint localSize;
	static uint localOffset;
	static uint offsetId;
	static uint* idOfRelPos;
	static uint* relPosOfId;
	bool
	 checkLocalIds(
	  const vector<vector<uint> >& partIds,
	  const uint task);
};

#endif /* ORDERING_H_ */
