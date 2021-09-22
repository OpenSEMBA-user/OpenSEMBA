

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {

template<class T>
OutputRequest<T>::OutputRequest(const Domain& domain,
                                const Type type,
                                const std::string& name,
                                const Geometry::Element::Group<T>& elems)
:   Base(type, name, domain),
    Geometry::Element::Group<const T>(elems) {
    if (type == bulkCurrentElectric || type == bulkCurrentMagnetic) {
        if (this->getMatId(MatId(0)).size() != this->size()) {
            throw Error::Material();
        }
    }
}

template<class T>
OutputRequest<T>::OutputRequest(const OutputRequest<T>& rhs)
:   Base(rhs),
    Geometry::Element::Group<const T>(rhs) {

}

template<class T>
OutputRequest<T>::OutputRequest(const OutputRequest::Base& rhs)
	: Base(rhs) {
}

template<class T>
OutputRequest<T>::~OutputRequest() {

}

template <class T>
void OutputRequest<T>::set(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::operator=(elems);
}

template <class T>
void OutputRequest<T>::add(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::add(elems);
}

template<class T>
inline void OutputRequest<T>::printInfo() const {
    std::cout << " --- Output request instance --- " << std::endl;
    Base::printInfo();
    Geometry::Element::Group<const T>::printInfo();
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
