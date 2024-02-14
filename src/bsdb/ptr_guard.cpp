//
// Created by bogusz on 05.02.24.
//

#include "ptr_guard.h"

#include <iostream>

namespace bsdb {
    ptr_guard::ptr_guard(std::vector<unsigned long> *ptrs,const unsigned long &ptr): ptrs(ptrs) {
        ptrs->push_back(ptr);
        it=--ptrs->end();
    }

    ptr_guard::~ptr_guard() {
        ptrs->erase(it);
    }

    unsigned long ptr_guard::get() const {
        return *it;
    }

} // bsdb