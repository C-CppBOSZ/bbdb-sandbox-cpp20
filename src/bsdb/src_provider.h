//
// Created by bogusz on 08.01.24.
//

#ifndef SRC_PROVIDER_H
#define SRC_PROVIDER_H
#include <cstddef>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include "ptr_guard.h"

namespace bsdb {
    template<typename T>
    concept contaner_out = requires(T a)
    {
        { a.size() } -> std::same_as<std::size_t>;
        { a.data() } -> std::same_as<typename T::value_type *>;
        { a.at(std::declval<std::size_t>()) } -> std::same_as<typename T::reference>;
    };

    template <typename Derived>
    class src_provider {
    public:
        virtual ~src_provider() = default;

        virtual unsigned long get_ptr() = 0;

        virtual ptr_guard push_ptr() = 0;

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

    class src_provider_impl_file : virtual public src_provider<src_provider_impl_file> {
    private:
        std::fstream file_;
        mutable std::mutex mutex_;
        std::vector<unsigned long> ptrs_;

        void simple_shift_left_unsafe_thread(const unsigned long &ptr,const unsigned long &n,const unsigned long &size_buffer = 4096) {
            if(n == 0) return;
            char buffer[size_buffer];
            ptr_to_end(); // TODO ehhhhh deadlock
            unsigned long ptr_end = get_ptr();
            unsigned long size = ptr_end - ptr;
            unsigned int n_shift = size / size_buffer;
            set_ptr(ptr); // TODO ehhhhh deadlock

            file_.read(buffer,size_buffer);

        }

        void simple_shift_right_unsafe_thread(const unsigned long &ptr,const unsigned long &n,unsigned int size_buffer = 4096) {
            if(n == 0) return;
            if(n > size_buffer) return shift_right_n_unsafe_thread(ptr,n);
            if (shift_right_if_n_exceeds_file_size_unsafe_thread(ptr,n)) return;

            char buffer[size_buffer];

            unsigned long size = get_size_unsafe_thread() - (ptr + n);
            // unsigned int n_shift = size / size_buffer;
            set_ptr_unsafe_thread(ptr);

            while (!file_.eof()) {



            }

            file_.read(buffer,size_buffer);
        }

        void shift_right_n_unsafe_thread(const unsigned long &ptr,const unsigned long &n) { // TODO kiedy n jest wieksze od całego pliku
            if (shift_right_if_n_exceeds_file_size_unsafe_thread(ptr,n)) return;
            if(n == 0) return;
            char buffer[2][n];
            bool switch_buffer = false;
            unsigned long size = get_size_unsafe_thread();
            set_ptr_unsafe_thread(ptr);
            file_.read(buffer[switch_buffer],n);
            while (!file_.eof()) {
                if((static_cast<unsigned long>(file_.tellg()) + n) >= size) {
                    const unsigned long tmp = size - static_cast<unsigned long>(file_.tellg());
                    char tmp_buffer[tmp];
                    file_.read(tmp_buffer,tmp);
                    shift_ptr_unsafe_thread(-tmp);
                    file_.write(buffer[switch_buffer],n);
                    file_.write(tmp_buffer,tmp);
                    break;
                }
                file_.read(buffer[!switch_buffer],n);
                shift_ptr_unsafe_thread(-n);
                file_.write(buffer[switch_buffer],n);
                switch_buffer = !switch_buffer;
            }

        }
        bool shift_right_if_n_exceeds_file_size_unsafe_thread(const unsigned long &ptr,const unsigned long &n) {
            const unsigned long size = get_size_unsafe_thread();
            if (const unsigned long ptr_end = ptr + n; ptr_end > size) {
                unsigned long diff = ptr_end - size;
                char buffer[size];
                set_ptr_unsafe_thread(0);
                file_.read(buffer,size);
                constexpr char nullChar = '\0';
                file_.write(&nullChar, diff);
                file_.write(buffer,size);
                return true;
            }else
                return false;
        }
        unsigned long get_size_unsafe_thread() {
            const auto streampos = file_.tellg();
            file_.seekg(0, std::ios::end);
            const unsigned long tmp = file_.tellg();
            file_.seekg(streampos);
            return tmp;
        }

        void set_ptr_unsafe_thread(const unsigned long &ptr) {
            file_.seekg(ptr, std::ios::beg);
            file_.seekp(ptr, std::ios::beg);
        }

        void shift_ptr_unsafe_thread(const long &shift) {
            file_.seekp(shift, std::ios::cur);
            file_.seekg(file_.tellp()); // TODO najprawdopodobniej nie potrzebnr seekp przesówa też g
        }

        void ptr_to_end_unsafe_thread() {
            file_.seekg(0, std::ios::end);
            file_.seekp(0, std::ios::end);
        }

    public:
        explicit src_provider_impl_file(const std::string& path, std::ios_base::openmode mode);

        ~src_provider_impl_file() override;

        void open(const std::string& path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        ptr_guard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const long &shift) override;

        // template<typename... Args>
        // unsigned int write_obj(const Args &... args) {
        //     // if constexpr (sizeof...(Args) > 0) {
        //         (file_.write(reinterpret_cast<const char *>(args), sizeof(Args)), ...);
        //         return (sizeof(Args) + ...);
        //     // } else {
        //     //     return 0;
        //     // }
        // };
        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            return (sizeof(args) + ...);
        };


        template<contaner_out... Args>
        unsigned int write_container(const Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            return ((args.size() * sizeof(args.at(0))) + ...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.read(reinterpret_cast<char *>(&args), sizeof(Args))), ...);
            return (sizeof(Args) + ...);
        }


        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            std::lock_guard lock(mutex_);



        };

        template<contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            std::lock_guard lock(mutex_);



        };

        void delete_n(const unsigned long &ptr,const long &n) {

        };
        void delete_(const unsigned long &ptr_start,const unsigned long &ptr_end) {

        };
        void lazy_delete_n(const unsigned long &ptr,const long &n) {
            std::lock_guard lock(mutex_);
            shift_right_n_unsafe_thread(ptr,n);
        };
        void lazy_delete_(const unsigned long &ptr_start,const unsigned long &ptr_end) {

        };

    };


} // bsdb

#endif //SRC_PROVIDER_H
