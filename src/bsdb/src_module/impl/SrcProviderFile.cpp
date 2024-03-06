//
// Created by bogusz on 19.02.24.
//

#include "SrcProviderFile.h"

namespace bbdb::src_module::impl {
    SrcProviderFile::SrcProviderFile(const std::filesystem::path &path, std::ios_base::openmode mode): file_(path, mode), path_(path) {}

    SrcProviderFile::~SrcProviderFile() = default;

    void SrcProviderFile::open(const std::filesystem::path &path, std::ios_base::openmode mode) {
        path_ = path;
        file_.open(path, mode);
    }

    void SrcProviderFile::close() {
        file_.close();
    }

    bool SrcProviderFile::is_open() {
        return file_.is_open();
    }

    unsigned long SrcProviderFile::get_ptr() {
        if (file_.tellg() < 0) {
            std::cerr << "get_ptr_unsafe_thread error < 0";
            throw std::runtime_error("get_ptr_unsafe_thread error < 0");
        }
        return file_.tellg();
    }

    guard::PtrGuard SrcProviderFile::push_ptr() {
        return guard::PtrGuard(&ptrs_,file_.tellg());
    }

    void SrcProviderFile::set_ptr(const unsigned long &ptr) {
        file_.seekg(ptr, std::ios::beg);
        file_.seekp(ptr, std::ios::beg);
    }

    void SrcProviderFile::ptr_to_end() {
        file_.seekg(0, std::ios::end);
        file_.seekp(0, std::ios::end);
    }

    void SrcProviderFile::shift_ptr(const long &shift) {
        file_.seekp(shift, std::ios::cur);
        file_.seekg(file_.tellp()); // TODO najprawdopodobniej nie potrzebnr seekp przesówa też g
    }
} // bbdb