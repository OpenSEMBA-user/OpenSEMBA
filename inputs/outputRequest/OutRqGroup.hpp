#include "OutRqGroup.h"

template<typename O>
OutRqGroup<O>::OutRqGroup() {

}

template<typename O>
OutRqGroup<O>::OutRqGroup(const vector<O*>& in) {
    for (UInt i = 0; i < in.size(); i++) {
        bool isSimilar = false;
        for (UInt j = 0; j < this->size(); j++) {
            if (in[i]->isSimilar(*this->element_[j])) {
                isSimilar = true;
                this->element_[j]->setAdditionalElems(in[i]->getElem());
                break;
            }
        }
        if (!isSimilar) {
            add(in[i]);
        }
    }
}

template<typename O>
OutRqGroup<O>::OutRqGroup(const Group<O>& rhs)
:   Group<O>(rhs) {

}

template<typename O>
OutRqGroup<O>::~OutRqGroup() {

}

template<typename O>
OutRqGroup<O>& OutRqGroup<O>::operator=(const Group<O>& rhs) {
    if (this == &rhs) {
        return *this;
    }

    Group<O>::operator=(rhs);

    return *this;
}

template<typename C>
void OutRqGroup<C>::add (OutRq* in) {
    bool foundSimilar = false;
    for (UInt j = 0; j < this->size(); j++) {
        if (in->isSimilar(*this->element_[j])) {
            foundSimilar = true;
            this->element_[j]->setAdditionalElems(in->getElem());
            delete in;
            break;
        }
    }
    if (!foundSimilar) {
        Group<C>::add(in);
    }
}

template<typename C>
void OutRqGroup<C>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
    cout<< " --- End of outputrequestGroup info ---" << endl;
}
