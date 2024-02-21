//
// Created by bogusz on 08.01.24.
//

#ifndef SRC_PROVIDER_H
#define SRC_PROVIDER_H
#include <cstddef>
#include <iostream>
#include <mutex>
#include <utility>

#include "../guard/PtrGuard.h"

namespace bbdb::src_module::base {
    template<typename T>
    concept contaner_out = requires(T a)
    {
        { a.size() } -> std::same_as<std::size_t>;
        { a.data() } -> std::same_as<typename T::value_type *>;
        { a.at(std::declval<std::size_t>()) } -> std::same_as<typename T::reference>;
    };

    template <typename Derived>
    class SrcProviderBase {
    public:
        virtual ~SrcProviderBase() = default;

        virtual unsigned long get_ptr() = 0;

        virtual guard::PtrGuard push_ptr() = 0;

        virtual void set_ptr(const unsigned long &ptr) = 0;

        virtual void ptr_to_end() = 0;

        virtual void shift_ptr(const long &shift) = 0;

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template write_obj(args...);
        };

        template<contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template write_container(args...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            return dynamic_cast<Derived*>(this)->template read_obj(args...);
        };

        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template insert_obj(args...);
        };

        template<contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template insert_container(args...);
        };

        virtual void delete_n(const unsigned long &ptr,const long &n) = 0;
        virtual void delete_(const unsigned long &ptr_start,const unsigned long &ptr_end) = 0;

        virtual void lazy_delete_n(const unsigned long &ptr,const long &n) = 0;
        virtual void lazy_delete_(const unsigned long &ptr_start,const unsigned long &ptr_end) = 0;
    };




} // bbdb

#endif //SRC_PROVIDER_H
