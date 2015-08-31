#ifndef GROUP_H_
#define GROUP_H_

#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "../class/ClassBase.h"
#include "../sequence/Vector.h"

template<typename T>
class Group : public virtual ClassBase,
              public Vector<T*> {
public:
    Group() {}
    template<typename T2>
    Group(T2* e)                     : Vector<T*>(e) {}
    template<typename T2>
    Group(const std::vector<T2*>& e) : Vector<T*>(e) {}
    template<typename T2>
    Group(VectorPtr<T2>&       rhs) : Vector<T*>(rhs) {}
    template<typename T2>
    Group(const VectorPtr<T2>& rhs) : Vector<T*>(rhs) {}
    Group(VectorPtr<T>&        rhs) : Vector<T*>(rhs) {}
    template<typename T2>
    Group(VectorPtr<T2>&& rhs) : Vector<T*>(std::move(rhs)) {}
    Group(VectorPtr<T >&& rhs) : Vector<T*>(std::move(rhs)) {}
    virtual ~Group() {}

    Group<T>* clone() const;

    virtual Group& operator=(VectorPtr<T>&  rhs);
    virtual Group& operator=(VectorPtr<T>&& rhs);

    template<class T2>
    bool emptyOf() const;
    template<class T2>
    UInt sizeOf () const;

    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOf    ();
    template<class T2>
    Group<const T2> getOf    () const;
    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOfOnly();
    template<class T2>
    Group<const T2> getOfOnly() const;

    void printInfo() const;

private:
    template<typename T2>
    std::vector<UInt> getElemsOf_       () const;
    template<typename T2>
    std::vector<UInt> getElemsNotOf_    () const;
    template<typename T2>
    std::vector<UInt> getElemsOfOnly_   () const;
    template<typename T2>
    std::vector<UInt> getElemsNotOfOnly_() const;
};

#include "Group.hpp"

#define DEFINE_GROUP_CLONE(NAME, T)             \
    NAME<T>* clone() const {                    \
        return new NAME<T>(this->cloneElems()); \
    }

#endif /* GROUP_H_ */
