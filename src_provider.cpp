//
// Created by bogusz on 08.01.24.
//

#include "src_provider.h"

namespace bsdb {
    src_provider_impl_file::src_provider_impl_file(const std::string &path, std::ios_base::openmode mode): file(path, mode) {}

    src_provider_impl_file::~src_provider_impl_file() = default;

    void src_provider_impl_file::open(const std::string &path, std::ios_base::openmode mode) {
        file.open(path, mode);
    }

    void src_provider_impl_file::close() {
        file.close();
    }

    bool src_provider_impl_file::is_open() {
        return file.is_open();
    }

    unsigned long src_provider_impl_file::get_ptr() {
        return file.tellg();
    }

    void src_provider_impl_file::set_ptr(const unsigned long &ptr) {
        file.seekg(ptr, std::ios::beg);
        file.seekp(ptr, std::ios::beg);
    }

    void src_provider_impl_file::ptr_to_end() {
        file.seekg(0, std::ios::end);
        file.seekp(0, std::ios::end);
    }

    void src_provider_impl_file::shift_ptr(const long &shift) {
        file.seekg(shift, std::ios::cur);
        file.seekp(shift, std::ios::cur);
    }
} // bsdb