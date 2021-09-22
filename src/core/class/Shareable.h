

#pragma once

#include <memory>

namespace SEMBA {
namespace Class {

class Shareable : public std::enable_shared_from_this<Shareable> {
public:
    Shareable() {}
    virtual ~Shareable() {}

    std::shared_ptr<Shareable> getSharedPtr() {
        try {
            return shared_from_this();
        } catch (const std::bad_weak_ptr&) {
            return std::shared_ptr<Shareable>(this);
        }
    }
    std::shared_ptr<const Shareable> getSharedPtr() const {
        try {
            return shared_from_this();
        } catch (const std::bad_weak_ptr&) {
            return std::shared_ptr<const Shareable>(this);
        }
    }

};

} /* namespace Class */
} /* namespace SEMBA */

