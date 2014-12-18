#include <limits>
#include <utility>
#include "../math/CartesianVector.h"

using namespace std;

#ifndef _limitBox_hxx
# define _limitBox_hxx

class BoundingBox {
public:

	//.....................................................................
	BoundingBox()  {};
	BoundingBox(const pair<CVecD3,CVecD3>& boundsMinMax);
	BoundingBox(const CVecD3& min, const CVecD3& max);
	void erase (void){setDefaultValues();}
	~BoundingBox() {};
	//.....................................................................
	void set(const pair<CVecD3,CVecD3>& boundsMinMax);
	void scale(const double factor);
	//.....................................................................
	bool operator >    (const BoundingBox &lBoxMin)const;
	bool operator <    (const BoundingBox &lBoxMax)const;
	bool operator <=   (const BoundingBox &lBoxMax)const;
	bool operator >=   (const BoundingBox &lBoxMin)const;
	bool operator ==   (const BoundingBox &lBoxMin)const;
	bool isIntersected (const BoundingBox &lBox)   const;
	bool isInnerPoint(const CVecD3& point) const;
	//.....................................................................
	void operator = (const BoundingBox &lBoxMin);
	void operator += (const BoundingBox &lBoxSource);
	void operator + (const BoundingBox &lBoxSource);
	//.....................................................................
	void operator << (const CVecD3 &p);
	//.....................................................................
	inline CVecD3 getMin(void) const  {return minMax_.first;}
	inline CVecD3 getMax(void) const  {return minMax_.second;}
	inline CVecD3 getLength() const {
		return (minMax_.second - minMax_.first).abs();
	 }

	void printInfo() const;
private:
	pair<CVecD3, CVecD3> minMax_;
	void setDefaultValues(void){
		minMax_.first.setPlusInfty();
		minMax_.second.setMinusInfty();
	};

};

#endif
