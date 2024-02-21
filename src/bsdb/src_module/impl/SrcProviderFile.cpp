//
// Created by bogusz on 19.02.24.
//

#include "SrcProviderFile.h"

namespace bsdb::src_module::impl {
    SrcProviderFile::SrcProviderFile(const std::string &path, std::ios_base::openmode mode): file_(path, mode) {}

    SrcProviderFile::~SrcProviderFile() = default;

    void SrcProviderFile::open(const std::string &path, std::ios_base::openmode mode) {
        std::lock_guard lock(mutex_);
        file_.open(path, mode);
    }

    void SrcProviderFile::close() {
        std::lock_guard lock(mutex_);
        file_.close();
    }

    bool SrcProviderFile::is_open() {
        std::lock_guard lock(mutex_);
        return file_.is_open();
    }

    unsigned long SrcProviderFile::get_ptr() {
        std::lock_guard lock(mutex_);
        return file_.tellg();
    }

    guard::PtrGuard SrcProviderFile::push_ptr() {
        std::lock_guard lock(mutex_);
        return guard::PtrGuard(&ptrs_,file_.tellg());
    }

    void SrcProviderFile::set_ptr(const unsigned long &ptr) {
        std::lock_guard lock(mutex_);
        set_ptr_unsafe_thread(ptr);
    }

    void SrcProviderFile::ptr_to_end() {
        std::lock_guard lock(mutex_);
        file_.seekg(0, std::ios::end);
        file_.seekp(0, std::ios::end);
    }

    void SrcProviderFile::shift_ptr(const long &shift) {
        std::lock_guard lock(mutex_);
        shift_ptr_unsafe_thread(shift);
    }
} // bsdb