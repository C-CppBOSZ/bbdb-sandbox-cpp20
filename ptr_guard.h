//
// Created by bogusz on 31.01.24.
//

#ifndef PTR_GUARD_H
#define PTR_GUARD_H
#include "src_provider.h"

namespace bsdb {
template<typename Derived>
class ptr_guard {
private:
    unsigned long ptr_;
    src_provider<Derived> &provider_;

public:
    explicit ptr_guard(bsdb::src_provider<Derived> &provider): ptr_(provider.get_ptr()), provider_(provider) {
    }

    unsigned long ptr() const {
        return ptr_;
    }

    operator unsigned long() const {
        return ptr_;
    }

    ptr_guard & operator=(const ptr_guard &other) = delete;

    ptr_guard & operator=(ptr_guard &&other) noexcept = delete;

    void ptr_return() {
        provider_.set_ptr(ptr_);
    }

    ~ptr_guard() {
        provider_.set_ptr(ptr_);
    };
};

} // bsdb

#endif //PTR_GUARD_H
