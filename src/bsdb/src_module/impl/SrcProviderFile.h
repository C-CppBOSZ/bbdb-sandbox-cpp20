//
// Created by bogusz on 19.02.24.
//

#ifndef SRCPROVIDERFILE_H
#define SRCPROVIDERFILE_H
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <cstring>

#include "../base/SrcProviderBase.h"
#include "../guard/PtrGuard.h"

namespace bbdb::src_module::impl {
    class SrcProviderFile : virtual public base::SrcProviderBase<SrcProviderFile> {
    private:
        std::fstream file_;
        mutable std::mutex mutex_;
        std::vector<unsigned long> ptrs_;

        void simple_shift_left_content_unsafe_thread(const unsigned long &ptr, const unsigned long &content_size,
                                                     const unsigned long &size_buffer = 4096) {
            if (content_size == 0) return;
            char buffer[size_buffer];
            ptr_to_end(); // TODO ehhhhh deadlock
            unsigned long ptr_end = get_ptr();
            unsigned long size = ptr_end - ptr;
            unsigned int n_shift = size / size_buffer;
            set_ptr(ptr); // TODO ehhhhh deadlock

            file_.read(buffer, size_buffer);
        }

        void simple_shift_right_content_unsafe_thread(const unsigned long &ptr, const unsigned long &content_size,
                                                      unsigned int size_buffer = 4096) {
            if (content_size == 0) return;
            if (content_size > size_buffer) return shift_right_n_unsafe_thread(ptr, content_size);
            if (shift_right_if_n_exceeds_file_size_unsafe_thread(ptr, content_size)) return;

            unsigned long size = get_size_unsafe_thread();

            set_ptr_unsafe_thread(ptr);
            if (unsigned long size_to_end = get_ptr_unsafe_thread();size_to_end + size_buffer + content_size >= size ) {
                size_to_end = size - size_to_end;
                char buffer[size_to_end];
                file_.read(buffer,size_to_end);
                shift_ptr_unsafe_thread(-static_cast<long>(size_to_end - content_size));
                file_.write(buffer,size_to_end);

                set_ptr_unsafe_thread(ptr);
                return;
            }

            char buffer[size_buffer];
            char content[content_size];


            // unsigned long size = get_size_unsafe_thread() - (ptr + content_size);
            // unsigned int n_shift = size / size_buffer;


            file_.read(buffer,size_buffer);
            file_.read(content,content_size);

            unsigned long diff_buffer_size = size_buffer - content_size;

            while (!file_.eof()) {
                // TODO + size_buffer + content_size
                if (unsigned long size_to_end = get_ptr_unsafe_thread();size_to_end  + size_buffer + content_size >= size ) {
                    size_to_end = size - size_to_end;
                    shift_ptr_unsafe_thread(-static_cast<long>(size_buffer));
                    file_.write(buffer,size_buffer);
                    file_.read(buffer,size_to_end);
                    shift_ptr_unsafe_thread(-static_cast<long>(size_to_end - content_size));
                    file_.write(buffer,size_to_end);
                    break;
                }
                shift_ptr_unsafe_thread(-static_cast<long>(size_buffer));
                file_.write(buffer,size_buffer);
                file_.read(buffer+content_size,diff_buffer_size);
                std::memcpy(buffer,content,content_size);
                file_.read(content,content_size);
            }

            set_ptr_unsafe_thread(ptr);

        }

        void shift_right_n_unsafe_thread(const unsigned long &ptr, const unsigned long &n) {
            // TODO kiedy n jest wieksze od całego pliku
            if (shift_right_if_n_exceeds_file_size_unsafe_thread(ptr, n)) return;
            if (n == 0) return;
            char buffer[2][n];
            bool switch_buffer = false;
            unsigned long size = get_size_unsafe_thread();
            set_ptr_unsafe_thread(ptr);
            file_.read(buffer[switch_buffer], n);
            while (!file_.eof()) {
                if ((get_ptr_unsafe_thread() + n) >= size) {
                    const unsigned long tmp = size - get_ptr_unsafe_thread();
                    char tmp_buffer[tmp];
                    file_.read(tmp_buffer, tmp);
                    shift_ptr_unsafe_thread(-static_cast<long>(tmp));
                    file_.write(buffer[switch_buffer], n);
                    file_.write(tmp_buffer, tmp);
                    break;
                }
                file_.read(buffer[!switch_buffer], n);
                shift_ptr_unsafe_thread(-static_cast<long>(n));
                file_.write(buffer[switch_buffer], n);
                switch_buffer = !switch_buffer;
            }
        }

        bool shift_right_if_n_exceeds_file_size_unsafe_thread(const unsigned long &ptr, const unsigned long &n) {
            const unsigned long size = get_size_unsafe_thread();
            if (const unsigned long ptr_end = ptr + n; ptr_end > size) {
                unsigned long diff = ptr_end - size;
                char buffer[size];
                set_ptr_unsafe_thread(0);
                file_.read(buffer, size);
                constexpr char nullChar = '\0';
                file_.write(&nullChar, diff);
                file_.write(buffer, size);
                return true;
            } else
                return false;
        }

        unsigned long get_size_unsafe_thread() {
            const auto streampos = get_ptr_unsafe_thread();
            file_.seekg(0, std::ios::end);
            const unsigned long tmp = get_ptr_unsafe_thread();
            file_.seekg(streampos);
            return tmp;
        }

        void set_ptr_unsafe_thread(const unsigned long &ptr) {
            file_.seekg(ptr, std::ios::beg);
            file_.seekp(ptr, std::ios::beg);
        }

        void shift_ptr_unsafe_thread(const unsigned long &shift) = delete;

        void shift_ptr_unsafe_thread(const long &shift) {
            file_.seekp(shift, std::ios::cur);
            file_.seekg(file_.tellp()); // TODO najprawdopodobniej nie potrzebnr seekp przesówa też g
        }

        void ptr_to_end_unsafe_thread() {
            file_.seekg(0, std::ios::end);
            file_.seekp(0, std::ios::end);
        }

        unsigned long get_ptr_unsafe_thread() {
            if (file_.tellg()<0) {
                std::cerr << "get_ptr_unsafe_thread error < 0";
                throw std::runtime_error("get_ptr_unsafe_thread error < 0");
            }
            auto streampos = file_.tellg();
            unsigned long t = streampos;
            return streampos;
        }

    public:
        explicit SrcProviderFile(const std::string &path, std::ios_base::openmode mode);

        ~SrcProviderFile() override;

        void open(const std::string &path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        guard::PtrGuard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const long &shift) override;

        void simple_shift_left_content(const unsigned long &ptr, const unsigned long &content_size,
                                       const unsigned long &size_buffer = 4096) override {
            std::lock_guard lock(mutex_);
            simple_shift_left_content_unsafe_thread(ptr, content_size, size_buffer);
        };

        void simple_shift_right_content(const unsigned long &ptr, const unsigned long &content_size,
                                        const unsigned long &size_buffer = 4096) override {
            std::lock_guard lock(mutex_);
            simple_shift_right_content_unsafe_thread(ptr, content_size, size_buffer);
        };


        void delete_n(const unsigned long &ptr, const long &n) {
        };

        void delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) {
        };

        void lazy_delete_n(const unsigned long &ptr, const long &n) {
            std::lock_guard lock(mutex_);
            shift_right_n_unsafe_thread(ptr, n);
        };

        void lazy_delete_(const unsigned long &ptr_start, const unsigned long &ptr_end) {
        }

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


        template<base::contaner_out... Args>
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

        template<base::contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            std::lock_guard lock(mutex_);
        };
    };
}


#endif //SRCPROVIDERFILE_H
