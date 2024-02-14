//
// Created by bogusz on 05.02.24.
//

#ifndef PTR_PROVIDER_H
#define PTR_PROVIDER_H
#include <vector>

namespace bsdb {

class ptr_guard {
protected:
    std::vector<unsigned long>::iterator it;
    std::vector<unsigned long>* ptrs;
public:
    explicit ptr_guard(std::vector<unsigned long> *ptrs,const unsigned long &ptr);
    ~ptr_guard();
    unsigned long get() const;
};

} // bsdb

#endif //PTR_PROVIDER_H
