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
 * OptionsSolverFDTD.h
 *
 *  Created on: Jun 24, 2015
 *      Author: luis
 */

#ifndef SRC_SOLVER_FDTD_OPTIONSSOLVERFDTD_H_
#define SRC_SOLVER_FDTD_OPTIONSSOLVERFDTD_H_

using namespace std;

#include "options/OptionsSolver.h"

class OptionsSolverFDTD: public OptionsSolver {
public:
    enum class CompositeModel {
        mibc, digFilt, ade, none
    };
    enum class WireModel {
        newWireModel, transitionWireModel, oldWireModel
    };
    enum class InductanceModel {
        boutayeb, ledfelt, berenger
    };
    enum class PMLBacking {
        none, mur1, mur2
    };
    enum class NoNF2FF {
        none, back, front, left, right, down, up
    };

    OptionsSolverFDTD();
    OptionsSolverFDTD(const OptionsSolver& base);
    virtual ~OptionsSolverFDTD();

    DEFINE_CLONE(OptionsSolverFDTD);

    void set(const Arguments& args);

    void setCompositeModel(CompositeModel compositeModel);
    void setCompositesAttenuationFactor(Real compositesAttenuationFactor);
    void setGroundWires(bool groundWires);
    void setMakeHoles(bool makeHoles);
    void setMTLN(bool mtln);
    void setPMLAlpha(const pair<Real,Real>& pmlAlpha);
    void setPMLCorrection(const pair<Real,Real>& pmlCorrection);
    void setPMLKappa(Real pmlKappa);
    void setInductanceModel(InductanceModel inductanceModel);
    void setTaparrabos(bool taparrabos);
    void setWireModel(WireModel wireModel);
    void setWiresAttenuationFactor(Real wiresAttenuationFactor);
    void setConformalSkin(bool conformalSkin);
    void setMap(bool map);
    void setMapVtk(bool mapVtk);
    void setMinDistanceWires(Real minDistanceWires);
    void setNewDispersiveFormulation(bool newDispersiveFormulation);
    void setNF2FFDecimation(bool f2FfDecimation);
    void setNoCompoMur(bool noCompoMur);
    void setNoNF2FF(NoNF2FF noNf2Ff);
    void setSkinDepth(bool skinDepth);
    void setUseDefaultPml(bool useDefaultPml);
    void setPMLBacking(PMLBacking pmlBacking);

    CompositeModel getCompositeModel() const;
    Real getCompositesAttenuationFactor() const;
    bool isGroundWires() const;
    bool isJoinWires() const;
    bool isMakeHoles() const;
    bool isMTLN() const;
    const pair<Real,Real>& getPmlAlpha() const;
    const pair<Real,Real>& getPmlCorrection() const;
    Real getPmlKappa() const;
    InductanceModel getInductanceModel() const;
    bool isTaparrabos() const;
    WireModel getWireModel() const;
    Real getWiresAttenuationFactor() const;
    bool isConformalSkin() const;
    bool isMap() const;
    bool isMapVtk() const;
    Real getMinDistanceWires() const;
    bool isNewDispersiveFormulation() const;
    bool isNF2FFDecimation() const;
    bool isNoCompoMur() const;
    NoNF2FF getNoNF2FF() const;
    bool isSkinDepth() const;
    bool isUseDefaultPml() const;
    PMLBacking getPMLBacking() const;

    string toArgsStr() const;
private:
    CompositeModel compositeModel_;
    bool conformalSkin_;
    bool noCompoMur_;
    bool skinDepth_;
    Real compositesAttenuationFactor_;

    WireModel wireModel_;
    bool MTLN_;
    Real minDistanceWires_;
    bool newDispersiveFormulation_;
    bool taparrabos_;
    bool makeHoles_;
    bool groundWires_;
    InductanceModel inductanceModel_;
    Real wiresAttenuationFactor_;

    bool useDefaultPML_;
    pair<Real,Real> pmlAlpha_;
    Real pmlKappa_;
    pair<Real,Real> pmlCorrection_;
    PMLBacking pmlBacking_;

    bool map_;
    bool mapVTK_;
    NoNF2FF noNF2FF_;
    bool nF2FFDecimation_;

    static string toStrIfTrue(const string str, const bool param);
    void initDefaults_();
};

#endif /* SRC_SOLVER_FDTD_OPTIONSSOLVERFDTD_H_ */
