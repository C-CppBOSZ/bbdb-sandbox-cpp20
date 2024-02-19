//
// Created by bogusz on 05.02.24.
//

#include "PtrGuard.h"

#include <iostream>

namespace bsdb::src_module::guard {
    PtrGuard::PtrGuard(std::vector<unsigned long> *ptrs,const unsigned long &ptr): ptrs(ptrs) {
        ptrs->push_back(ptr);
        it=--ptrs->end();
    }

    PtrGuard::~PtrGuard() {
        ptrs->erase(it);
    }

    unsigned long PtrGuard::get() const {
        return *it;
    }

} // bsdb