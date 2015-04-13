#ifndef IDBASE_H_
#define IDBASE_H_

#include <iostream>

using namespace std;

#include "Types.h"

#include "ClassCompBase.h"

class IdBase : public ClassCompBase {
public:
    IdBase();
    explicit IdBase(const UInt id);
    IdBase(const IdBase& rhs);
    virtual ~IdBase();
    
    operator UInt() const;
    
protected:
    UInt id_;
};

#define CREATE_ID(NAME)                                     \
class NAME : public IdBase {                                \
public:                                                     \
    NAME() {}                                               \
    explicit NAME(const UInt id) : IdBase( id) {}           \
    NAME(const NAME& rhs)        : IdBase(rhs) {}           \
    virtual ~NAME() {}                                      \
                                                            \
    ClassBase* clone() const {  \
        return new NAME(*this); \
    }                                                       \
                                                            \
    NAME& operator=(const NAME& rhs) {                      \
        if(this == &rhs)                                    \
            return *this;                                   \
        id_ = rhs.id_;                                      \
        return *this;                                       \
    }                                                       \
                                                            \
    bool operator<(const ClassCompBase& rhs) const {        \
        if (ClassCompBase::operator<(rhs)) {                \
            return true;                                    \
        }                                                   \
        const NAME* rhsPtr = rhs.castTo<NAME>();            \
        return (id_ < rhsPtr->id_);                         \
    }                                                       \
    NAME operator+(const NAME& rhs) const {                 \
        return NAME(id_ + rhs.id_);                         \
    }                                                       \
    NAME operator-(const NAME& rhs) const {                 \
        return NAME(id_ - rhs.id_);                         \
    }                                                       \
    NAME& operator+=(const NAME& rhs) {                     \
        id_ += rhs.id_;                                     \
        return *this;                                       \
    }                                                       \
    NAME& operator-=(const NAME& rhs) {                     \
        id_ -= rhs.id_;                                     \
        return *this;                                       \
    }                                                       \
    NAME& operator++() {                                    \
        id_++;                                              \
        return *this;                                       \
    }                                                       \
    NAME operator++(int) {                                  \
        NAME copy(*this);                                   \
        id_++;                                              \
        return copy;                                        \
    }                                                       \
                                                            \
    const string& getOwnTypeNameStr() const {               \
        static const string res = #NAME;                    \
        return res;                                         \
    }                                                       \
    void printInfo() const {                                \
        cout << "Id (" << id_ << ")";                       \
    }                                                       \
                                                            \
    friend istream &operator>>(istream  &input, NAME &id) { \
        input >> id.id_;                                    \
        return input;                                       \
    }                                                       \
}

#endif /* IDBASE_H_ */
