#pragma once

#include <memory>
#include <set>
#include <iterator>

#include "Error.h"

namespace SEMBA {

template<typename T>
class IdentifiableUnique {
public:
    typedef decltype(std::declval<T>().getId()) Id;
    
    struct idComparator {
        using is_transparent = std::true_type;

        bool operator() (const std::unique_ptr<T>& a, const std::unique_ptr<T>& b) const {
            return a->getId() < b->getId();
        }

        bool operator() (const Id& a, const std::unique_ptr<T>& b) const {
            return a < b->getId();
        }

        bool operator() (const std::unique_ptr<T>& a, const Id& b) const {
            return a->getId() < b;
        }
    };

    typedef std::set<std::unique_ptr<T>, idComparator> Container;

    IdentifiableUnique() = default;
    IdentifiableUnique(const IdentifiableUnique<T>&);
    
    virtual ~IdentifiableUnique() = default;

    IdentifiableUnique<T>& operator=(const IdentifiableUnique<T>&);
    
    typename Container::iterator begin() { return items_.begin(); }
    typename Container::iterator end() { return items_.end(); }
    typename Container::const_iterator begin() const { return items_.begin(); }
    typename Container::const_iterator end() const { return items_.end(); }

    typename Container::iterator getId(const Id& id);
    typename Container::const_iterator getId(const Id& id) const;

    bool existId(const Id& id) const { return items_.count(id) != 0; }
    
    void add(const std::unique_ptr<T>& item);
    void transfer(std::unique_ptr<T>&& item);

    std::size_t size() const { return items_.size(); }

private:
    Container items_;
};

template<typename T>
IdentifiableUnique<T>::IdentifiableUnique(const IdentifiableUnique<T>& rhs)
{
    for (auto const& elem : rhs) {
        add(std::make_unique<T>(*elem));
    }
}

template<typename T>
IdentifiableUnique<T>& IdentifiableUnique<T>::operator=(const IdentifiableUnique<T>& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    for (auto const & elem : rhs) {
        add(elem);
    }
    return *this;
}

template<typename T>
typename IdentifiableUnique<T>::Container::iterator
IdentifiableUnique<T>::getId(const IdentifiableUnique<T>::Id& id)
{
    auto it = items_.find(id);
    if (it != end()) {
        return it;
    }
    else {
        throw typename Error::Id::NotExists<Id>(id);
    }
}

template<typename T>
typename IdentifiableUnique<T>::Container::const_iterator 
IdentifiableUnique<T>::getId(const IdentifiableUnique<T>::Id& id) const 
{
    auto it = items_.find(id);
    if (it != end()) {
        return it;
    }
    else {
        throw typename Group::Error::Id::NotExists<Id>(id);
    }
}

template<typename T>
void IdentifiableUnique<T>::add(const std::unique_ptr<T>& elem)
{
    if ( !items_.insert(std::make_unique<T>(*elem)).second ) {
        throw Group::Error::Id::Duplicated<Id>(elem->getId());
    }
}

template<typename T>
void IdentifiableUnique<T>::transfer(std::unique_ptr<T>&& elem)
{
    if (!items_.insert(std::move(elem)).second) {
        throw Group::Error::Id::Duplicated<Id>(elem->getId());
    }
}

}
