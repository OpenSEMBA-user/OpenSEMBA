#include "../outRq/OutRqGroup.h"

template<typename O>
OutRqGroup<O>::OutRqGroup() {

}

template<typename O> template<typename O2>
OutRqGroup<O>::OutRqGroup(const vector<O2*>& outRqs) {
    add(outRqs);
}

template<typename O>
OutRqGroup<O>::OutRqGroup(const Group<O>& rhs)
:   Group<O>(rhs) {

}

template<typename O> template<typename O2>
OutRqGroup<O>::OutRqGroup(const Group<O2>& rhs)
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

template<typename O> template<typename O2>
OutRqGroup<O>& OutRqGroup<O>::operator=(const Group<O2>& rhs) {
    Group<O>::operator=(rhs);

    return *this;
}

template<typename O> template<typename T2>
void OutRqGroup<O>::add(T2* newORq) {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->template is<T2>()) {
            T2* oRq = this->element_[i]->template castTo<T2>();
            if (oRq->isSimilar(newORq)) {
                oRq->elems().add(newORq->elems());
                return;
            }
        }
    }
    Group<O>::add(newORq);
}

template<typename O> template<typename T2>
void OutRqGroup<O>::add(vector<T2*>& newOuts) {
    for (UInt i = 0; i < newOuts.size(); i++) {
        add(newOuts[i]);
    }
}

template<typename O> template<typename T2>
void OutRqGroup<O>::add(const Group<T2>& rhs) {
    Group<O>::add(rhs);
}

template<typename O>
void OutRqGroup<O>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
    cout<< " --- End of outputrequestGroup info ---" << endl;
}
