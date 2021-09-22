

#include "Printable.h"

#include <iostream>

namespace SEMBA {
namespace Group {

template<typename T>
void Printable<T>::printInfo() const {
    std::cout << "Number of elements in group: " << this->size() << std::endl;
    for (std::size_t i = 0; i < this->size(); i++) {
        this->get(i)->printInfo();
    }
}

} /* namespace Group */
} /* namespace SEMBA */
