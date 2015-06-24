#include "CellTet.h"

template<Int N, class E>
CellTet<N,E>::CellTet(const E& base) : E(base) {
    buildNodes();
}

template<Int N, class E>
CellTet<N,E>::~CellTet() {

}

template<Int N, class E>
inline CVecR3 CellTet<N,E>::getNode(const UInt i) const {
    return node_[i];
}

template<Int N, class E>
void CellTet<N,E>::getCMatrices(StaMatrix<Real,np,np> C[3]) const {
    MatR44 cJ[SimplexTet<1>::ncp];
    this->getCubatureJacobian(cJ);
    Real cJDet[SimplexTet<1>::ncp];
    this->getCubatureJacobianDeterminant(cJDet, cJ);
    StaMatrix<Real,np,np> invM;
    invM  = getMassMatrix(cJDet).invert();
    MatR43 cJHat[SimplexTet<1>::ncp];
    this->getCubatureJacobianHat(cJHat, cJ, cJDet);
    for (UInt x = 0; x < 3; x++) {
        C[x] = getCMatrix(x, invM, cJHat, cJ);
    }
}


template<Int N, class E>
inline UInt CellTet<N,E>::getNfp() const {
    return tet->numberOfSideNodes();
}

template<Int N, class E>
CellTet<N,E>::MatNpNp CellTet<N,E>::getConductivityWithGeometricProfile(
        const PMVolumePML& mat,
        const UInt type,
        const Real maxSigma) const {
    static const UInt ncp = SimplexTet<N>::ncp;
    StaMatrix<Real,np,np> res;
    CVecR3 cPos[ncp];
    this->getCubaturePositions(cPos);
    Int firstOrientation = mat.getFirstOrientationIndex();
    Int i = ((type % 10) - 1 + firstOrientation);
    Int j = ((type / 10) - 1 + firstOrientation);
    const CVecR3 posBorder = mat.getPMLBeginningPosition();
    const CVecR3 width = mat.getWidth();
    // Computes sigma(x) = [(x - x_0)/width]^2 * maxSigma.
    Real sigma[ncp];
    i = i % 3;
    for (UInt c = 0; c < ncp; c++) {
        sigma[c] = maxSigma;
        sigma[c] *= pow((cPos[c](i) - posBorder(i))/width(i), 2);
    }
    // Computes second sigma if necessary.
    if (j - firstOrientation >= 0) {
        j = j % 3;
        for (UInt c = 0; c < ncp; c++) {
            sigma[c] *= maxSigma;
            sigma[c] *= pow((cPos[c](j) - posBorder(j))/width(j), 2);
        }
    }
    // Computes Mass Matrix.
    res = getMassMatrixIntegratedWithScalar(sigma);
    StaMatrix<Real,np,np> invMM = getMassMatrix().invert();
    return (invMM * res);
}

template<Int N, class E>
StaMatrix<Real,TET_NP,TET_NP> CellTet<N,E>::getCMatrix(
        const UInt x,
        const StaMatrix<Real,np,np>& invM,
        const MatR43 cJHat[SimplexTet<1>::ncp],
        const MatR44 cJ[SimplexTet<1>::ncp]) const {
    StaMatrix<Real,TET_NP,TET_NP> res;
    // Computes preliminary C matrices.
    for (UInt i = 0; i < this->tet.np; i++) {
        for (UInt j = 0; j < this->tet.np; j++) {
            for (UInt k = 0; k < faces; k++) {
                for (UInt c = 0; c < SimplexTet<1>::ncp; c++) {
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

template<Int N, class E>
StaMatrix<Real,TET_NP,TET_NP> CellTet<N,E>::getMassMatrix(
        const Real cJDet[SimplexTet<1>::ncp]) const{
    StaMatrix<Real,TET_NP,TET_NP> res;
    for (UInt c = 0; c < SimplexTet<1>::ncp; c++) {
        res += tet.cwaa[c] * cJDet[c];
    }
    res *= Real(1.0 / 6.0);
    return res;
}

template<Int N, class E>
CellTet<N,E>::MatNpNp CellTet<N,E>::getMassMatrix() const {
    static const UInt ncp = SimplexTet<1>::ncp;
    MatR44 cJ[SimplexTet<1>::ncp];
    this->getCubatureJacobian(cJ);
    Real cJDet[ncp];
    this->getCubatureJacobianDeterminant(cJDet, cJ);
    return getMassMatrix(cJDet);
}

template<Int N, class E>
StaMatrix<Real,TET_NP,TET_NP> CellTet<N,E>::getMassMatrixIntegratedWithScalar(
        const Real cScalar[SimplexTet<1>::ncp]) const {
    static const UInt ncp = SimplexTet<1>::ncp;
    MatR44 cJ[SimplexTet<1>::ncp];
    this->getCubatureJacobian(cJ);
    Real cJDetByScalar[ncp];
    this->getCubatureJacobianDeterminant(cJDetByScalar, cJ);
    for (UInt c = 0; c < ncp; c++) {
        cJDetByScalar[c] *= cScalar[c];
    }
    return getMassMatrix(cJDetByScalar);
}

template<Int N, class E>
UInt CellTet<N,E>::getNodeVertex(const UInt i) const {
    return tet.vertex(i);
}

template<Int N, class E>
CVecR3 CellTet<N,E>::getSideNodePos(
        const UInt f, const UInt i) const {
    return node_[tet.sideNode(f, i)];
}

template<Int N, class E>
UInt CellTet<N,E>::getSideNode(
        const UInt f, const UInt i) const {
    return tet.sideNode(f, i);
}

template<Int N, class E> void CellTet<N,E>::buildNodes() {
    // Evaluates Lagrange's functions in positions specified by the
    // simplex coordinates of tet.
    Real lagrEv[tet.np][this->numberOfCoordinates()];
    for (UInt j = 0; j < tet.np; j++) {
        for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
            lagrEv[j][i]= this->getTet().getLagr(i).eval(tet.coordinate(j));
        }
    }
    // Computes nodes.
    for (UInt j = 0; j < tet.np; j++) {
        for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
            this->n[j] += *(this->getV(i)) * lagrEv[j][i];
        }
    }
}

template<Int N, class E>
void CellTet<N,E>::printInfo() const {
    cout << " --- CellTet information --- " << endl;
    cout << "Nodes: " << this->np << endl;
    for (UInt i = 0; i < this->np; i++) {
        this->n[i].printInfo();
        cout << endl;
    }
    cout << "Side Nodes and Normal vectors:" << endl;
    for (UInt f = 0; f < this->numberOfFaces(); f++) {
        cout << "Face " << f << ": " << endl;
        for (UInt i = 0; i < this->nfp; i++) {
            cout << "Pos: ";
            this->getSideNodePos(f,i).printInfo();
            cout << "Normal: ";
            this->getSideNormal(f).printInfo();
            cout << endl;
        }
    }
    for (UInt f = 0; f < this->numberOfFaces(); f++) {
        cout << "Area of face " << f << ": "
                << this->getAreaOfFace(f) << endl;
    }
    cout << " Volume: " << this->base->getVolume() << endl;
    cout << " Neighbour nodes: (vmapP)" << endl;
    for (UInt f = 0; f < this->faces; f++) {
        cout << " Face #" << f << ": ";
        for (UInt i = 0; i < this->nfp; i++) {
            cout << this->vmapP[f][i] << " ";
        }
        cout << endl;
    }
    cout << " Vertex info: " << endl;
    for (UInt i = 0; i < this->vertices; i++) {
        this->getV(i)->printInfo();
        cout << endl;
    }
    cout << " Side Vertex Info: " << endl;
    for (UInt f = 0; f < this->faces; f++) {
        cout << "Face #" << f << endl;
        for (UInt i = 0; i < this->nfp; i++) {
            this->getSideVertexBaseNode(f,i)->printInfo();
            cout << endl;
        }
    }
    cout << " --- End of CellTet4 information --- " << endl;
}

template<Int N, class E>
array<UInt,TET_NFP> CellTet<N,E>::getNeighbourNodes(
        const UInt f,
        const Cell& neigh) const {
    vector<UInt> res(getNfp());
    CVecR3 diff, posM, posP;
    const ElementId id = getId();
    // Stores contiguous element (e2) number and orientation.
    const Tet* neigh = map.getNeighbour(id, f);
    const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
    const UInt f2 = map.getVolToF(id, f);
    // Runs over each node in local element.
    for (UInt i = 0; i < cell[e]->getNfp(); i++) {
        // Initializes mapP and vmapP to default values.
        cell[e]->vmapP[f][i] = cell[e]->getSideNode(f, i);
        // Creates the position vector of local element.
        posM = cell[e]->getSideNodePos(f, i);
        // Checks posM against all nodes in e2 face.
        for (UInt j = 0; j < c2->nfp; j++) {
            posP = c2->getSideNodePos(f2, j);
            diff = posM - posP;
            // Stores value if the share position.
            if (MathUtils::equal(diff.norm(), (Real) 0.0)) {
                res[i] = getSideNode(f2, j);
            }
        }
    }
    return res;
}

template<Int N, class E>
void CellTet<N,E>::getCurvedLIFTnormal(
        StaMatrix<Real,np,nfp> LIFTn[3],
        StaMatrix<Real,np,nfp> LIFTcn[3],
        StaMatrix<Real,np,nfp> LIFTrn[3],
        const UInt f) const {
    // Side mass matrix.
    const Tet10* base = dynamic_cast<const Tet10*>(this->base);
    assert(base != NULL);
    Tri6 face = base->getTri6Face(f);
    static const UInt ncp = SimplexTri<2>::ncp;
    Real csdf[ncp];
    face.getCubatureDifferentials(csdf);
    CVecR3 csdn[ncp], csdcn[ncp], csdrn[ncp];
    face.getCubatureNormals(csdn);
    for (UInt c = 0; c < ncp; c++) {
        csdcn[c](0) = csdn[c](0) * csdn[c](1);
        csdcn[c](1) = csdn[c](1) * csdn[c](2);
        csdcn[c](2) = csdn[c](2) * csdn[c](0);
        csdrn[c] = (Real) 1.0;
        csdrn[c] -= csdn[c] * csdn[c];
    }
    StaMatrix<Real,nfp,nfp> sMMn[3], sMMcn[3], sMMrn[3];
    static const SimplexTri<N> tri;
    for (UInt i = 0; i < 3; i++) {
        sMMn[i].zeros();
        sMMcn[i].zeros();
        sMMrn[i].zeros();
        for (UInt c = 0; c < ncp; c++) {
            sMMn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdn[c](i);
            sMMcn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdcn[c](i);
            sMMrn[i] += tri.cwaa[c] * (csdf[c] / 2.0) * csdrn[c](i);
        }
    }
    // Mass matrix.
    StaMatrix<Int,nfp,np> RInt;
    StaMatrix<Int,np,nfp> RtrInt;
    StaMatrix<Real,np,nfp> R;
    RInt = this->tet.R[f];
    RtrInt = RInt.transpose();
    R.assign(RtrInt);
    StaMatrix<Real,np,nfp> MMR;
    MMR = this->getMassMatrix().invert() * R;
    // Final form.
    for (UInt i = 0; i < 3; i++) {
        LIFTn[i] = MMR * sMMn[i];
        LIFTcn[i] = MMR * sMMcn[i];
        LIFTrn[i] = MMR * sMMrn[i];
    }
}
