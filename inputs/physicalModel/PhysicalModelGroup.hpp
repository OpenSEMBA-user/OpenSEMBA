/*
 * PhysicalModelGroup.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

template<typename P>
PhysicalModelGroup<P>::PhysicalModelGroup() {

}

template<typename P>
PhysicalModelGroup<P>::PhysicalModelGroup(const vector<P*>& elems)
:   GroupId<P, MatId>(elems) {

}

template<typename P>
PhysicalModelGroup<P>::PhysicalModelGroup(const Group<P>& rhs)
:   GroupId<P, MatId>(rhs) {

}

template<typename P>
PhysicalModelGroup<P>::~PhysicalModelGroup() {

}

template<typename P>
PhysicalModelGroup<P>& PhysicalModelGroup<P>::operator=(const Group<P>& rhs) {
    if (this == &rhs) {
        return *this;
    }

    GroupId<P, MatId>::operator=(rhs);

    return *this;
}

template<typename P>
void
PhysicalModelGroup<P>::printInfo() const {
    cout << "---- Physical Models ----" << endl;
    cout << "Number of physical models: " << this->size() << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
}

template<typename P>
void
PhysicalModelGroup<P>::getDirection(
        PMVolumePML::Direction direction[3],
        const UInt i) const {
    assert(i < PMVolumePML::possibleDirections);
    direction[x] = getDirectionFromInt((i/9) % 3);
    direction[y] = getDirectionFromInt((i/3) % 3);
    direction[z] = getDirectionFromInt(i % 3);
    assert( !(direction[0] == PMVolumePML::none
            && direction[1] == PMVolumePML::none
            && direction[2] == PMVolumePML::none) );
}

template<typename P>
PMVolumePML::Direction
PhysicalModelGroup<P>::getDirectionFromInt(
        const UInt i) const {
    assert(PMVolumePML::plus == 0);
    assert(PMVolumePML::minus == 1);
    assert(PMVolumePML::none == 2);
    assert(i == PMVolumePML::plus
            || i == PMVolumePML::minus
            || i == PMVolumePML::none);
    if (PMVolumePML::plus == i) {
        return PMVolumePML::plus;
    } else if (PMVolumePML::minus == i) {
        return PMVolumePML::minus;
    } else {
        return PMVolumePML::none;
    }
}

template<typename P>
vector<MatId> PhysicalModelGroup<P>::getIds() const {
    vector<MatId> res;
    for (UInt i = 0; i < this->size(); i++) {
        res.push_back(this->element_[i]->getId());
    }
    return res;
}
