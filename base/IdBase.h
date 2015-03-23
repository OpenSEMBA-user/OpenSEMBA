#ifndef IDBASE_H_
#define IDBASE_H_

#include <iostream>

using namespace std;

#include "Types.h"

class IdBase {
public:
    IdBase();
    explicit IdBase(const UInt id);
    IdBase(const IdBase& rhs);
    virtual ~IdBase();
    
    operator UInt() const;

    
protected:
    UInt id_;
};

#define CreateId(name)                                      \
class name : public IdBase {                                \
public:                                                     \
    name() {}                                               \
    explicit name(const UInt id) : IdBase( id) {}           \
    name(const name& rhs)        : IdBase(rhs) {}           \
    virtual ~name() {}                                      \
                                                            \
    name& operator=(const name& rhs) {                      \
        if(this == &rhs)                                    \
            return *this;                                   \
        id_ = rhs.id_;                                      \
        return *this;                                       \
    }                                                       \
                                                            \
    name& operator++() {                                    \
        id_++;                                              \
        return *this;                                       \
    }                                                       \
    name& operator++(int) {                                 \
        id_++;                                              \
        return *this;                                       \
    }                                                       \
    bool operator==(const name& rhs) const {              \
        return id_ == rhs.id_;                              \
    }                                                        \
    friend istream &operator>>(istream  &input, name &id) { \
        input >> id.id_;                                    \
        return input;                                       \
    }                                                       \
}

#endif /* IDBASE_H_ */
