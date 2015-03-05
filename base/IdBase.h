#ifndef IDBASE_H_
#define IDBASE_H_

#include <iostream>

using namespace std;

class IdBase {
public:
    IdBase();
    explicit IdBase(const unsigned id);
    IdBase(const IdBase& rhs);
    virtual ~IdBase();
    
    operator unsigned() const;
    
protected:
    unsigned id_;
};

#define CreateId(name)                                      \
class name : public IdBase {                                \
public:                                                     \
    name()                                         {}       \
    explicit name(const unsigned id) : IdBase( id) {}       \
    name(const name& rhs)            : IdBase(rhs) {}       \
    virtual ~name()                                {}       \
                                                            \
    name& operator=(const unsigned id) {                    \
        id_ = id;                                           \
        return *this;                                       \
    }                                                       \
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
    friend istream &operator>>(istream  &input, name &id) { \
        input >> id.id_;                                    \
        return input;                                       \
    }                                                       \
}

#endif /* IDBASE_H_ */
