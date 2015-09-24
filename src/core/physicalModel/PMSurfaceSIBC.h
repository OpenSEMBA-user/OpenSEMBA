// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * PMSurface.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMSURFACESIBC_H_
#define PMSURFACESIBC_H_

#include "math/StaMatrix.h"

#include "PMSurface.h"

class PMSurfaceSIBC : public PMSurface {
	friend class DGSIBC;
public:
	PMSurfaceSIBC();
	PMSurfaceSIBC(
	 const MatId id,
	 const string& name,
	 const MatR22& Zinfinite,
	 const MatR22& Zstatic,
	 const vector<Real>& pole,
	 const vector<MatR22>& Z);
	PMSurfaceSIBC(
	     const MatId id,
	     const string& name,
	     const ProjectFile& file);
	virtual ~PMSurfaceSIBC();

	DEFINE_CLONE(PMSurfaceSIBC);

	virtual PMSurfaceSIBC& operator=(const PMSurfaceSIBC &param);
	void setZInfinity(const MatR22& Zinf);
	void setZStatic(const MatR22& Zsta);
	virtual void addPole(const Real pole, const MatR22& Z);
    virtual UInt getNumberOfPoles() const;
    virtual void printInfo() const;
    const ProjectFile getFile() const;

protected:
	MatR22 ZInfinity_, ZStatic_;
	vector<Real> pole_; // Impedance poles.
	vector<MatR22> Z_; // Impedance residuals.
	ProjectFile file_;
};

#endif /* PMSURFACE_H_ */
