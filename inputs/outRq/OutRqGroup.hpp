#include "../outRq/OutRqGroup.h"

template<typename O>
OutRqGroup<O>::OutRqGroup() {

}

template<typename O> template<typename O2>
OutRqGroup<O>::OutRqGroup(const vector<O2*>& outRqs)
:   Group<O>(outRqs) {

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
void OutRqGroup<O>::add(OutRq<T2>* newORq) {
    for (UInt i = 0; i < this->size(); i++) {
        OutRq<>* local = this->element_[i];
        if (local->is<OutRq<T2> >()) {
            OutRq<T2>* oRq = local->castTo<OutRq<T2> >();
            if (oRq->isSimilar(newORq)) {
                oRq->add(newORq);
                return;
            }
        }
    }
    this->Group<O>::add(newORq);
}

template<typename O>
void OutRqGroup<O>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
    cout<< " --- End of outputrequestGroup info ---" << endl;
}
