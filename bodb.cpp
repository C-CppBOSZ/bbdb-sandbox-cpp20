//
// Created by bogusz on 03.01.24.
//

#include "bodb.h"

#include <complex>
#include <filesystem>

#include "scheme_b.h"

namespace bsdb::bodb {
    // bodb::bodb(const std::filesystem::path &db_path, const std::string &name) {
    //     std::string path = db_path / (name + ".bsdb");
    //
    //     std::fstream fileStream(path, std::ios::in | std::ios::out | std::ios::binary);
    //
    //     if (!fileStream.is_open()) {
    //         try {
    //             fileStream.open(path, std::ios::out | std::ios::binary);
    //             if (!fileStream.is_open()) {
    //                 throw std::runtime_error("Error creating file");
    //             }
    //             db = std::move(fileStream);
    //             write_obj(0L);
    //             fileStream.close();
    //             fileStream.open(path, std::ios::in | std::ios::out | std::ios::binary);
    //         } catch (const std::exception &e) {
    //             throw std::runtime_error("Error opening or creating file: " + std::string(e.what()));
    //         }
    //     }
    //     db = std::move(fileStream);
    // }
    bodb::bodb(const std::filesystem::path &db_path, const std::string &name) {
        std::string path = db_path / (name + ".bsdb");

        auto _src = new src_probider_impl_file(path,std::ios::in | std::ios::out | std::ios::binary);

        if (!_src->is_open()) {
            try {
                _src->open(path, std::ios::out | std::ios::binary);
                if (!_src->is_open()) {
                    throw std::runtime_error("Error creating file");
                }
                _src->write_obj(0L);
                _src->close();
                _src->open(path, std::ios::in | std::ios::out | std::ios::binary);
            } catch (const std::exception &e) {
                throw std::runtime_error("Error opening or creating file: " + std::string(e.what()));
            }
        }

        src = _src;
    }

    unsigned long bodb::get_next_id() {
        const std::streampos originalp = db.tellp();
        const std::streampos originalg = db.tellg();
        db.seekg(0, std::ios::beg);
        unsigned long id = 0;
        db.read(reinterpret_cast<char *>(&id), sizeof(unsigned long));
        db.seekp(0, std::ios::beg);
        write_obj(id + 1);
        db.seekp(originalp);
        db.seekg(originalg);
        return id;
    }

    void bodb::add_simple_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                               const int &n_byte) {
        unsigned char options = 0;
        if (is_nullable) {
            options |= 0x1;
        }
        if (is_dynamic) {
            options |= 0x10;
        }
        const auto streampos = db.tellp();
        db.seekp(0, std::ios::end);
        // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of byte
        write_obj(get_next_id(),scheme_b::simple, options, static_cast<unsigned int>(name.size()));
        write_contaner(name);
        write_obj(n_byte);
        db.seekp(streampos);
    }

    void bodb::add_complex_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                                const std::vector<unsigned long> &types) {
        unsigned char options = 0;
        if (is_nullable) {
            options |= 0x1;
        }
        if (is_dynamic) {
            options |= 0x10;
        }
        const auto streampos = db.tellp();
        db.seekp(0, std::ios::end);
        // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
        write_obj(get_next_id(),scheme_b::complex, options, static_cast<unsigned int>(name.size()));
        write_contaner(name);
        write_obj(static_cast<int>(types.size()));
        write_contaner(types);
        db.seekp(streampos);
    }
} // bsdb
