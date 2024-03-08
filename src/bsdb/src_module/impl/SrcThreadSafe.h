//
// Created by bogusz on 06.03.24.
//

#ifndef SRCTHREADSAFE_H
#define SRCTHREADSAFE_H

#include "../base/SrcProviderBase.h"

namespace bbdb::src_module::impl {
    template<typename Derived>
    class SrcThreadSafe : virtual public base::SrcProviderBase<SrcThreadSafe<Derived>> {
    public:
        unsigned long get_ptr() override;

        guard::PtrGuard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const long &shift) override;

        unsigned long get_size() override;

        void shift_left_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
            const unsigned long &size_buffer) override;

        void shift_left_content(const unsigned long &content_size, const unsigned long &size_buffer) override;

        void shift_right_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
            const unsigned long &size_buffer) override;

        void shift_right_content(const unsigned long &content_size, const unsigned long &size_buffer) override;

        void delete_ptr_content(const unsigned long &ptr, const long &content_size) override;

        void delete_content(const long &content_size) override;

        void delete_ptr_to_ptr(const unsigned long &ptr_start, const unsigned long &ptr_end) override;

        void delete_to_ptr(const unsigned long &ptr_end) override;


    };
} // bbdb

#endif //SRCTHREADSAFE_H
