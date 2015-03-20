#include "../outRq/OutRqGroup.h"

template<typename O>
OutRqGroup<O>::OutRqGroup() {

}

template<typename O>
OutRqGroup<O>::OutRqGroup(const vector<O*>& outRqs) {
    add(outRqs);
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

template<typename O>
void OutRqGroup<O>::add(O* newOutRq) {
    vector<O*> aux;
    aux.push_back(newOutRq);
    add(aux);
}

template<typename O>
void OutRqGroup<O>::add(vector<O*>& newOutRqs) {
    for (UInt i = 0; i < newOutRqs.size(); i++) {
        bool isSimilar = false;
        for (UInt j = 0; j < this->size(); j++) {
            if (newOutRqs[i]->isSimilar(*this->element_[j])) {
                isSimilar = true;
                this->element_[j]->setAdditionalElems(
                                       newOutRqs[i]->getElems());
                break;
            }
        }
        if (!isSimilar) {
            this->element_.push_back(newOutRqs[i]);
        }
    }
}

template<typename O>
void OutRqGroup<O>::add(const Group<O>& rhs) {
    Group<O>::add(rhs);
}

template<typename C>
void OutRqGroup<C>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
    cout<< " --- End of outputrequestGroup info ---" << endl;
}
