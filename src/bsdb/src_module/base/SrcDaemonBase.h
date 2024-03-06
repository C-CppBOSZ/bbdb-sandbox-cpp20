//
// Created by bogusz on 05.03.24.
//

#ifndef SRCDAEMONBASE_H
#define SRCDAEMONBASE_H
#include "SrcProviderBase.h"

namespace bbdb::src_module::base {



    template<typename Derived>
    class SrcDaemonBase : virtual public SrcProviderBase<SrcDaemonBase<Derived>> {
    public:
        // TODO std::condition_variable::notify_all
        virtual void notify_daemon() = 0;

    };

} // bbdb

#endif //SRCDAEMONBASE_H
