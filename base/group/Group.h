#ifndef GROUP_H_
#define GROUP_H_

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

#include "SuperGroup.h"
#include "ClassBase.h"

template<typename T>
class Group : public virtual ClassBase {
    template<typename T2>
    friend class Group;
public:
    Group();

    template<typename T2>
    Group(T2*);
    template<typename T2>
    Group(const vector<T2*>&);

    Group(Group<T>&);
    template<typename T2>
    Group(Group<T2>& rhs);
    template<typename T2>
    Group(const Group<T2>& rhs);

    Group(Group<T>&&);
    template<typename T2>
    Group(Group<T2>&& rhs);

    virtual ~Group();

    ClassBase* clone() const;
    Group<typename remove_const<T>::type> newGroup() const;

    Group<T>& operator=(Group<T>& rhs);
    template<typename T2>
    Group<T>& operator=(Group<T2>& rhs);
    template<typename T2>
    Group<T>& operator=(const Group<T2>& rhs);
    Group<T>& operator=(Group<T>&& rhs);
    template<typename T2>
    Group<T>& operator=(Group<T2>&& rhs);

    void reserve(const UInt nS) { element_.reserve(nS);    }
    bool empty() const          { return element_.empty(); }
    UInt size () const          { return element_.size();  }
    void clear();

    template<class T2>
    bool emptyOf() const;
    template<class T2>
    UInt sizeOf() const;

    template<class T2>
    Group<typename conditional<is_const<T>::value, const T2, T2>::type>
                    getGroupOf();
    template<class T2>
    Group<const T2> getGroupOf() const;
    template<class T2>
    Group<T>        getGroupNotOf();
    template<class T2>
    Group<const T>  getGroupNotOf() const;

    Group<T> getGroupWith(const UInt);
    Group<T> getGroupWith(const vector<UInt>&);
    Group<const T> getGroupWith(const UInt) const;
    Group<const T> getGroupWith(const vector<UInt>&) const;
    Group<T> getGroupWithout(const UInt);
    Group<T> getGroupWithout(const vector<UInt>&);
    Group<const T> getGroupWithout(const UInt) const;
    Group<const T> getGroupWithout(const vector<UInt>&) const;

    T*       operator()(const UInt i)       { return element_[i].get(); }
    const T* operator()(const UInt i) const { return element_[i].get(); }

    T*       get(const UInt i)       { return element_[i].get(); }
    const T* get(const UInt i) const { return element_[i].get(); }

    template<typename T2>
    T* add(T2* newElem);
    template<typename T2>
    vector<T*> add(const vector<T2*>&);
    template<typename T2>
    vector<T*> add(Group<T2>&);
    template<typename T2>
    vector<T*> add(const Group<T2>&);
    template<typename T2>
    vector<T*> add(Group<T2>&&);

    void remove(const UInt);
    void remove(const vector<UInt>&);

    void printInfo() const;

protected:
    virtual void construct();
    virtual void destruct ();

    virtual vector<T*> preprocess (const vector<T*>& elems) const;
    virtual void       postprocess(const UInt i);

private:
    vector<shared_ptr<T>> element_;
    set<const T*> set_;

    template<typename T2>
    Group(const vector<shared_ptr<T2>>&);

    template<typename T2>
    vector<typename add_pointer<
               typename conditional<is_const<T>::value,
                                    const T2, T2>::type>::type>
                    getElemsOf_   () const;
    template<typename T2>
    vector<T*>      getElemsNotOf_() const;

    vector<T*>  getElemsWith_   (const vector<UInt>&) const;
    vector<T*>  getElemsWithout_(const vector<UInt>&) const;

    template<typename T2>
    static vector<T*> compatElems_(const vector<T2*>&);

    vector<typename add_pointer<
               typename remove_const<T>::type>::type> cloneElems_() const;
};

#include "Group.hpp"

#define USE_GROUP_CONSTRUCTS(NAME, T)                           \
    NAME() {}                                                   \
    template<typename T2>                                       \
    NAME(T2*& elems)            { this->add(elems);          }  \
    template<typename T2>                                       \
    NAME(vector<T2*>& elems)    { this->add(elems);          }  \
    NAME(Group<T>& rhs)         { this->add(rhs);            }  \
    template<typename T2>                                       \
    NAME(Group<T2>& rhs)        { this->add(rhs);            }  \
    template<typename T2>                                       \
    NAME(const Group<T2>& rhs)  { this->add(rhs);            }  \
    NAME(Group<T>&& rhs)        { Group<T>::add(std::move(rhs)); }  \
    template<typename T2>                                       \
    NAME(Group<T2>&& rhs)       { Group<T>::add(std::move(rhs)); }  \
    virtual ~NAME() {}

#define DEFINE_GROUP_CLONE(NAME, T)                         \
    ClassBase* clone() const {                              \
        return new NAME<typename remove_const<T>::type>(    \
                       this->newGroup());                  \
    }

#define USE_GROUP_ASSIGN(T)                         \
    Group<T>& operator=(Group<T>& rhs) {            \
        return Group<T>::operator=(rhs);            \
    }                                               \
    template<typename T2>                           \
    Group<T>& operator=(Group<T2>& rhs) {           \
        return Group<T>::operator=(rhs);            \
    }                                               \
    template<typename T2>                           \
    Group<T>& operator=(const Group<T2>& rhs) {     \
        return Group<T>::operator=(rhs);            \
    }                                               \
    Group<T>& operator=(Group<T>&& rhs) {           \
        return Group<T>::operator=(std::move(rhs)); \
    }                                               \
    template<typename T2>                           \
    Group<T>& operator=(Group<T2>&& rhs) {          \
        return Group<T>::operator=(std::move(rhs)); \
    }

#define USE_GROUP_GETGROUPOF(T)                                         \
    template<class T2>                                                  \
    Group<typename conditional<is_const<T>::value, const T2, T2>::type> \
        getGroupOf() {                                                  \
        return Group<T>::getGroupOf<T2>();                              \
    }                                                                   \
    template<class T2>                                                  \
    Group<const T2> getGroupOf() const {                                \
        return Group<T>::getGroupOf<T2>();                              \
    }                                                                   \
    template<class T2>                                                  \
    Group<T> getGroupNotOf() {                                          \
        return Group<T>::getGroupNotOf<T2>();                           \
    }                                                                   \
    template<class T2>                                                  \
    Group<const T> getGroupNotOf() const {                              \
        return Group<T>::getGroupNotOf<T2>();                           \
    }

#define USE_GROUP_GETGROUPWITH(T)                                       \
    Group<T> getGroupWith(const UInt elems) {                           \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<T> getGroupWith(const vector<UInt>& elems) {                  \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<const T> getGroupWith(const UInt elems) const {               \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<const T> getGroupWith(const vector<UInt>& elems) const {      \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<T> getGroupWithout(const UInt elems) {                        \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<T> getGroupWithout(const vector<UInt>& elems) {               \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<const T> getGroupWithout(const UInt elems) const {            \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<const T> getGroupWithout(const vector<UInt>& elems) const {   \
        return Group<T>::getGroupWithout(elems);                        \
    }

#define USE_GROUP_GET(T)                                            \
    T*       get(const UInt i)       { return Group<T>::get(i); }   \
    const T* get(const UInt i) const { return Group<T>::get(i); }

#define USE_GROUP_ADD(T)                        \
template<typename T2>                           \
    T* add(T2* newElem) {                       \
        return Group<T>::add(newElem);          \
    }                                           \
    template<typename T2>                       \
    vector<T*> add(const vector<T2*>& elems) {  \
        return Group<T>::add(elems);            \
    }                                           \
    template<typename T2>                       \
    vector<T*> add(Group<T2>& rhs) {            \
        return Group<T>::add(rhs);              \
    }                                           \
    vector<T*> add(const Group<T2>& rhs) {      \
        return Group<T>::add(rhs);              \
    }                                           \
    vector<T*> add(Group<T2>&& rhs) {           \
        return Group<T>::add(std::move(rhs));   \
    }

#define USE_GROUP_REMOVE(T)                     \
    void remove(const UInt elem) {              \
        Group<T>::remove(elem);                 \
    }                                           \
    void remove(const vector<UInt>& elems) {    \
        Group<T>::remove(elems);                \
    }

#endif /* GROUP_H_ */
