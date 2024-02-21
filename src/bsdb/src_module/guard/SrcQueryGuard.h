//
// Created by bogusz on 20.02.24.
//

#ifndef SRCQUERYGUARD_H
#define SRCQUERYGUARD_H

#include "../base/SrcProviderBase.h"

namespace bbdb::src_module::guard {
    template<typename Func, typename Derived>
    concept FuncQuery = requires(Func func, base::SrcProviderBase<Derived> &s)
    {
        { func(s) };
    };

    template<typename Derived>
    class SrcQueryGuard {
        std::unique_lock<std::mutex> lg_;
        base::SrcProviderBase<Derived>& src_;

    public:
        explicit SrcQueryGuard(std::mutex & mutex,base::SrcProviderBase<Derived> &src)
            : lg_(mutex) ,src_(src) {
        }

        template<FuncQuery<Derived> Func>
        SrcQueryGuard<Derived> &fun(Func func) {
            func(src_);
            return *this;
        };
    };
} // bbdb

#endif //SRCQUERYGUARD_H
