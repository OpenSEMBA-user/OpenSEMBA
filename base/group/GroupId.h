#ifndef GROUPID_H_
#define GROUPID_H_

#include <iostream>
#include <map>
#include <sstream>
using namespace std;

#include "base/error/Error.h"

#include "Group.h"

template<typename T, class Id>
class GroupId : public virtual Group<T> {
public:
    class ErrorId : public virtual Error {
    public:
        ErrorId(const Id&);
        virtual ~ErrorId() throw();

        Id getId() const;
    private:
        Id id_;
    };
    class ErrorIdNotExists : public ErrorId {
    public:
        ErrorIdNotExists(const Id&);
        virtual ~ErrorIdNotExists() throw();
    };
    class ErrorIdZero : public ErrorId {
    public:
        ErrorIdZero(const Id&);
        virtual ~ErrorIdZero() throw();
    };
    class ErrorIdDuplicated : public ErrorId {
    public:
        ErrorIdDuplicated(const Id&);
        virtual ~ErrorIdDuplicated() throw();
    };

    USE_GROUP_CONSTRUCTS(GroupId, T);

    ClassBase* clone() const;

    USE_GROUP_ASSIGN(T);

    USE_GROUP_GETGROUPWITH(T);
    GroupId<T, Id> getGroupWith(const Id&);
    GroupId<T, Id> getGroupWith(const vector<Id>&);
    GroupId<const T, Id> getGroupWith(const Id&) const;
    GroupId<const T, Id> getGroupWith(const vector<Id>&) const;
    GroupId<T, Id> getGroupWithout(const Id&);
    GroupId<T, Id> getGroupWithout(const vector<Id>&);
    GroupId<const T, Id> getGroupWithout(const Id&) const;
    GroupId<const T, Id> getGroupWithout(const vector<Id>&) const;

    vector<Id> getIds() const;

    bool existId(const Id id) const;

    USE_GROUP_GET(T);
    T*       get(const Id id);
    const T* get(const Id id) const;

    template<typename T2>
    T*         add(T2* newElem , bool newId = false);
    template<typename T2>
    vector<T*> add(vector<T2*>&, bool newId = false);
    template<typename T2>
    vector<T*> add(Group<T2>& rhs);
    template<typename T2>
    vector<T*> add(const Group<T2>& rhs);
    template<typename T2>
    vector<T*> add(Group<T2>&&);

    void remove(const UInt elem);
    void remove(const vector<UInt>& elems);
    void remove(const Id);
    void remove(const vector<Id>&);

protected:
    void construct();
    void destruct ();

    vector<T*> preprocess (const vector<T*>& v) const;
    void       postprocess(const UInt i);

private:
    Id lastId_;
    map<Id, UInt> mapId_;

    vector<UInt> getElemsWith_(const vector<Id>&) const;
};

#include "GroupId.hpp"

#define USE_GROUPID_GETGROUPWITH(T, ID)                                 \
    Group<T> getGroupWith(const UInt elems) {                           \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<T> getGroupWith(const vector<UInt>& elems) {                  \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<const T> getGroupWith(const UInt elems) const {               \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<const T> getGroupWith(const vector<UInt>& elems) const {      \
        return Group<T>::getGroupWith(elems);                           \
    }                                                                   \
    Group<T> getGroupWithout(const UInt elems) {                        \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<T> getGroupWithout(const vector<UInt>& elems) {               \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<const T> getGroupWithout(const UInt elems) const {            \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    Group<const T> getGroupWithout(const vector<UInt>& elems) const {   \
        return Group<T>::getGroupWithout(elems);                        \
    }                                                                   \
    GroupId<T, ID> getGroupWith(const ID& ids) {                        \
        return GroupId<T,ID>::getGroupWith(ids);                        \
    }                                                                   \
    GroupId<T, ID> getGroupWith(const vector<ID>& ids) {                \
        return GroupId<T,ID>::getGroupWith(ids);                        \
    }                                                                   \
    GroupId<const T, ID> getGroupWith(const ID& ids) const {            \
        return GroupId<T,ID>::getGroupWith(ids);                        \
    }                                                                   \
    GroupId<const T, ID> getGroupWith(const vector<ID>& ids) const {    \
        return GroupId<T,ID>::getGroupWith(ids);                        \
    }                                                                   \
    GroupId<T, ID> getGroupWithout(const ID& ids) {                     \
        return GroupId<T,ID>::getGroupWithout(ids);                     \
    }                                                                   \
    GroupId<T, ID> getGroupWithout(const vector<ID>& ids) {             \
        return GroupId<T,ID>::getGroupWithout(ids);                     \
    }                                                                   \
    GroupId<const T, ID> getGroupWithout(const ID& ids) const {         \
        return GroupId<T,ID>::getGroupWithout(ids);                     \
    }                                                                   \
    GroupId<const T, ID> getGroupWithout(const vector<ID>& ids) const { \
        return GroupId<T,ID>::getGroupWithout(ids);                     \
    }

#define USE_GROUPID_GET(T, ID)          \
    T* get(const UInt i) {              \
        return Group<T>::get(i);        \
    }                                   \
    const T* get(const UInt i) const {  \
        return Group<T>::get(i);        \
    }                                   \
    T* get(const ID id) {               \
        return GroupId<T,ID>::get(id);  \
    }                                   \
    const T* get(const ID id) const {   \
        return GroupId<T,ID>::get(id);  \
    }

#define USE_GROUPID_ADD(T, ID)                                  \
    template<typename T2>                                       \
    T* add(T2* newElem, bool newId = false) {                   \
        return GroupId<T,ID>::add(newElem, newId);              \
    }                                                           \
    template<typename T2>                                       \
    vector<T*> add(vector<T2*>& elems, bool newId = false) {    \
        return GroupId<T,ID>::add(elems, newId);                \
    }                                                           \
    template<typename T2>                                       \
    vector<T*> add(Group<T2>& rhs) {                            \
        return GroupId<T,ID>::add(rhs);                         \
    }                                                           \
    template<typename T2>                                       \
    vector<T*> add(const Group<T2>& rhs) {                      \
        return GroupId<T,ID>::add(rhs);                         \
    }                                                           \
    template<typename T2>                                       \
    vector<T*> add(Group<T2>&& rhs) {                           \
        return GroupId<T,ID>::add(std::move(rhs));              \
    }

#define USE_GROUPID_REMOVE(T,ID)                \
    void remove(const UInt elem) {              \
        GroupId<T,ID>::remove(elem);            \
    }                                           \
    void remove(const vector<UInt>& elems) {    \
        GroupId<T,ID>::remove(elems);           \
    }                                           \
    void remove(const ID id) {                  \
        GroupId<T,ID>::remove(id);              \
    }                                           \
    void remove(const vector<ID>& ids) {        \
        GroupId<T,ID>::remove(ids);             \
    }

#endif /* GROUPID_H_ */
