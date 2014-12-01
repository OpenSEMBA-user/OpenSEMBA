//=======================================================================================//
//   Title: limitBox                                                                     //
//   Author: Miguel D. Ruiz - Cabello N.                                                 //
//=======================================================================================//
// HYSTORY & VERSION:                                                                    //
//    DATE beginning: 10.12.2013                                                         //
//=======================================================================================//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <omp.h>
#include <limits>
#include <utility>
#include "../math/CartesianVector.h"

using namespace std;
#ifndef _limitBox_hxx
# define _limitBox_hxx
# define _limitBox_hxx_version "0.0.0"       //last version tested  
# define _limitBox_hxx_date    "22.01.2014"  //last version tested

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%   class LimitBox  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
	//.....................................................................
	bool operator >    (const BoundingBox &lBoxMin)const;
	bool operator <    (const BoundingBox &lBoxMax)const;
	bool operator <=   (const BoundingBox &lBoxMax)const;
	bool operator >=   (const BoundingBox &lBoxMin)const;
	bool operator ==   (const BoundingBox &lBoxMin)const;
	bool isIntersected (const BoundingBox &lBox)   const;
	//.....................................................................
	void operator = (const BoundingBox &lBoxMin);
	void operator += (const BoundingBox &lBoxSource);
	void operator + (const BoundingBox &lBoxSource);
	//.....................................................................
	void operator << (const CVecD3 &p);
	//.....................................................................
	inline CVecD3 get_min(void) const  {return minMax_.first;}
	inline CVecD3 get_max(void) const  {return minMax_.second;}
	inline CVecD3 getLength() const {
		return (minMax_.second - minMax_.first).abs();
	 }

	void printInfo() const;
private:
	//.....................................................................
	pair<CVecD3, CVecD3> minMax_;
	void setDefaultValues(void){
		minMax_.first.setPlusInfty();
		minMax_.second.setMinusInfty();
	};
	//.....................................................................
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
