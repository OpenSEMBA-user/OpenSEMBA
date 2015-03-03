#ifndef CLASSBASE_H_
#define CLASSBASE_H_

#include <cstdlib>

using namespace std;

class ClassBase {
public:
    ClassBase();
    virtual ~ClassBase();

    virtual ClassBase* clone() const = 0;

    template<typename T>
    bool isOf() const;

    template<typename T>
    T* castTo();

    template<typename T>
    const T* castTo() const;
};

#endif /* CLASSBASE_H_ */
