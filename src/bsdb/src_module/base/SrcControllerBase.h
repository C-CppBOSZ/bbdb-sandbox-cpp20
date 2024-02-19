//
// Created by bogusz on 19.02.24.
//

#ifndef SRCCONTROLLERBASE_H
#define SRCCONTROLLERBASE_H
#include "SrcProviderBase.h"

namespace bsdb::src_module::base {

    template <typename Derived>
class SrcControllerBase : virtual public SrcProviderBase<Derived>{



};

} // bsdb

#endif //SRCCONTROLLERBASE_H
