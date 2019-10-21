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

#ifndef SEMBA_ARGUMENT_OBJECT_H_
#define SEMBA_ARGUMENT_OBJECT_H_

#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "StringParser.h"

namespace SEMBA {
namespace Argument {

class Object {
public:
#ifdef SEMBA_ARGUMENT_OBJECT_INT
    typedef SEMBA_ARGUMENT_OBJECT_INT  Int;
#else
    typedef int                        Int;
#endif
#ifdef SEMBA_ARGUMENT_OBJECT_REAL
    typedef SEMBA_ARGUMENT_OBJECT_REAL Real;
#else
    typedef double                     Real;
#endif
    enum class Type {
        Null,
        Dictionary,
        Array,
        Bool,
        Number,
        String
    };

    static std::string getTypeStr(const Type& type) {
        switch (type) {
        case Type::Null:
            return "Null";
            break;
        case Type::Dictionary:
            return "Object";
            break;
        case Type::Array:
            return "Array";
            break;
        case Type::Bool:
            return "Bool";
            break;
        case Type::Number:
            return "Number";
            break;
        case Type::String:
            return "String";
            break;
        }
        return "Null";
    }

    Object() {
        clear_();
    }
    Object(const Object& rhs) {
        copy_(rhs);
    }
    Object(Object&& rhs) {
        move_(std::move(rhs));
    }
    virtual ~Object() {
        clear_();
    }

    Object& operator =(const Object& rhs) {
        if (this == &rhs) {
            return *this;
        }
        copy_(rhs);
        return *this;
    }
    Object& operator =(Object&& rhs) {
        move_(std::move(rhs));
        return *this;
    }
    template<typename... T>
    Object& operator =(const T&...);

    Object& operator+=(const Object& rhs) {
        *this = *this + rhs;
        return *this;
    }
    Object  operator+ (const Object& rhs) const {
        if (isNull() && rhs.isNull()) {
            return *this;
        } else if (isNull()) {
            return rhs;
        } else if (rhs.isNull()) {
            return *this;
        }
        if (isObject() && rhs.isObject()) {
            Object res;
            res.setObject();
            for (std::size_t i = 0; i < size(); i++) {
                if (rhs.existsName(getName(i))) {
                    res(getName(i)) = (*this)(getName(i)) + rhs(getName(i));
                } else {
                    res(getName(i)) = (*this)(getName(i));
                }
            }
            for (std::size_t i = 0; i < rhs.size(); i++) {
                if (!res.existsName(rhs.getName(i))) {
                    res(rhs.getName(i)) = rhs(rhs.getName(i));
                }
            }
            return res;
        } else if (isObject()) {
            return *this;
        } else if (rhs.isObject()) {
            return rhs;
        }
        return rhs;
    }

    Object& reset() { return setType(type_); }

    //Value Type
    bool     isType(const Type& type) const {
        if (type_ == Type::String) {
            if (type == Type::Bool) {
                return is<bool>();
            }
            if (type == Type::Number) {
                return is<Int>() || is<Real>();
            }
        }
        return type == type_;
    }
    Type    getType()                 const { return type_; }
    Object& setType(const Type& type) {
        if (((type == Type::Dictionary) || (type == Type::Array) ||
             (type == Type::Null)) && (type != type_)) {
            clear_();
            type_ = type;
        }
        return *this;
    }

    //Null Value
    bool     isNull() const { return  isType(Type::Null); }
    Object& setNull()       { return setType(Type::Null); }

    //Object Value
    bool     isObject() const { return  isType(Type::Dictionary); }
    Object& setObject()       { return setType(Type::Dictionary); }
    Object&       operator ()(const std::string& name) {
        setObject();
        if (nameMembers_.count(name) == 0) {
            nameMembers_[name] = objMembers_.size();
            objMembers_.push_back(Object());
            objName_   .push_back(name);
        }
        return objMembers_.at(nameMembers_.at(name));

    }
    const Object& operator ()(const std::string& name) const {
        return objMembers_.at(nameMembers_.at(name));
    }
    Object&   addMember(const std::string& name, const Object& val) {
        setObject();
        if (nameMembers_.count(name) == 0) {
            nameMembers_[name] = objMembers_.size();
            objMembers_.push_back(Object());
            objName_   .push_back(name);
        }
        objMembers_.at(nameMembers_.at(name)) = val;
        return *this;
    }
    Object&   addMember(const std::string& name, Object&& val) {
        setObject();
        if (nameMembers_.count(name) == 0) {
            nameMembers_[name] = objMembers_.size();
            objMembers_.push_back(Object());
            objName_   .push_back(name);
        }
        objMembers_.at(nameMembers_.at(name)) = std::move(val);
        return *this;
    }
    Object& eraseMember(const std::string& name) {
        if (existsName(name)) {
            objMembers_ .erase(objMembers_.begin() + nameMembers_.at(name));
            objName_    .erase(objName_   .begin() + nameMembers_.at(name));
            nameMembers_.erase(name);
        }
        return *this;
    }
    bool            existsName(const std::string& name) const {
        if (!isObject() || (nameMembers_.count(name) == 0)) {
            return false;
        }
        return !objMembers_.at(nameMembers_.at(name)).isNull();
    }
    const std::string& getName(const std::size_t& i) const {
        return objName_.at(i);
    }

    //Array Value
    bool     isArray() const { return  isType(Type::Array); }
    Object& setArray()       { return setType(Type::Array); }
    std::size_t size() const { return objMembers_.size();   }
    Object&       operator ()(const std::size_t& i) {
        return objMembers_.at(i);
    }
    const Object& operator ()(const std::size_t& i) const {
        return objMembers_.at(i);
    }
    Object&   addValue(const Object& val) {
        setArray();
        objMembers_.push_back(val);
        return *this;
    }
    Object&   addValue(Object&& val) {
        setArray();
        objMembers_.push_back(std::move(val));
        return *this;
    }
    Object& eraseValue(const std::size_t& i) {
        if (isArray()) {
            objMembers_.erase(objMembers_.begin() + i);
        }
        return *this;
    }

    //Bool Value
    bool     isBool () const          { return isType(Type::Bool); }
    bool    getBool () const          { return get<bool>();        }
    Object& setBool (const bool& val) { return set(val);           }
    Object& setTrue ()                { return setBool(true );     }
    Object& setFalse()                { return setBool(false);     }

    //Number Value
    bool     isNumber() const          { return isType(Type::Number); }
    bool     isInt   () const          { return is <Int >();          }
    bool     isReal  () const          { return is <Real>();          }
    Int     getInt   () const          { 
		Real val = getReal();
		if (std::numeric_limits<Int>::max() < std::abs(val)) {
			std::stringstream msg;
			msg << "Integer value overflow: " << val;
			throw std::logic_error(msg.str());
		}
		return (Int) std::floor(val); 
	}
    Real    getReal  () const          { return get<Real>();          }
    Object& setInt   (const Int&  val) { return set(val);             }
    Object& setReal  (const Real& val) { return set(val);             }

    //String Value
    bool         isString() const { return isType(Type::String); }
    std::string getString() const {
        std::string res;
        for (std::list<std::string>::const_iterator it = strValue_.begin();
        it != strValue_.end(); ++it) {
            if (it != strValue_.begin()) {
                res += " ";
            }
            res += *it;
        }
        return res;
    }
    Object&     setString(const std::string& str) {
        clear_();
        type_ = Type::String;
        strValue_.push_back(str);
        return *this;
    }
    
    const std::list<std::string>& getGenericString() const {
        return strValue_;
    }
    Object&                       setGenericString(
            const std::list<std::string>& str) {
        clear_();
        type_ = Type::String;
        strValue_ = str;
        return *this;
    }

    //Generic Value
    template <typename... T>
    bool is() const;
    template<typename T>
    T                            get() const;
    template<typename T1, typename T2>
    std::pair<T1, T2>            get() const;
    template<typename T1, typename T2, typename T3, typename... T>
    std::tuple<T1, T2, T3, T...> get() const;
    template<typename... T>
    Object&                      set(const T&...);

    void printInfo(const bool& asString = false,
                   const std::size_t& depth = 0) const {
        switch (getType()) {
        case Object::Type::Null:
            std::cout << "null";
            break;
        case Object::Type::String:
            if (isNumber() && !asString) {
                if (isInt()) {
                    std::cout.precision(
                        std::numeric_limits<long long>::max_digits10);
                    std::cout << getInt();
                } else {
                    std::cout.precision(
                        std::numeric_limits<long double>::max_digits10);
                    std::cout << getReal();
                }
            } else {
                const std::list<std::string>& val = getGenericString();
                for (std::list<std::string>::const_iterator
                     it = val.begin(); it != val.end(); ++it) {
                    if (it != val.begin()) {
                        std::cout << " ";
                    }
                    std::cout << "\"" << *it << "\"";
                }
            }
            break;
        case Object::Type::Array:
            std::cout << "[" << std::endl;
            for (std::size_t i = 0; i < size(); i++) {
                std::cout << std::string(depth+2, ' ');
                (*this)(i).printInfo(asString, depth+2);
                if (i+1 < size()) {
                    std::cout << ",";
                }
                std::cout << std::endl;
            }
            std::cout << std::string(depth, ' ') << "]";
            break;
        case Object::Type::Dictionary:
            std::cout << "{" << std::endl;
            for (std::size_t i = 0; i < size(); i++) {
                std::cout << std::string(depth+2, ' ');
                std::cout << "\"" << getName(i) << "\" : ";
                (*this)(i).printInfo(asString, depth+2);
                if (i+1 < size()) {
                    std::cout << ",";
                }
                std::cout << std::endl;
            }
            std::cout << std::string(depth, ' ') << "}";
            break;
        default:
            break;
        }
        if (depth == 0) {
            std::cout << std::endl;
        }
    }

private:
    //Value Type
    Type type_;
    //Object Value
    std::vector<Object>                objMembers_;
    std::vector<std::string>           objName_;
    std::map<std::string, std::size_t> nameMembers_;
    //String Value
    std::list<std::string> strValue_;

    void copy_(const Object& rhs) {
        //Value Type
        type_ = rhs.type_;
        //Object Value
        objMembers_  = rhs.objMembers_;
        objName_     = rhs.objName_;
        nameMembers_ = rhs.nameMembers_;
        //String Value
        strValue_ = rhs.strValue_;
    }
    void move_(Object&& rhs) {
        //Value Type
        type_ = rhs.type_;
        //Object Value
        objMembers_  = std::move(rhs.objMembers_);
        objName_     = std::move(rhs.objName_);
        nameMembers_ = std::move(rhs.nameMembers_);
        rhs.objName_.clear();
        rhs.objMembers_.clear();
        rhs.nameMembers_.clear();
        //String Value
        strValue_ = std::move(rhs.strValue_);
    }
    void clear_() {
        //Value Type
        type_ = Type::Null;
        //Object Value
        objMembers_ .clear();
        objName_    .clear();
        nameMembers_.clear();
        //String Value
        strValue_.clear();
    }

    template<typename T>
    void set_(const T&);
    template<typename T1, typename T2, typename... T>
    void set_(const T1&, const T2&, const T&...);
};

namespace Error {
namespace Object {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class NotType : public Error {
public:
    NotType(const SEMBA::Argument::Object::Type& t)
    :   type_(t),
        str_(std::string("Object not a valid ") +
             SEMBA::Argument::Object::getTypeStr(type_)) {
        std::stringstream aux;
        aux << "Object not a valid "
            << SEMBA::Argument::Object::getTypeStr(type_);
        str_ = aux.str();
    }
    virtual ~NotType() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const SEMBA::Argument::Object::Type&       getType()       const {
        return type_;
    }

private:
    SEMBA::Argument::Object::Type type_;
    std::string str_;
};

} /* namespace Object */
} /* namespace Error */
} /* namespace Objects */
} /* namespace SEMBA */

#include "Object.hpp"

#endif /* SEMBA_ARGUMENT_OBJECT_H_ */
