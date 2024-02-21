//
// Created by bogusz on 05.02.24.
//

#ifndef PTR_PROVIDER_H
#define PTR_PROVIDER_H
#include <vector>

namespace bbdb::src_module::guard {

class PtrGuard {
protected:
    std::vector<unsigned long>::iterator it;
    std::vector<unsigned long>* ptrs;
public:
    explicit PtrGuard(std::vector<unsigned long> *ptrs,const unsigned long &ptr);
    ~PtrGuard();
    unsigned long get() const;
};

} // bbdb

#endif //PTR_PROVIDER_H
