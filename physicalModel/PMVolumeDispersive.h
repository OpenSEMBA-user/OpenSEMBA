/*
 * PMVolumeDispersive.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUMEDISPERSIVE_H_
#define PMVOLUMEDISPERSIVE_H_

#include "base/error/Error.h"

#include "PMVolume.h"

typedef pair<complex<Real>,complex<Real>> PoleResidue;

class PMVolumeDispersive : public PMVolume {
public:
    class ErrorMagneticMaterial : public Error {
    public:
        ErrorMagneticMaterial();
        virtual ~ErrorMagneticMaterial() throw();
    };

    PMVolumeDispersive();
    virtual ~PMVolumeDispersive();
    PMVolumeDispersive(
            const MatId id,
            const string& name,
            const Real rEps,
            const Real rMu,
            const Real elecCond,
            const Real magnCond);
    PMVolumeDispersive(
            const MatId id,
            const string& name,
            const Real rEps,
            const Real rMu,
            const Real elecCond,
            const Real magnCond,
            const vector<PoleResidue>& poleResidue);
    PMVolumeDispersive(
            const MatId id,
            const string& name,
            const ProjectFile& file);

    DEFINE_CLONE(PMVolumeDispersive);

    UInt getPoleNumber() const;
    complex<Real> getPole(UInt p) const;
    complex<Real> getResidue(UInt p) const;
    virtual Real getElectricConductivity() const;

    bool isClassic() const;
    bool isSimplyConductive() const;
    bool isDispersive() const;

    void printInfo() const;
    const ProjectFile getFile() const;

protected:
    Real rEpsInfty_, rMuInfty_; // @ InftyFreq.
    vector<PoleResidue> poleResidue_; // Residues for dispers model. c_p.
    ProjectFile file_;
    void addPole(
            const complex<Real>& pole_,
            const complex<Real>& res_);
    void addDrudePole(
            const complex<Real>& pole_,
            const complex<Real>& res_);
};

#endif /* PMVOLUMEDISPERSIVE_H_ */
