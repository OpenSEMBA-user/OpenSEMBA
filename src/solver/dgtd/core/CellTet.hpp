#include "CellTet.h"

template <int TET_N>
CellTet<TET_N>::CellTet() {
    base = NULL;
}

template <int TET_N>
CellTet<TET_N>::~CellTet() {

}

template <int TET_N>
void
CellTet<TET_N>::init(
        const Tet* base_,
        const GroupPhysicalModels<>* pMGroup) {
    base = base_;
    if (pMGroup != NULL) {
        material = pMGroup->getPMVolumeWithId(base->getMatId());
    } else {
        material = NULL;
    }
    buildNodes();
}

template <int TET_N>
void
CellTet<TET_N>::getCMatrices(StaMatrix<Real,np,np> C[3]) const {
    StaMatrix<Real,4,4> cJ[SimplexTet<1>::ncp];
    base->getCubatureJacobian(cJ);
    Real cJDet[SimplexTet<1>::ncp];
    base->getCubatureJacobianDeterminant(cJDet, cJ);
    StaMatrix<Real,np,np> invM;
    invM  = getMassMatrix(cJDet).invert();
    StaMatrix<Real,4,3> cJHat[SimplexTet<1>::ncp];
    base->getCubatureJacobianHat(cJHat, cJ, cJDet);
    for (uint x = 0; x < 3; x++) {
        C[x] = getCMatrix(x, invM, cJHat, cJ);
    }
}

template <int TET_N>
StaMatrix<Real,TET_NP,TET_NP>
CellTet<TET_N>::getConductivityWithGeometricProfile(
        const PMVolumePML& mat,
        const uint type,
        const Real maxSigma) const {
    static const uint ncp = SimplexTet<TET_N>::ncp;
    StaMatrix<Real,np,np> res;
    CVecR3 cPos[ncp];
    base->getCubaturePositions(cPos);
    int firstOrientation = mat.getFirstOrientationIndex();
    int i = ((type % 10) - 1 + firstOrientation);
    int j = ((type / 10) - 1 + firstOrientation);
    const CVecR3 posBorder = mat.getPMLBeginningPosition();
    const CVecR3 width = mat.getWidth();
    // Computes sigma(x) = [(x - x_0)/width]^2 * maxSigma.
    Real sigma[ncp];
    i = i % 3;
    for (uint c = 0; c < ncp; c++) {
        sigma[c] = maxSigma;
        sigma[c] *= pow((cPos[c](i) - posBorder(i))/width(i), 2);
    }
    // Computes second sigma if necessary.
    if (j - firstOrientation >= 0) {
        j = j % 3;
        for (uint c = 0; c < ncp; c++) {
            sigma[c] *= maxSigma;
            sigma[c] *= pow((cPos[c](j) - posBorder(j))/width(j), 2);
        }
    }
    // Computes Mass Matrix.
    res = getMassMatrixIntegratedWithScalar(sigma);
    StaMatrix<Real,np,np> invMM = getMassMatrix().invert();
    return (invMM * res);
}

template <int TET_N>
void
CellTet<TET_N>::getCurvedLIFTnormal(
        StaMatrix<Real,np,nfp> LIFTn[3],
        StaMatrix<Real,np,nfp> LIFTcn[3],
        StaMatrix<Real,np,nfp> LIFTrn[3],
        const uint face) const {
    cerr << endl << "ERROR @ CellTet" << endl;
    assert(false);
    exit(-1);
}

template <int TET_N>
uint
CellTet<TET_N>::getNbfp() const {
    return base->numberOfSideCoordinates();
}

template <int TET_N>
bool
CellTet<TET_N>::isCurved() const {
    return base->isCurved();
}

template <int TET_N>
bool
CellTet<TET_N>::isCurvedFace(const uint f) const {
    return base->isCurvedFace(f);
}

template <int TET_N>
Real
CellTet<TET_N>::getVolume() const {
    return base->getVolume();
}

template <int TET_N>
const Tet*
CellTet<TET_N>::getPtrToBase() const {
    return base;
}

template <int TET_N>
StaMatrix<Real,TET_NP,TET_NP>
CellTet<TET_N>::getCMatrix(
        const uint x,
        const StaMatrix<Real,np,np>& invM,
        const StaMatrix<Real,4,3> cJHat[SimplexTet<1>::ncp],
        const StaMatrix<Real,4,4> cJ[SimplexTet<1>::ncp]) const {
    StaMatrix<Real,TET_NP,TET_NP> res;
    // Computes preliminary C matrices.
    for (uint i = 0; i < this->tet.np; i++) {
        for (uint j = 0; j < this->tet.np; j++) {
            for (uint k = 0; k < faces; k++) {
                for (uint c = 0; c < SimplexTet<1>::ncp; c++) {
                    res(i,j) += this->tet.cwada[c][k](i,j) * cJHat[c](k,x);
                }
            }
        }
    }
    // Multiplies by invMM.
    res = invM * res;
    res *= Real(1.0 / 6.0);
    return res;
}

template <int TET_N>
StaMatrix<Real,TET_NP,TET_NP>
CellTet<TET_N>::getMassMatrix(
        const Real cJDet[SimplexTet<1>::ncp]) const{
    StaMatrix<Real,TET_NP,TET_NP> res;
    for (uint c = 0; c < SimplexTet<1>::ncp; c++) {
        res += tet.cwaa[c] * cJDet[c];
    }
    res *= Real(1.0 / 6.0);
    return res;
}

template <int TET_N>
StaMatrix<Real,TET_NP,TET_NP>
CellTet<TET_N>::getMassMatrix() const {
    static const uint ncp = SimplexTet<1>::ncp;
    StaMatrix<Real,4,4> cJ[SimplexTet<1>::ncp];
    base->getCubatureJacobian(cJ);
    Real cJDet[ncp];
    base->getCubatureJacobianDeterminant(cJDet, cJ);
    return getMassMatrix(cJDet);
}

template <int TET_N>
StaMatrix<Real,TET_NP,TET_NP>
CellTet<TET_N>::getMassMatrixIntegratedWithScalar(
        const Real cScalar[SimplexTet<1>::ncp]) const {
    static const uint ncp = SimplexTet<1>::ncp;
    StaMatrix<Real,4,4> cJ[SimplexTet<1>::ncp];
    base->getCubatureJacobian(cJ);
    Real cJDetByScalar[ncp];
    base->getCubatureJacobianDeterminant(cJDetByScalar, cJ);
    for (uint c = 0; c < ncp; c++) {
        cJDetByScalar[c] *= cScalar[c];
    }
    return getMassMatrix(cJDetByScalar);
}

template <int TET_N>
Real CellTet<TET_N>::getAreaOfFace(uint face) const {
    return base->getAreaOfFace(face);
}

template <int TET_N>
uint CellTet<TET_N>::getNodeVertex(const uint i) const {
    return tet.vertex(i);
}

template <int TET_N>
bool CellTet<TET_N>::isLocalSide(
        const uint face,
        const SurfR* surf) const {
    return (base->isLocalFace(face, *surf));
}

template <int TET_N>
bool
CellTet<TET_N>::isLocalSide(
        const SurfR* surf) const {
    for (uint f = 0; f < faces; f++) {
        if (isLocalSide(f, surf)) {
            return true;
        }
    }
    return false;
}

template <int TET_N>
CVecR3
CellTet<TET_N>::getSideNodePos(
        const uint f, const uint i) const {
    return n[tet.sideNode(f, i)];
}

template <int TET_N>
CVecR3
CellTet<TET_N>::getSideNormal(
        const uint f) const {
    return base->sideNormal(f);
}

template <int TET_N>
uint
CellTet<TET_N>::getSideNode(
        const uint f, const uint i) const {
    return tet.sideNode(f, i);
}

template <int TET_N>
const CoordR3* CellTet<TET_N>::getSideBaseNode(
        const uint f, const uint i) const {
    return base->getSideV(f,i);
}

template <int TET_N>
const CoordR3* CellTet<TET_N>::getSideVertexBaseNode(uint f, uint i) const {
    return (base->getSideVertex(f, i));
}

template <int TET_N>
void CellTet<TET_N>::buildNodes() {
    // Evaluates Lagrange's functions in positions specified by the
    // simplex coordinates of tet.
    Real lagrEv[tet.np][base->numberOfCoordinates()];
    for (uint j = 0; j < tet.np; j++) {
        for (uint i = 0; i < base->numberOfCoordinates(); i++) {
            lagrEv[j][i]= base->getTet().getLagr(i).eval(tet.coordinate(j));
        }
    }
    // Computes nodes.
    for (uint j = 0; j < tet.np; j++) {
        for (uint i = 0; i < base->numberOfCoordinates(); i++) {
            this->n[j] += *(base->getV(i)) * lagrEv[j][i];
        }
    }
}

template <int TET_N>
void
CellTet<TET_N>::printInfo() const {
    cout << " --- CellTet information --- " << endl;
    this->base->printInfo();
    cout << "Nodes: " << this->np << endl;
    for (uint i = 0; i < this->np; i++) {
        this->n[i].printInfo();
        cout << endl;
    }
    cout << "Side Nodes and Normal vectors:" << endl;
    for (uint f = 0; f < base->numberOfFaces(); f++) {
        cout << "Face " << f << ": " << endl;
        for (uint i = 0; i < this->nfp; i++) {
            cout << "Pos: ";
            this->getSideNodePos(f,i).printInfo();
            cout << "Normal: ";
            this->getSideNormal(f).printInfo();
            cout << endl;
        }
    }
    for (uint f = 0; f < base->numberOfFaces(); f++) {
        cout << "Area of face " << f << ": "
                << this->base->getAreaOfFace(f) << endl;
    }
    cout << " Volume: " << this->base->getVolume() << endl;
    cout << " Neighbour nodes: (vmapP)" << endl;
    for (uint f = 0; f < this->faces; f++) {
        cout << " Face #" << f << ": ";
        for (uint i = 0; i < this->nfp; i++) {
            cout << this->vmapP[f][i] << " ";
        }
        cout << endl;
    }
    cout << " Vertex info: " << endl;
    for (uint i = 0; i < this->vertices; i++) {
        this->getV(i)->printInfo();
        cout << endl;
    }
    cout << " Side Vertex Info: " << endl;
    for (uint f = 0; f < this->faces; f++) {
        cout << "Face #" << f << endl;
        for (uint i = 0; i < this->nfp; i++) {
            this->getSideVertexBaseNode(f,i)->printInfo();
            cout << endl;
        }
    }
    cout << " --- End of CellTet4 information --- " << endl;
}

template <int TET_N>
CellTet4<TET_N>::CellTet4() {

}

template <int TET_N>
CellTet4<TET_N>::~CellTet4() {

}

template <int TET_N>
CellTet4<TET_N>::CellTet4(
        const Tet* base_,
        const PMGroup& pMGroup) {
    this->init(base_, &pMGroup);
}

template <int TET_N>
CellTet10<TET_N>::CellTet10() {

}

template <int TET_N>
CellTet10<TET_N>::~CellTet10() {

}

template <int TET_N>
CellTet10<TET_N>::CellTet10(
        const Tet* base_,
        const PMGroup& pMGroup) {
    this->init(base_, &pMGroup);
}

template <int TET_N>
void CellTet10<TET_N>::getCurvedLIFTnormal(
        StaMatrix<Real,np,nfp> LIFTn[3],
        StaMatrix<Real,np,nfp> LIFTcn[3],
        StaMatrix<Real,np,nfp> LIFTrn[3],
        const uint f) const {
    // Side mass matrix.
    const Tet10* base = dynamic_cast<const Tet10*>(this->base);
    assert(base != NULL);
    Tri6 face = base->getTri6Face(f);
    static const uint ncp = SimplexTri<2>::ncp;
    Real csdf[ncp];
    face.getCubatureDifferentials(csdf);
    CVecR3 csdn[ncp], csdcn[ncp], csdrn[ncp];
    face.getCubatureNormals(csdn);
    for (uint c = 0; c < ncp; c++) {
        csdcn[c](0) = csdn[c](0) * csdn[c](1);
        csdcn[c](1) = csdn[c](1) * csdn[c](2);
        csdcn[c](2) = csdn[c](2) * csdn[c](0);
        csdrn[c] = (Real) 1.0;
        csdrn[c] -= csdn[c] * csdn[c];
    }
    StaMatrix<Real,nfp,nfp> sMMn[3], sMMcn[3], sMMrn[3];
    static const SimplexTri<TET_N> tri;
    for (uint i = 0; i < 3; i++) {
        sMMn[i].zeros();
        sMMcn[i].zeros();
        sMMrn[i].zeros();
        for (uint c = 0; c < ncp; c++) {
            sMMn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdn[c](i);
            sMMcn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdcn[c](i);
            sMMrn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdrn[c](i);
        }
    }
    // Mass matrix.
    StaMatrix<int,nfp,np> RInt;
    StaMatrix<int,np,nfp> RtrInt;
    StaMatrix<Real,np,nfp> R;
    RInt = this->tet.R[f];
    RtrInt = RInt.transpose();
    R.assign(RtrInt);
    StaMatrix<Real,np,nfp> MMR;
    MMR = this->getMassMatrix().invert() * R;
    // Final form.
    for (uint i = 0; i < 3; i++) {
        LIFTn[i] = MMR * sMMn[i];
        LIFTcn[i] = MMR * sMMcn[i];
        LIFTrn[i] = MMR * sMMrn[i];
    }
}
