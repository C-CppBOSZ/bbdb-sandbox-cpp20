//
// Created by bogusz on 19.02.24.
//

#ifndef SRCCONTROLLERBASE_H
#define SRCCONTROLLERBASE_H
#include "SrcProviderBase.h"
#include "../guard/SrcQueryGuard.h"

namespace bbdb::src_module::base {
    template<typename Derived>
    class SrcControllerBase : virtual public SrcProviderBase<SrcControllerBase<Derived>> {
    public:
        virtual ~SrcControllerBase() = default;

        virtual guard::SrcQueryGuard<Derived> src_transaction() = 0;
    };
} // bbdb

#endif //SRCCONTROLLERBASE_H
