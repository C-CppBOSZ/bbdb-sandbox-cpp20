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

        void shift_left_content(const unsigned long &content_size, const unsigned long &size_buffer = 4096) override;

        void shift_right_content(const unsigned long &content_size, const unsigned long &size_buffer = 4096) override;

        ~SrcProviderFile() override;

        void open(const std::filesystem::path &path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        guard::PtrGuard push_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const unsigned long &shift) override = delete;

        void shift_ptr(const long &shift) override;

        void shift_left_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                    const unsigned long &size_buffer = 4096) override {
            const auto size = get_size();
            if (ptr > size) {
                std::cerr << "shift_left_content_ptr error ptr:" << ptr << " > size:" << size;
                throw std::runtime_error("left_right_content_ptr error ptr > size");
            }
            if (ptr + content_size >= size) {
                std::filesystem::resize_file(path_, ptr);
                ptr_to_end();
                return;
            }
            set_ptr(ptr + content_size);
            if (const unsigned long diff = size - (ptr + content_size); diff <= size_buffer) {
                char buffer[diff];
                file_.read(buffer, diff);
                shift_ptr(-static_cast<long>(diff + content_size));
                file_.write(buffer, diff);
                std::filesystem::resize_file(path_, size - content_size);
                set_ptr(ptr);
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
            set_ptr(ptr);
        };

        void shift_right_ptr_content(const unsigned long &ptr, const unsigned long &content_size,
                                     const unsigned long &size_buffer = 4096) override {
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
                set_ptr(ptr);
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
            set_ptr(ptr);        };


        void delete_ptr_content(const unsigned long &ptr, const long &content_size) {
        };

        void delete_content(const long &content_size) override;

        void delete_ptr_to_ptr(const unsigned long &ptr_start, const unsigned long &ptr_end) {
        };

        void delete_to_ptr(const unsigned long &ptr_end) override;

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
            shift_right_content(get_ptr(), size);
            ((file_.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            shift_ptr(-size);
            return size;
        };

        template<typename... Args>
        unsigned int write_obj_ptr(const unsigned long &ptr, const Args &... args) {
        };

        template<base::contaner_out... Args>
        unsigned int write_contaner_ptr(const unsigned long &ptr, const Args &... args) {
        };

        template<typename... Args>
        unsigned int read_obj_ptr(const unsigned long &ptr, Args &... args) {
        };

        template<typename... Args>
        unsigned int insert_obj_ptr(const unsigned long &ptr, const Args &... args) {
        };

        template<base::contaner_out... Args>
        unsigned int insert_contaner_ptr(const unsigned long &ptr, const Args &... args) {
        };
    };
}


#endif //SRCPROVIDERFILE_H
