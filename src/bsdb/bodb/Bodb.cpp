//
// Created by bogusz on 03.01.24.
//

#include "Bodb.h"

#include <complex>
#include <filesystem>

#include "SchemeB.h"

namespace bsdb::bodb {
    // Bodb::Bodb(const std::filesystem::path &db_path, const std::string &name) {
    //     std::string path = db_path / (name + ".bsdb");
    //
    //     const auto _src = new SrcProviderFile(path,std::ios::in | std::ios::out | std::ios::binary);
    //
    //     if (!_src->is_open()) {
    //         try {
    //             _src->open(path, std::ios::out | std::ios::binary);
    //             if (!_src->is_open()) {
    //                 throw std::runtime_error("Error creating file_");
    //             }
    //             _src->write_obj(static_cast<unsigned long>(0L));
    //             _src->close();
    //             _src->open(path, std::ios::in | std::ios::out | std::ios::binary);
    //         } catch (const std::exception &e) {
    //             throw std::runtime_error("Error opening or creating file_: " + std::string(e.what()));
    //         }
    //     }
    //
    //     src = _src;
    // }
    //
    // unsigned long Bodb::get_next_id() {
    //     const unsigned long ptr = src->get_ptr();
    //     src->set_ptr(number_of_obj_position);
    //     unsigned long id = 0;
    //     src->read_obj(id);
    //     src->set_ptr(number_of_obj_position);
    //     src->write_obj(id +1);
    //     src->set_ptr(ptr);
    //     return id;
    // }
    //
    // void Bodb::add_simple_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
    //                            const int &n_byte) {
    //     unsigned char options = 0;
    //     if (is_nullable) {
    //         options |= 0x1;
    //     }
    //     if (is_dynamic) {
    //         options |= 0x10;
    //     }
    //     const unsigned long ptr = src->get_ptr();
    //     src->ptr_to_end();
    //     // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of byte
    //     src->write_obj(get_next_id(),SchemeB::simple, options, static_cast<unsigned int>(name.size()));
    //     src->write_contaner(name);
    //     src->write_obj(n_byte);
    //     src->set_ptr(ptr);
    // }
    //
    // void Bodb::add_complex_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
    //                             const std::vector<unsigned long> &types) {
    //     unsigned char options = 0;
    //     if (is_nullable) {
    //         options |= 0x1;
    //     }
    //     if (is_dynamic) {
    //         options |= 0x10;
    //     }
    //     const unsigned long ptr = src->get_ptr();
    //     src->ptr_to_end();
    //     // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
    //     src->write_obj(get_next_id(),SchemeB::complex, options, static_cast<unsigned int>(name.size()));
    //     src->write_contaner(name);
    //     src->write_obj(static_cast<int>(types.size()));
    //     src->write_contaner(types);
    //     src->set_ptr(ptr);
    // }
    Bodb<src_module::impl::SrcProviderFile> open_db_file(const std::filesystem::path &db_path, const std::string &name) {
        std::string path = db_path / (name + ".bsdb");

        const auto _src = new src_module::impl::SrcProviderFile(path, std::ios::in | std::ios::out | std::ios::binary);

        if (!_src->is_open()) {
            try {
                _src->open(path, std::ios::out | std::ios::binary);
                if (!_src->is_open()) {
                    throw std::runtime_error("Error creating file_");
                }
                _src->write_obj(static_cast<unsigned long>(0L));
                _src->close();
                _src->open(path, std::ios::in | std::ios::out | std::ios::binary);
            } catch (const std::exception &e) {
                throw std::runtime_error("Error opening or creating file_: " + std::string(e.what()));
            }
        }

        return static_cast<Bodb<src_module::impl::SrcProviderFile>>(_src);
    }
} // bsdb
