//
// Created by bogusz on 19.02.24.
//

#ifndef SRCCONTROLLER_H
#define SRCCONTROLLER_H
#include "SrcQuery.h"
#include "../base/SrcControllerBase.h"

namespace bsdb::src_module::provider {
    template<typename DerivedQuery,typename Derived>
    class SrcController : virtual public base::SrcControllerBase<DerivedQuery,Derived>{
        base::SrcProviderBase<Derived> * src_;
        mutable std::mutex mutex_;

    public:
        explicit SrcController(base::SrcProviderBase<Derived> *src)
            : src_(src) {
        }

        ~SrcController() {
            std::lock_guard lg(mutex_);
        };

        unsigned long get_ptr() override {
            std::lock_guard lg(mutex_);
            return src_->get_ptr();
        };

        guard::PtrGuard push_ptr() override {
            std::lock_guard lg(mutex_);
            return src_->push_ptr();
        };

        void set_ptr(const unsigned long &ptr) override {
            std::lock_guard lg(mutex_);
            src_->set_ptr();
        };

        void ptr_to_end() override {
            std::lock_guard lg(mutex_);
            src_->ptr_to_end();
        };

        void shift_ptr(const long &shift) override {
            std::lock_guard lg(mutex_);
            src_->shift_ptr(shift);
        };

        void delete_n(const unsigned long &ptr, const long &n) override {
            std::lock_guard lg(mutex_);
            src_->delete_n(ptr,n);
        };

        void delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) override {
            std::lock_guard lg(mutex_);
            src_->delete_(ptr_start,ptr_end);
        };

        void lazy_delete_n(const unsigned long &ptr, const long &n) override {
            std::lock_guard lg(mutex_);
            src_->lazy_delete_n(ptr,n);
        };

        void lazy_delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) override {
            std::lock_guard lg(mutex_);
            src_->lazy_delete_(ptr_start,ptr_end);
        };

        base::SrcQueryBase<DerivedQuery,Derived> src_transaction() override {
            return provider::SrcQuery<DerivedQuery,Derived>(mutex_);
        };
    };
}

#endif //SRCCONTROLLER_H
