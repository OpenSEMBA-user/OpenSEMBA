#ifndef CLASSBASE_H_
#define CLASSBASE_H_

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>

using namespace std;

class ClassBase {
public:
    ClassBase();
    virtual ~ClassBase();

    virtual ClassBase* clone() const = 0;

    template<typename T>
    bool is() const {
        if(dynamic_cast<const T*>(this) != NULL)
            return true;
        return false;
    }

    template<typename T>
    T* castTo() {
        if(!this->is<T>()) {
            cerr << endl << "ERROR @ ClassBase::castTo():"
                 << "Invalid cast" << endl;
            assert(false);
            exit(EXIT_FAILURE);
            return NULL;
        }
        return dynamic_cast<T*>(const_cast<ClassBase*>(this));
    }

    template<typename T>
    const T* castTo() const {
        if(!this->is<T>()) {
            cerr << endl << "ERROR @ ClassBase::castTo():"
                 << "Invalid cast" << endl;
            assert(false);
            exit(EXIT_FAILURE);
            return NULL;
        }
        return dynamic_cast<const T*>(this);
    }

    virtual void printInfo() const = 0;
};

#endif /* CLASSBASE_H_ */
