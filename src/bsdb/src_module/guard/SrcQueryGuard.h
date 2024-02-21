//
// Created by bogusz on 20.02.24.
//

#ifndef SRCQUERYGUARD_H
#define SRCQUERYGUARD_H

#include "../base/SrcProviderBase.h"

namespace bbdb::src_module::guard {
    template<typename Func, typename Derived,typename... OtherArgs>
    concept FuncQuery = requires(Func func, base::SrcProviderBase<Derived> &s,OtherArgs... other_args)
    {
        { func(s,other_args...) };
    };

    template<typename Func, typename Derived,typename ReturnType, typename... OtherArgs>
   concept FactoryQuery = requires(Func func, base::SrcProviderBase<Derived> &s,OtherArgs... other_args)
    {
        { func(s,other_args...) } -> std::convertible_to<ReturnType>;
    };

    template<typename Derived>
    class SrcQueryGuard {
        std::unique_lock<std::mutex> lg_;
        base::SrcProviderBase<Derived>& src_;

    public:
        explicit SrcQueryGuard(std::mutex & mutex,base::SrcProviderBase<Derived> &src)
            : lg_(mutex) ,src_(src) {
        }

        template<typename... OtherArgs,FuncQuery<Derived,OtherArgs...> Func>
        SrcQueryGuard<Derived> &function(Func func,OtherArgs... other_args) {
            func(src_,other_args...);
            return *this;
        };

        template<typename ReturnType,typename... OtherArgs,FactoryQuery<Derived,ReturnType,OtherArgs...> Func>
        SrcQueryGuard<Derived> &factory(ReturnType & return_,Func func,OtherArgs... other_args) {
            return_ = func(src_,other_args...);
            return *this;
        };
    };
} // bbdb

#endif //SRCQUERYGUARD_H
