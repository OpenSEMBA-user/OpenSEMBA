#pragma once

#include <memory>
#include <set>
#include <iterator>

namespace SEMBA {
namespace Class {
namespace Group {

template<typename T>
class GroupIdentifiableUnique {
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

    GroupIdentifiableUnique() = default;
    GroupIdentifiableUnique(const GroupIdentifiableUnique<T>&);
    GroupIdentifiableUnique(GroupIdentifiableUnique<T>&&) noexcept = default;
    virtual ~GroupIdentifiableUnique() = default;

    GroupIdentifiableUnique<T>& operator=(const GroupIdentifiableUnique<T>&);
    GroupIdentifiableUnique<T>& operator=(GroupIdentifiableUnique<T>&&) = default;
    
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
    virtual iterator addAndAssignIds(const GroupIdentifiableUnique<T>& items);

    std::vector<const T*> get() const;

    void removeId(const Id& id);
    void remove(const std::vector<const T*>&);

    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }

    template<typename T2>
    bool emptyOf() const;
    template<typename T2>
    std::size_t sizeOf() const;
    template<typename T2>
    std::vector<const T2*> getOf() const;
    template<typename T2>
    std::vector<const T*> getByCastingToParent() const;

protected:

private:
    Container items_;
};

template<typename T>
GroupIdentifiableUnique<T>::GroupIdentifiableUnique(const GroupIdentifiableUnique<T>& rhs)
{
    for (auto const& it: rhs) {
        add(it);
    }
}

template<typename T>
GroupIdentifiableUnique<T>& GroupIdentifiableUnique<T>::operator=(const GroupIdentifiableUnique<T>& rhs)
{
    for (auto const& it: rhs) {
        add(it);
    }
    return *this;
}

template<typename T>
const T* GroupIdentifiableUnique<T>::getId(const GroupIdentifiableUnique<T>::Id& id) const
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
T* GroupIdentifiableUnique<T>::getId(const GroupIdentifiableUnique<T>::Id& id)
{
    auto it = GroupIdentifiableUnique<T>::findId(id);
    if (it != this->end()) {
        return it->get();
    }
    else {
        return nullptr;
    }
}

template<typename T> 
typename GroupIdentifiableUnique<T>::iterator
GroupIdentifiableUnique<T>::add(const std::unique_ptr<T>& elem)
{
    std::unique_ptr<T> res(dynamic_cast<T*>(elem->clone().release()));
    auto it = items_.insert(std::move(res));
    
    if (it.second) {
        return it.first;
    }
    else {
        throw std::logic_error("Group: Duplicated Id " + elem->getId().toStr());
    }

}

template<typename T> 
typename GroupIdentifiableUnique<T>::iterator
GroupIdentifiableUnique<T>::add(std::unique_ptr<T>&& elem)
{
    auto it = items_.insert(std::move(elem));
    if (it.second) {
        return it.first;
    }
    else {
        throw std::logic_error("Group: Duplicated Id " + elem->getId().toStr());
    }
}

template<typename T>
typename GroupIdentifiableUnique<T>::iterator
GroupIdentifiableUnique<T>::addAndAssignId(std::unique_ptr<T>&& elem)
{
    if (empty()) {
        elem->setId(Id(1));
    }
    else {
        elem->setId(++items_.rbegin()->get()->getId());
    }
    return add(std::move(elem));
}

template<typename T>
typename GroupIdentifiableUnique<T>::iterator
GroupIdentifiableUnique<T>::addAndAssignIds(const GroupIdentifiableUnique<T>& elems) {
	iterator it;

    for (const auto& elem : elems) {
        std::unique_ptr<T> res(dynamic_cast<T*>(elem->clone().release()));
        it = addAndAssignId(std::move(res));
    }

    return it;
}

template<typename T>
void GroupIdentifiableUnique<T>::removeId(const GroupIdentifiableUnique<T>::Id& id) {
    auto it = items_.find(id);
    items_.erase(it);
}

template<typename T>
void GroupIdentifiableUnique<T>::remove(const std::vector<const T*>& rhs) {
    for (auto const& it : rhs) {
        items_.erase(items_.find(it));
    }
}

template<typename T> template<typename T2>
std::vector<const T2*> GroupIdentifiableUnique<T>::getOf() const {    
    std::vector<const T2*> res;
    for (auto const& item : items_) {
        if (item->template is<T2>()) {
            res.push_back(item->template castTo<T2>());
        }   
    }
    return res;
}

template<typename T>
std::vector<const T*> GroupIdentifiableUnique<T>::get() const {
    std::vector<const T*> res;
    for (auto const& item : items_) {
        res.push_back(item.get());
    }
    return res;
}

template<typename T> template<typename T2>
std::vector<const T*> GroupIdentifiableUnique<T>::getByCastingToParent() const {
    std::vector<const T*> res;
    for (auto const& item : items_) {
        if (item->template is<T2>()) {
            res.push_back(item.get());
        }   
    }
    return res;
}

template<typename T> template<typename T2>
std::size_t GroupIdentifiableUnique<T>::sizeOf() const 
{
    std::size_t res = 0;
    for (auto const& item : items_) {
        if (item->template is<T2>()) {
            res++;
        }
    }
    return res;
}

template<typename T> template<class T2>
bool GroupIdentifiableUnique<T>::emptyOf() const 
{
    return this->sizeOf<T2>() == 0;
}

} // Namespace Group
} // Namespace Class
} // Namespace SEMBA
