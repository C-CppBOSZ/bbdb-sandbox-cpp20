//
// Created by bogusz on 19.02.24.
//

#ifndef SRCQUERYBASE_H
#define SRCQUERYBASE_H
#include "SrcProviderBase.h"

namespace bsdb::src_module::base {

    template<typename Func,typename Derived>
    concept FuncQuery = requires(Func func, SrcProviderBase<Derived> s) {
        { func(s) };
    };

    template<typename DerivedQuery,typename Derived>
    class SrcQueryBase<DerivedQuery> {
    public:
        virtual ~SrcQueryBase() = default;

        template<FuncQuery<Derived> Func>
        SrcQueryBase<DerivedQuery,Derived> & fun(Func func) {
            return dynamic_cast<DerivedQuery*>(this)->template fun(func);
        };


    };
} // bsdb::src_module::base

#endif //SRCQUERYBASE_H
