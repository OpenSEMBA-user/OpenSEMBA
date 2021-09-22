

#pragma once

#include <exception>
#include <map>

#include "Group.h"

namespace SEMBA {
namespace Group {

template<typename T, class Id>
class Identifiable : public Group<T> {
public:
    Identifiable();
    template<typename T2>
    Identifiable(T2*);
    template<typename T2>
    Identifiable(const std::vector<T2*>&);
    template<typename T2>
    Identifiable(Group<T2>&);
    template<typename T2>
    Identifiable(const Group<T2>&);
    Identifiable(Group<T>&);
    template<typename T2>
    Identifiable(Group<T2>&&);
    Identifiable(Group<T>&&);
    virtual ~Identifiable();

    Identifiable<T,Id>& operator=(Group<T>&);
    Identifiable<T,Id>& operator=(Group<T>&&);

    virtual void clear();

    bool existId(const Id id) const;

    std::vector<Id> getIds() const;

    virtual T*       getId(const Id id);
    virtual const T* getId(const Id id) const;
    virtual Identifiable<T,Id>       getId(const std::vector<Id>&);
    virtual Identifiable<const T,Id> getId(const std::vector<Id>&) const;

    using Group<T>::add;
    Group<T> add(Group<T>&);
    Group<T> add(Group<T>&&);

    template<typename T2>
    Group<T> addId(T2*);
    template<typename T2>
    Group<T> addId(const std::vector<T2*>&);
    template<typename T2>
    Group<T> addId(Group<T2>&);
    Group<T> addId(Group<T>&);
#ifdef __GNUC__
    Group<T> addId(Group<T>&&);
#endif

    virtual void remove(const std::size_t&);
    virtual void remove(const std::vector<std::size_t>&);

    virtual void removeId(const Id);
    virtual void removeId(const std::vector<Id>&);

private:
    Id lastId_;
    std::map<Id, std::size_t> mapId_;

    void postprocess_(const std::size_t& pos);
    std::vector<std::size_t> getElemsId_(const std::vector<Id>&) const;
};

} /* namespace Group */
} /* namespace SEMBA */

#include "Identifiable.hpp"

