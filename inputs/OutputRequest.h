#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "../geometry/elements/Element.h"
#include "Condition.h"
#include "Domain.h"

using namespace std;

class OutputRequest : public Condition, public Domain {
public:
	typedef enum {
		undefined,
		electricField,
		magneticField,
		electricFieldNormals,
		magneticFieldNormals,
		// powerDensity is the Poynting vector mult. by the surface normal.
		powerDensity,
		planeWaveDecomposition,
		current,
		voltage,
		bulkCurrentElectric,
		bulkCurrentMagnetic,
		// power is the result of powerDensity integrated over an Area.
		// The result of power is given in a graph.
		power,
		sParameter,
		zParameter,
		yParameter,
		theveninVoltageGenerator,
		nortonCurrentGenerator,
		couplingCrossSection,
		poyntingVector,
		energy,
		farField
	} Type;
	OutputRequest();
	OutputRequest(
	 const Domain& domain,
	 const Element::Type elementType,
	 const OutputRequest::Type outputType,
	 const string& name,
	 const vector<unsigned int>& elements);
	OutputRequest(
	 const Domain& domain,
	 const Element::Type elementType,
	 const OutputRequest::Type outputType_,
	 const string& name_,
	 const BoxD3& box);
	OutputRequest(
	 const Domain& domain,
	 const Element::Type elementType,
	 const OutputRequest::Type outputType_,
	 const string& name_,
	 const BoxD3& box,
	 const double iTh, const double fTh, const double sTh,
	 const double iPhi, const double fPhi, const double sPhi);
	OutputRequest(
	 const Element::Type elementType,
	 const OutputRequest::Type outputType,
	 const string& name,
	 const vector<unsigned int>& elements);
	OutputRequest&
	 operator=(const OutputRequest& rhs);
	bool
	 isSimilar(const OutputRequest& rhs) const;
	string
	 outputTypeStr() const;
	string
	 elementTypeStr() const;
	unsigned int
	 count() const {return elem_.size();}

	void
	 printInfo() const;

	const string& getName() const {
		return name_;
	}

	OutputRequest::Type getOutputType() const {
		return outputType_;
	}

	const vector<unsigned int>& getElem() const {
		return elem_;
	}

	double getStepPhi() const {
		assert(outputType_ == OutputRequest::farField);
		return stepPhi_;
	}

	double getStepTheta() const {
		assert(outputType_ == OutputRequest::farField);
		return stepTheta_;
	}

	double getFinalPhi() const {
		assert(outputType_ == OutputRequest::farField);
		return finalPhi_;
	}

	double getFinalTheta() const {
		assert(outputType_ == OutputRequest::farField);
		return finalTheta_;
	}

	double getInitialPhi() const {
		assert(outputType_ == OutputRequest::farField);
		return initialPhi_;
	}

	double getInitialTheta() const {
		assert(outputType_ == OutputRequest::farField);
		return initialTheta_;
	}

	const BoxD3& getBound() const {
		assert(usingBound_);
		return bound_;
	}

	void
	setAdditionalElems(const vector<uint> elems);
private:
	string name_;
	OutputRequest::Type outputType_;
	double initialTheta_, finalTheta_, stepTheta_;
	double initialPhi_, finalPhi_, stepPhi_;
	bool usingBound_;
	BoxD3 bound_;
	vector<uint> elem_;
	void
	 setThetaAndPhi(
	  double iTh, double fTh, double sTh,
	  double iPhi, double fPhi, double sPhi);
};

#	endif
