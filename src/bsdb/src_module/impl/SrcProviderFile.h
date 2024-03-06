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
        std::vector<unsigned long> ptrs_;

    public:
        explicit SrcProviderFile(const std::filesystem::path &path, std::ios_base::openmode mode);

        unsigned long get_size() override {
            return std::filesystem::file_size(path_);
        };

        void shift_left_content(const unsigned long &content_size, const unsigned long &size_buffer = 4096) override {
            shift_left_ptr_content(get_ptr(),content_size,size_buffer);
        };

        void shift_right_content(const unsigned long &content_size, const unsigned long &size_buffer = 4096) override {
            shift_right_ptr_content(get_ptr(),content_size,size_buffer);
        };

        ~SrcProviderFile() override;

        void open(const std::filesystem::path &path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        guard::PtrGuard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        // void shift_ptr(const unsigned long  &shift) = delete;

        void shift_ptr(const long &shift) override;

        void shift_left_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                    const unsigned long &size_buffer = 4096) override {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            const auto size = get_size();
            if (ptr > size) {
                std::cerr << "shift_left_content_ptr error ptr:" << ptr << " > size:" << size;
                throw std::runtime_error("left_right_content_ptr error ptr > size");
            }
            if (ptr + content_size >= size) {
                std::filesystem::resize_file(path_, ptr);
                set_ptr(ptr_exit);
                return;
            }
            set_ptr(ptr + content_size);
            if (const unsigned long diff = size - (ptr + content_size); diff <= size_buffer) {
                char buffer[diff];
                file_.read(buffer, diff);
                shift_ptr(-static_cast<long>(diff + content_size));
                file_.write(buffer, diff);
                std::filesystem::resize_file(path_, size - content_size);
                set_ptr(ptr_exit);
                return;
            }
            while (get_ptr() + size_buffer < size) {
                char buffer[size_buffer];
                file_.read(buffer, size_buffer);
                shift_ptr(-static_cast<long>(size_buffer + content_size));
                file_.write(buffer, size_buffer);
                shift_ptr(static_cast<long>(content_size));
            }
            if (const unsigned long diff = size - get_ptr(); diff != 0) {
                char buffer[diff];
                file_.read(buffer, diff);
                shift_ptr(-static_cast<long>(diff + content_size));
                file_.write(buffer, diff);
                std::filesystem::resize_file(path_, size - content_size);
            }
            set_ptr(ptr_exit);
        };

        void shift_right_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                     const unsigned long &size_buffer = 4096) override {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            ptr_to_end();
            const auto ptr_end = get_ptr();
            if (ptr > ptr_end) {
                std::cerr << "shift_right_content_ptr error ptr > ptr_end";
                throw std::runtime_error("shift_right_content_ptr error ptr > ptr_end");
            }
            if (const unsigned long diff = ptr_end - ptr; diff <= size_buffer) {
                shift_ptr(-static_cast<long>(diff));
                char buffer[diff];
                file_.read(buffer, diff);
                shift_ptr(-static_cast<long>(diff - content_size));
                file_.write(buffer, diff);
                set_ptr(ptr_exit);
                return;
            }
            while (size_buffer < get_ptr() - ptr) {
                shift_ptr(-static_cast<long>(size_buffer));
                char buffer[size_buffer];
                file_.read(buffer, size_buffer);
                shift_ptr(-static_cast<long>(size_buffer - content_size));
                file_.write(buffer, size_buffer);
                shift_ptr(-static_cast<long>(size_buffer + content_size));
            }
            if (const unsigned long diff = get_ptr() - ptr; diff != 0) {
                shift_ptr(-static_cast<long>(diff));
                char buffer[diff];
                file_.read(buffer, diff);
                shift_ptr(-static_cast<long>(diff - content_size));
                file_.write(buffer, diff);
            }
            set_ptr(ptr_exit);        };


        void delete_ptr_content(const unsigned long &ptr, const long &content_size) {
            shift_left_ptr_content(ptr,content_size);
        };

        void delete_content(const long &content_size) override {
            // TODO def size buffer
            shift_left_content(content_size);
        };

        void delete_ptr_to_ptr(const unsigned long &ptr_start, const unsigned long &ptr_end) {
            if (ptr_start == ptr_end)
                return;
            const unsigned long diff = ptr_start > ptr_end ? ptr_start - ptr_end : ptr_end - ptr_start;
            shift_left_ptr_content(ptr_start,diff);
        };

        void delete_to_ptr(const unsigned long &ptr_end) override {
            delete_ptr_to_ptr(get_ptr(),ptr_end);
        };

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            const long size = (sizeof(args) + ...);
            shift_ptr(-size);
            return size;
        };

        template<base::contaner_out... Args>
        unsigned int write_container(const Args &... args) {
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            const long size = ((args.size() * sizeof(args.at(0))) + ...);
            shift_ptr(-size);
            return size;
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            ((file_.read(reinterpret_cast<char *>(&args), sizeof(Args))), ...);
            const long size = (sizeof(Args) + ...);
            shift_ptr(-size);
            return size;
        }

        template<typename... Args>
        unsigned int insert_obj(const Args &... args) {
            const long size = (sizeof(args) + ...);
            shift_right_content(size);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            shift_ptr(-size);
            return size;
        };

        template<base::contaner_out... Args>
        unsigned int insert_contaner(const Args &... args) {
            const long size = ((args.size() * sizeof(args.at(0))) + ...);
            shift_right_content(size);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            shift_ptr(-size);
            return size;
        };

        template<typename... Args>
        unsigned int write_obj_ptr(const unsigned long &ptr, const Args &... args) {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            set_ptr(ptr);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            set_ptr(ptr_exit);
            return (sizeof(args) + ...);
        };

        template<base::contaner_out... Args>
        unsigned int write_contaner_ptr(const unsigned long &ptr, const Args &... args) {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            set_ptr(ptr);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            set_ptr(ptr_exit);
            return ((args.size() * sizeof(args.at(0))) + ...);
        };

        template<typename... Args>
        unsigned int read_obj_ptr(const unsigned long &ptr, Args &... args) {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            set_ptr(ptr);
            ((file_.read(reinterpret_cast<char *>(&args), sizeof(Args))), ...);
            set_ptr(ptr_exit);
            return (sizeof(Args) + ...);
        };

        template<typename... Args>
        unsigned int insert_obj_ptr(const unsigned long &ptr, const Args &... args) {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            const long size = (sizeof(args) + ...);
            set_ptr(ptr);
            shift_right_content(size);
            ((file_.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            set_ptr(ptr_exit);
            return (sizeof(Args) + ...);
        };

        template<base::contaner_out... Args>
        unsigned int insert_contaner_ptr(const unsigned long &ptr, const Args &... args) {
            // TODO push_ptr
            const unsigned long ptr_exit = get_ptr();
            const long size = ((args.size() * sizeof(args.at(0))) + ...);
            set_ptr(ptr);
            shift_right_content(size);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            set_ptr(ptr_exit);
            return (sizeof(Args) + ...);
        };
    };
}


#endif //SRCPROVIDERFILE_H
