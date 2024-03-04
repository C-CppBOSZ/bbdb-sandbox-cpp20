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

    template<typename Derived>
    class SrcProviderBase {
    public:
        virtual ~SrcProviderBase() = default;

        /**
         * functions do not modify the pointer
         */
        virtual unsigned long get_ptr() = 0;

        /**
         * functions do not modify the pointer
         */
        virtual guard::PtrGuard push_ptr() = 0;

        virtual void set_ptr(const unsigned long &ptr) = 0;

        virtual void ptr_to_end() = 0;

        // TODO virtual ?
        void shift_ptr(const unsigned long &shift) = delete;

        virtual void shift_ptr(const long &shift) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual unsigned long get_size() = 0;

        // TODO move i swap
        // TODO todo first

        /**
         * functions do not modify the pointer
         */
        virtual void shift_left_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                                   const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_left_ptr_content_lazy(const unsigned long &ptr, const unsigned long &content_size,
                                                   const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_left_content(const unsigned long &content_size,
                                               const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_left_content_lazy(const unsigned long &content_size,
                                               const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_right_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                                    const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_right_ptr_content_lazy(const unsigned long &ptr, const unsigned long &content_size,
                                                    const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_right_content(const unsigned long &content_size,
                                                const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void shift_right_content_lazy(const unsigned long &content_size,
                                                const unsigned long &size_buffer = 4096) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_ptr_content(const unsigned long &ptr, const long &content_size) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_ptr_content_lazy(const unsigned long &ptr, const long &content_size) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_content(const long &content_size) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_content_lazy(const long &content_size) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_ptr_to_ptr(const unsigned long &ptr_start, const unsigned long &ptr_end) = 0;

        /**
         * functions do not modify the pointer
         */
        virtual void delete_ptr_to_ptr_lazy(const unsigned long &ptr_start, const unsigned long &ptr_end) = 0;

        /**
        * functions do not modify the pointer
        */
        virtual void delete_to_ptr(const unsigned long &ptr_end) = 0;

        /**
        * functions do not modify the pointer
        */
        virtual void delete_to_ptr_lazy(const unsigned long &ptr_end) = 0;

        // TODO std::condition_variable::notify_all
        virtual void notify_daemon() = 0;

        /**
         * functions do not modify the pointer
         * @return size write
         */
        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_obj(args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size write
         */
        template<typename... Args>
        void write_obj_lazy(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_obj_lazy(args...);
        };

        /**
         * functions do not modify the pointer
         * @return size write
         */
        template<typename... Args>
        unsigned int write_obj_ptr(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_obj_ptr(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size write
         */
        template<typename... Args>
        void write_obj_ptr_lazy(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_obj_ptr_lazy(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * @return size write
         */
        template<contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_container(args...);
        };


        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size write
         */
        template<contaner_out... Args>
        void write_contaner_lazy(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_contaner_lazy(args...);
        };

        /**
         * functions do not modify the pointer
         * @return size write
         */
        template<contaner_out... Args>
        unsigned int write_contaner_ptr(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_contaner_ptr(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size write
         */
        template<contaner_out... Args>
        void write_contaner_ptr_lazy(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template write_contaner_ptr_lazy(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * @return size read
         */
        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            return dynamic_cast<Derived *>(this)->template read_obj(args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size read
         */
        template<typename... Args>
        void read_obj_lazy(Args &... args) {
            return dynamic_cast<Derived *>(this)->template read_obj_lazy(args...);
        };

        /**
         * functions do not modify the pointer
         * @return size read
         */
        template<typename... Args>
        unsigned int read_obj_ptr(const unsigned long &ptr, Args &... args) {
            return dynamic_cast<Derived *>(this)->template read_obj_ptr(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size read
         */
        template<typename... Args>
        void read_obj_ptr_lazy(const unsigned long &ptr, Args &... args) {
            return dynamic_cast<Derived *>(this)->template read_obj_ptr_lazy(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * @return size insert
         */
        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_obj(args...);
        };

        /**
        * functions do not modify the pointer
        * caused by a daemon
        * @return size insert
        */
        template<typename... Args>
        void insert_obj_lazy(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_obj_lazy(args...);
        };

        /**
         * functions do not modify the pointer
         * @return size insert
         */
        template<typename... Args>
        unsigned int insert_obj_ptr(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_obj_ptr(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size insert
         */
        template<typename... Args>
        void insert_obj_ptr_lazy(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_obj_ptr_lazy(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * @return size insert
         */
        template<contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_container(args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size insert
         */
        template<contaner_out... Args>
        void insert_contaner_lazy(const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_contaner_lazy(args...);
        };

        /**
         * functions do not modify the pointer
         * @return size insert
         */
        template<contaner_out... Args>
        unsigned int insert_contaner_ptr(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_contaner_ptr(ptr, args...);
        };

        /**
         * functions do not modify the pointer
         * caused by a daemon
         * @return size insert
         */
        template<contaner_out... Args>
        void insert_contaner_ptr_lazy(const unsigned long &ptr, const Args &... args) {
            return dynamic_cast<Derived *>(this)->template insert_contaner_ptr_lazy(ptr, args...);
        };
    };
} // bbdb

#endif //SRC_PROVIDER_H
