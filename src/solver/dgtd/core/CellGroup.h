/*
 * CellGroup.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#ifndef CELLGROUP_H_
#define CELLGROUP_H_

#include "CellTet.h"
#include "Ordering.h"

class CellGroup : public Ordering {
public:
	vector<CellTet<ORDER_N>*> cell;
	vector<CellTet4<ORDER_N> > linTet;
	vector<CellTet10<ORDER_N> > quadTet;
	CellGroup(const SmbData* smb);
	~CellGroup();
	const CellTet<ORDER_N>* operator()(const UInt i) const;
	const CellTet<ORDER_N>* getPtrToCell(const Tet* elem) const;
	const CellTet<ORDER_N>* getPtrToCellWithId(UInt) const;
private:
	UInt cellOffsetId;
	void buildNodalMaps(const MapGroup& map);
	void check(const MapGroup& map) const;
	void checkIdsAreConsecutive() const;
	void checkNodalMaps(const MapGroup& map) const;
};

#endif /* CELLGROUP_H_ */
