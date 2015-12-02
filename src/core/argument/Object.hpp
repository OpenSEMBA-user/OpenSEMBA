// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "Object.h"

namespace SEMBA {
namespace Argument {

template<typename ... T>
Object& Object::operator =(const T&... params) {
    return set(params...);
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
    Object::isInt() const {
    if ((type_ != Type::String) || (strValue_.size() != 1)) {
        return false;
    }
    T res;
    std::stringstream aux(strValue_.front());
    aux >> res;
    if (aux.fail()) {
        return false;
    }
    std::string rem;
    aux >> rem;
    if (!rem.empty()) {
        return false;
    }
    return true;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
    Object::isReal() const {
    if ((type_ != Type::String) || (strValue_.size() != 1)) {
        return false;
    }
    T res;
    std::stringstream aux(strValue_.front());
    aux >> res;
    if (aux.fail()) {
        return false;
    }
    std::string rem;
    aux >> rem;
    if (!rem.empty()) {
        return false;
    }
    return true;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
    Object::getInt() const {
    if ((type_ != Type::String) || !isInt<T>()) {
        throw Error::Object::NotType(Type::String, NumberType::Int);
    }
    T res;
    std::stringstream aux(strValue_.front());
    aux.exceptions(std::ios::failbit);
    aux.precision(std::numeric_limits<T>::max_digits10);
    aux >> res;
    return res;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
    Object::getReal() const {
    if ((type_ != Type::String) || !isReal<T>()) {
        throw Error::Object::NotType(Type::String, NumberType::Real);
    }
    T res;
    std::stringstream aux(strValue_.front());
    aux.exceptions(std::ios::failbit);
    aux.precision(std::numeric_limits<T>::max_digits10);
    aux >> res;
    return res;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, Object&>::type
    Object::setInt(const T& num) {
    clear_();
    std::stringstream aux;
    aux.precision(std::numeric_limits<T>::max_digits10);
    aux << num;
    type_ = Type::String;
    strValue_.push_back(aux.str());
    return *this;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, Object&>::type
    Object::setReal(const T& num) {
    clear_();
    std::stringstream aux;
    aux.precision(std::numeric_limits<T>::max_digits10);
    aux << num;
    type_ = Type::String;
    strValue_.push_back(aux.str());
    return *this;
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
    aux.precision(std::numeric_limits<T>::max_digits10);
    aux << param;
    strValue_.push_back(aux.str());
}

template<typename T1, typename T2, typename... T>
void Object::set_(const T1& param1, const T2& param2, const T&... params) {
    std::stringstream aux;
    aux.precision(std::numeric_limits<T1>::max_digits10);
    aux << param1;
    strValue_.push_back(aux.str());
    set_(param2, params...);
}

} /* namespace Objects */
} /* namespace SEMBA */
