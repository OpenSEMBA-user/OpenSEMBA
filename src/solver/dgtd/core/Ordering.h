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

class Ordering {
public:
	UInt
	 getGlobalSize() const;
	UInt
	 getIdOfGlobalRelPos(const UInt rp) const;
	UInt
	 getGlobalRelPosOfId(const UInt id) const;
	UInt
	 getIdOfRelPos(const UInt rp) const;
	UInt
	 getRelPosOfId(const UInt id) const;
	UInt
	 getLocalSize() const;
	bool
	 checkRelPosOfId() const;
	bool
	 isLocalId(const UInt id) const;
protected:
	Ordering();
	virtual ~Ordering();
	void
	 setGlobalSize(const UInt globalSize_);
	void
	 setLocalSizeAndOffset(
	  const UInt localSize,
	  const UInt localOffset);
	void
	 buildRelPosOfIds(
	  const DynMatrix<UInt>& list);
	void
	 printOrderingInfo() const;
private:
	static UInt globalSize;
	static UInt localSize;
	static UInt localOffset;
	static UInt offsetId;
	static UInt* idOfRelPos;
	static UInt* relPosOfId;
	bool
	 checkLocalIds(
	  const vector<vector<UInt> >& partIds,
	  const UInt task);
};

#endif /* ORDERING_H_ */
