
#ifndef SEMBA_ARGUMENT_GROUP_H_
#define SEMBA_ARGUMENT_GROUP_H_

#include "GroupBase.h"

namespace SEMBA {
namespace Argument {

template <bool S = true>
class Group : public GroupBase {
    template <bool S2>
    friend class Group;
public:
    virtual ~Group();

    bool isRequired() const;
    bool isMutuallyExclusive() const { return !S; }

    Group& required();

    template <bool S2 = S, typename T = typename std::enable_if<S2>::type>
    Group<true >& addGroup(const std::string& = std::string(),
                           const std::string& = std::string());
    template <bool S2 = S, typename T = typename std::enable_if<S2>::type>
    Group<false>& addMutuallyExclusiveGroup();

protected:
    std::size_t numMutExc_;

    Group(Group<true>* = NULL,
          const std::string& = std::string(),
          const std::string& = std::string());

private:
    //Erased
    Group();
    Group(const Group&);
    Group(Group&&);

    Group& operator=(const Group&);
    Group& operator=(Group&&);
};

typedef Group<false> MutuallyExclusiveGroup;

} /* namespace Argument */
} /* namespace SEMBA */

#include "Group.hpp"

#endif /* SEMBA_ARGUMENT_GROUP_H_ */
