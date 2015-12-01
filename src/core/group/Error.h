
#ifndef SEMBA_GROUP_ERROR_H_
#define SEMBA_GROUP_ERROR_H_

#include <exception>
#include <sstream>
#include <string>

namespace SEMBA {
namespace Group {
namespace Error {
namespace Id {

template <typename Identification>
class Base : public std::exception {
public:
    Base(const Identification& id) : id_(id) {}
    virtual ~Base() throw() {}

    Identification getId() const { return id_; }
private:
    Identification id_;
};

template <typename Identification>
class NotExists : public Base<Identification> {
public:
    NotExists(const Identification& id)
    :   Base<Identification>(id) {
        str_ = std::string("Group: Element with id ") +
               this->getId().toStr() +
               std::string(" not exists");
    }
    virtual ~NotExists() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

template <typename Identification>
class Zero : public Base<Identification> {
public:
    Zero() : Base<Identification>(Identification(0)) {}
    virtual ~Zero() throw() {}

    const char* what() const throw() {
        return "Group: Element with id 0";
    }
};

template <typename Identification>
class Duplicated : public Base<Identification> {
public:
    Duplicated(const Identification& id)
    :   Base<Identification>(id) {
        str_ = std::string("Group: Duplicated Id ") +
               this->getId().toStr();
    }
    virtual ~Duplicated() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Id */
} /* namespace Error */
} /* namespace Group */
} /* namespace SEMBA */

#endif /* SEMBA_GROUP_ERROR_H_ */
