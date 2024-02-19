//
// Created by bogusz on 31.01.24.
//

#ifndef PTR_GUARD_H
#define PTR_GUARD_H
#include "../base/SrcProviderBase.h"

namespace bsdb::src_module::guard {
template<typename Derived>
class [[deprecated("unsafe thread")]] IndexPtrGuard {
private:
    unsigned long ptr_;
    base::SrcProviderBase<Derived> &provider_;

public:
    explicit IndexPtrGuard(base::SrcProviderBase<Derived> &provider): ptr_(provider.get_ptr()), provider_(provider) {
    }

    unsigned long ptr() const {
        return ptr_;
    }

    operator unsigned long() const {
        return ptr_;
    }

    IndexPtrGuard & operator=(const IndexPtrGuard &other) = delete;

    IndexPtrGuard & operator=(IndexPtrGuard &&other) noexcept = delete;

    void ptr_return() {
        provider_.set_ptr(ptr_);
    }

    ~IndexPtrGuard() {
        provider_.set_ptr(ptr_);
    };
};

} // bsdb

#endif //PTR_GUARD_H
