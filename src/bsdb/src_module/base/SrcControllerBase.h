//
// Created by bogusz on 19.02.24.
//

#ifndef SRCCONTROLLERBASE_H
#define SRCCONTROLLERBASE_H
#include "SrcProviderBase.h"
#include "SrcQueryBase.h"

namespace bsdb::src_module::base {
    template<typename DerivedQuery,typename Derived>
    class SrcControllerBase : virtual public SrcProviderBase<Derived> {
    public:
        virtual ~SrcControllerBase() = default;

        virtual SrcQueryBase<DerivedQuery,Derived> src_transaction() = 0;
    };
} // bsdb

#endif //SRCCONTROLLERBASE_H
