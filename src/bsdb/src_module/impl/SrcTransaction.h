//
// Created by bogusz on 19.02.24.
//

#ifndef SRCCONTROLLER_H
#define SRCCONTROLLER_H
#include "../base/SrcTransactionBase.h"

namespace bbdb::src_module::impl {
    template<typename Derived>
    class SrcTransaction : virtual public base::SrcTransactionBase<Derived> {
        base::SrcProviderBase<Derived> *src_;
        mutable std::mutex mutex_;

    public:
        explicit SrcTransaction(base::SrcProviderBase<Derived> *src)
            : src_(src) {
        }

        ~SrcTransaction() {
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
            src_->set_ptr(ptr);
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
            src_->delete_n(ptr, n);
        };

        void delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) override {
            std::lock_guard lg(mutex_);
            src_->delete_(ptr_start, ptr_end);
        };

        void lazy_delete_n(const unsigned long &ptr, const long &n) override {
            std::lock_guard lg(mutex_);
            src_->lazy_delete_n(ptr, n);
        };

        void lazy_delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) override {
            std::lock_guard lg(mutex_);
            src_->lazy_delete_(ptr_start, ptr_end);
        }

        guard::SrcQueryGuard<Derived> src_transaction() override {
            return guard::SrcQueryGuard<Derived>(mutex_, *src_);
        };

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            std::lock_guard lg(mutex_);
            return src_->write_obj(args...);
        };

        template<base::contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            std::lock_guard lg(mutex_);
            return src_->write_contaner(args...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            std::lock_guard lg(mutex_);
            return src_->read_obj(args...);
        };

        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            std::lock_guard lg(mutex_);
            return src_->insert_obj(args...);
        };

        template<base::contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            std::lock_guard lg(mutex_);
            return src_->insert_contaner(args...);
        };
    };
}

#endif //SRCCONTROLLER_H
