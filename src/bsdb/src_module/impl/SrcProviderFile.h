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
#include <filesystem>

#include "../base/SrcProviderBase.h"
#include "../guard/PtrGuard.h"

namespace bbdb::src_module::impl {
    class SrcProviderFile : virtual public base::SrcProviderBase<SrcProviderFile> {
    private:
        std::fstream file_;
        std::filesystem::path path_;
        mutable std::mutex mutex_;
        std::vector<unsigned long> ptrs_;

        void shift_left_content_ptr_unsafe_thread(unsigned long ptr, const unsigned long &content_size,
                                                     const unsigned long &size_buffer = 4096) {
            const auto size = get_size_unsafe_thread();
            if (ptr > size) {
                std::cerr << "shift_left_content_ptr_unsafe_thread error ptr:"<<ptr<<" > size:"<<size;
                throw std::runtime_error("left_right_content_ptr_unsafe_thread error ptr > size");
            }
            if (ptr + content_size >= size ) {
                std::filesystem::resize_file(path_,ptr);
                ptr_to_end_unsafe_thread();
                return;
            }
            set_ptr_unsafe_thread(ptr + content_size);
            if (const unsigned long diff = size - (ptr + content_size); diff <= size_buffer) {
                char buffer[diff];
                file_.read(buffer,diff);
                shift_ptr_unsafe_thread(-static_cast<long>(diff + content_size));
                file_.write(buffer,diff);
                std::filesystem::resize_file(path_,size-content_size);
                set_ptr_unsafe_thread(ptr);
                return;
            }
            while (get_ptr_unsafe_thread() + size_buffer < size) {
                char buffer[size_buffer];
                file_.read(buffer,size_buffer);
                shift_ptr_unsafe_thread(-static_cast<long>(size_buffer + content_size));
                file_.write(buffer,size_buffer);
                shift_ptr_unsafe_thread(static_cast<long>(content_size));
            }
            if ( const unsigned long diff = size - get_ptr_unsafe_thread() ;diff != 0) {
                char buffer[diff];
                file_.read(buffer,diff);
                shift_ptr_unsafe_thread(-static_cast<long>(diff + content_size));
                file_.write(buffer,diff);
                std::filesystem::resize_file(path_,size-content_size);
            }
            set_ptr_unsafe_thread(ptr);
        }

        void shift_right_content_ptr_unsafe_thread(const unsigned long &ptr, const unsigned long &size_content,
                                                      const unsigned int size_buffer = 4096) {
            ptr_to_end_unsafe_thread();
            const auto ptr_end = get_ptr_unsafe_thread();
            if (ptr > ptr_end) {
                std::cerr << "shift_right_content_ptr_unsafe_thread error ptr > ptr_end";
                throw std::runtime_error("shift_right_content_ptr_unsafe_thread error ptr > ptr_end");
            }
            if (const unsigned long diff = ptr_end - ptr; diff <= size_buffer) {
                shift_ptr_unsafe_thread(-static_cast<long>(diff));
                char buffer[diff];
                file_.read(buffer,diff);
                shift_ptr_unsafe_thread(-static_cast<long>(diff-size_content));
                file_.write(buffer,diff);
                set_ptr_unsafe_thread(ptr);
                return;
            }
            while (size_buffer < get_ptr_unsafe_thread() - ptr ) {
                shift_ptr_unsafe_thread(-static_cast<long>(size_buffer));
                char buffer[size_buffer];
                file_.read(buffer,size_buffer);
                shift_ptr_unsafe_thread(-static_cast<long>(size_buffer-size_content));
                file_.write(buffer,size_buffer);
                shift_ptr_unsafe_thread(-static_cast<long>(size_buffer+size_content));
            }
            if ( const unsigned long diff = get_ptr_unsafe_thread() - ptr;diff != 0){
                shift_ptr_unsafe_thread(-static_cast<long>(diff));
                char buffer[diff];
                file_.read(buffer,diff);
                shift_ptr_unsafe_thread(-static_cast<long>(diff-size_content));
                file_.write(buffer,diff);
            }
            set_ptr_unsafe_thread(ptr);
        }

        unsigned long get_size_unsafe_thread() const {
            return std::filesystem::file_size(path_);
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
            return file_.tellg();;
        }

    public:
        explicit SrcProviderFile(const std::filesystem::path &path, std::ios_base::openmode mode);

        ~SrcProviderFile() override;

        void open(const std::filesystem::path &path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        guard::PtrGuard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const long &shift) override;

        void shift_left_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                       const unsigned long &size_buffer = 4096) override {
            std::lock_guard lock(mutex_);
            shift_left_content_ptr_unsafe_thread(ptr, content_size, size_buffer);
        };

        void shift_right_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                        const unsigned long &size_buffer = 4096) override {
            std::lock_guard lock(mutex_);
            shift_right_content_ptr_unsafe_thread(ptr, content_size, size_buffer);
        };


        void delete_ptr_content(const unsigned long &ptr, const long &content_size) {
        };

        void delete_content(const long &content_size) override;

        void delete_ptr_to_ptr(const unsigned long &ptr_start, const unsigned long &ptr_end) {
        };

        void delete_to_ptr(const unsigned long &ptr_end) override;

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            const long size = (sizeof(args) + ...);
            shift_ptr_unsafe_thread(-size);
            return size;
        };

        template<base::contaner_out... Args>
        unsigned int write_container(const Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            const long size = ((args.size() * sizeof(args.at(0))) + ...);
            shift_ptr_unsafe_thread(-size);
            return size;
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            std::lock_guard lock(mutex_);
            ((file_.read(reinterpret_cast<char *>(&args), sizeof(Args))), ...);
            const long size = (sizeof(Args) + ...);
            shift_ptr_unsafe_thread(-size);
            return size;
        }

        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            std::lock_guard lock(mutex_);
            const long size = (sizeof(args) + ...);
            shift_right_content_ptr_unsafe_thread(get_ptr_unsafe_thread(),size);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            shift_ptr_unsafe_thread(-size);
            return size;
        };

        template<base::contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            std::lock_guard lock(mutex_);
            const long size = ((args.size() * sizeof(args.at(0))) + ...);
            shift_right_content_ptr_unsafe_thread(get_ptr_unsafe_thread(),size);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            shift_ptr_unsafe_thread(-size);
            return size;
        };
    };
}


#endif //SRCPROVIDERFILE_H
