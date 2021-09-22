

#include "Object.h"

namespace SEMBA {
namespace Argument {

template<typename ... T>
Object& Object::operator =(const T&... params) {
    return set(params...);
}

template<typename... T>
bool Object::is() const {
    if (type_ != Type::String) {
        return false;
    }
    try {
        StringParser::parse<T...>(strValue_);
    } catch (...) {
        return false;
    }
    return true;
}

template<typename T>
T Object::get() const {
    if (type_ != Type::String) {
        if (std::is_same<bool, T>::value) {
            throw Error::Object::NotType(Type::Bool);
        }
        if (std::is_integral<T>::value || std::is_floating_point<T>::value) {
            throw Error::Object::NotType(Type::Number);
        }
        throw Error::Object::NotType(Type::String);
    }
    return StringParser::parse<T>(strValue_);
}

template<typename T1, typename T2>
std::pair<T1, T2> Object::get() const {
    if (type_ != Type::String) {
        throw Error::Object::NotType(Type::String);
    }
    return StringParser::parse<T1, T2>(strValue_);
}

template<typename T1, typename T2, typename T3, typename... T>
std::tuple<T1, T2, T3, T...> Object::get() const {
    if (type_ != Type::String) {
        throw Error::Object::NotType(Type::String);
    }
    return StringParser::parse<T1, T2, T3, T...>(strValue_);
}

template<typename ... T>
Object& Object::set(const T&... params) {
    clear_();
    set_(params...);
    type_ = Type::String;
    return *this;
}

template<typename T>
void Object::set_(const T& param) {
    std::stringstream aux;
    aux << param;
    strValue_.push_back(aux.str());
}

template<typename T1, typename T2, typename... T>
void Object::set_(const T1& param1, const T2& param2, const T&... params) {
    std::stringstream aux;
    aux << param1;
    strValue_.push_back(aux.str());
    set_(param2, params...);
}

} /* namespace Objects */
} /* namespace SEMBA */
