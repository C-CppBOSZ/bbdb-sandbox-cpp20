//
// Created by bogusz on 19.02.24.
//

#ifndef SRCQUERY_H
#define SRCQUERY_H
#include <mutex>

#include "../base/SrcQueryBase.h"


namespace bsdb::src_module::provider {
    template<typename Derived>
    class SrcQuery : base::SrcQueryBase<SrcQuery<Derived>, Derived> {
        std::unique_lock<std::mutex> lg_;
        base::SrcProviderBase<Derived>& src_;

    public:
        explicit SrcQuery(std::mutex &mutex,base::SrcProviderBase<Derived>& src)
            : lg_(mutex) ,src_(src){}

        template<base::FuncQuery<Derived> Func>
        base::SrcQueryBase<SrcQuery<Derived>, Derived> &fun(Func func) {



        };
    };
}

#endif //SRCQUERY_H
