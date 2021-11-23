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
    typedef typename std::set<std::unique_ptr<T>, idComparator>::iterator iterator;
    typedef typename std::set<std::unique_ptr<T>, idComparator>::const_iterator const_iterator;

    IdentifiableUnique() = default;
    IdentifiableUnique(const IdentifiableUnique<T>&);
    IdentifiableUnique(IdentifiableUnique<T>&&) noexcept = default;
    virtual ~IdentifiableUnique() = default;

    IdentifiableUnique<T>& operator=(const IdentifiableUnique<T>&);
    IdentifiableUnique<T>& operator=(IdentifiableUnique<T>&&) = default;
    
    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); }
    
    const_iterator begin() const { return items_.begin(); }
    const_iterator end() const { return items_.end(); }
    
    iterator findId(const Id& id) { return items_.find(id); };
    const_iterator findId(const Id& id) const { return items_.find(id); };

    T* getId(const Id& id);
    const T* getId(const Id& id) const;

    bool existId(const Id& id) const { return items_.count(id) != 0; }
    
    virtual iterator add(const std::unique_ptr<T>& item);
    virtual iterator add(std::unique_ptr<T>&& item);
virtual iterator addAndAssignId(std::unique_ptr<T>&& item);

std::size_t size() const { return items_.size(); }
bool empty() const { return items_.empty(); }

template<typename T2>
bool emptyOf() const;
template<typename T2>
std::size_t sizeOf() const;
template<typename T2>
std::vector<const T2*> getOf() const;
private:
    Container items_;
};

template<typename T>
IdentifiableUnique<T>::IdentifiableUnique(const IdentifiableUnique<T>& rhs)
{
    for (auto const& elem : rhs) {
        add(elem);
    }
}

template<typename T>
IdentifiableUnique<T>& IdentifiableUnique<T>::operator=(const IdentifiableUnique<T>& rhs)
{
    for (auto const& elem : rhs) {
        add(elem);
    }
    return *this;
}

template<typename T>
const T* IdentifiableUnique<T>::getId(const IdentifiableUnique<T>::Id& id) const
{
    auto it = findId(id);
    if (it != this->end()) {
        return it->get();
    }
    else {
        return nullptr;
    }
}

template<typename T>
T* IdentifiableUnique<T>::getId(const IdentifiableUnique<T>::Id& id)
{
    auto it = IdentifiableUnique<T>::findId(id);
    if (it != this->end()) {
        return it->get();
    }
    else {
        return nullptr;
    }
}

template<typename T>
typename IdentifiableUnique<T>::iterator
IdentifiableUnique<T>::add(const std::unique_ptr<T>& elem)
{
    auto it = items_.insert(std::make_unique<T>(*elem));
    if (it.second) {
        return it.first;
    }
    else {
        throw Group::Error::Id::Duplicated<Id>(elem->getId());
    }
}

template<typename T>
typename IdentifiableUnique<T>::iterator
IdentifiableUnique<T>::add(std::unique_ptr<T>&& elem)
{
    auto it = items_.insert(std::move(elem));
    if (it.second) {
        return it.first;
    }
    else {
        throw Group::Error::Id::Duplicated<Id>(elem->getId());
    }
}

template<typename T>
typename IdentifiableUnique<T>::iterator
IdentifiableUnique<T>::addAndAssignId(std::unique_ptr<T>&& elem)
{
    if (empty()) {
        elem->setId(Id(1));
    }
    else {
        elem->setId(++items_.rbegin()->get()->getId());
    }
    return add(std::move(elem));
}


template<typename T> template<typename T2>
std::vector<const T2*> IdentifiableUnique<T>::getOf() const {
    std::vector<const T2*> res;
    for (auto const& item : items_) {
        if (item->is<T2>()) {
            res.push_back(item->castTo<T2>());
        }   
    }
    return res;
}

template<typename T> template<typename T2>
std::size_t IdentifiableUnique<T>::sizeOf() const 
{
    std::size_t res = 0;
    for (auto const& item : items_) {
        if (item->is<T2>()) {
            res++;
        }
    }
    return res;
}

template<typename T> template<class T2>
bool IdentifiableUnique<T>::emptyOf() const 
{
    return this->sizeOf<T2>() == 0;
}


}
