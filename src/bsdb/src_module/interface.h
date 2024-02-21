//
// Created by bogusz on 21.02.24.
//

#ifndef INTERFACE_H
#define INTERFACE_H
#include "base/SrcProviderBase.h"

namespace bbdb {
    template <typename Derived>
    typedef src_module::base::SrcProviderBase<Derived> SrcBase;

    namespace src_module {

    }
}

#endif //INTERFACE_H
