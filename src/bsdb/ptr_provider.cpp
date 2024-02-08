//
// Created by bogusz on 05.02.24.
//

#include "ptr_provider.h"

#include <iostream>

namespace bsdb {
    ptr_provider::ptr_provider(std::vector<unsigned long> *ptrs,const unsigned long &ptr): ptrs(ptrs) {
        ptrs->push_back(ptr);
        it=--ptrs->end();
        std::cout << "con ptr_provider: ptr=" <<  ptr << " it=" << *it << "\n";
    }

    ptr_provider::~ptr_provider() {
        ptrs->erase(it);
    }

    unsigned long ptr_provider::get() const {
        return *it;
    }

} // bsdb