#include "../outRq/OutRqGroup.h"

template<typename O> template<typename O2>
O* OutRqGroup<O>::add(O2* newORq) {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<O2>()) {
            O2* oRq = this->get(i)->template castTo<O2>();
            if (oRq->hasSameProperties(*newORq)) {
                oRq->add(newORq->elems());
                delete newORq;
                return NULL;
            }
        }
    }
    return Group<O>::add(newORq);
}

template<typename O> template<typename O2>
vector<O*> OutRqGroup<O>::add(vector<O2*>& newOuts) {
    vector<O*> res;
    res.reserve(newOuts.size());
    for (UInt i = 0; i < newOuts.size(); i++) {
        O* resElem = add(newOuts[i]);
        if (resElem != NULL) {
            res.push_back(resElem);
        }
    }
    return res;
}

template<typename O> template<typename O2>
vector<O*> OutRqGroup<O>::add(Group<O2>& rhs) {
    return Group<O>::add(rhs);
}

template<typename O>
void OutRqGroup<O>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    Group<O>::printInfo();
}
