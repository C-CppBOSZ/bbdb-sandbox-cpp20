//
// Created by bogusz on 03.01.24.
//

#ifndef BODB_H
#define BODB_H
#include <filesystem>
#include <mutex>
#include <vector>

#include "../src_module/guard/IndexPtrGuard.h"
#include "../src_module/provider/SrcProviderFile.h"
#include "../src_module/base/SrcProviderBase.h"
#include "SchemeB.h"

namespace bsdb::bodb {
    template<typename SRC_PROVIDER>
    class Bodb {
    private:
        src_module::base::SrcProviderBase<SRC_PROVIDER> *src;
        mutable std::mutex mutex_;

    public:
        // explicit Bodb(const std::filesystem::path &db_path, const std::string &name) {
        //
        // };

        explicit Bodb(src_module::base::SrcProviderBase<SRC_PROVIDER> *src)
            : src(src) {
        }

        ~Bodb() {
            delete src;
        };

        unsigned long get_next_id() {
            unsigned long id = 0;
            {
                src_module::guard::IndexPtrGuard guard(*src);
                src->set_ptr(number_of_obj_position);
                src->read_obj(id);
                src->set_ptr(number_of_obj_position);
                src->write_obj(id + 1);
            }
            return id;
        };
        // TODO sprawdzać czy nie ma zagnierzdzonych typów takis samych żeby sie nie zrobiła pentla
        unsigned long add_simple_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                                      const int &n_byte) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                src->ptr_to_end();
                // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of byte
                src->write_obj(next_id, SchemeB::simple, options, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(n_byte);
            }
            return next_id;
        };

        unsigned long add_complex_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                                       const std::vector<unsigned long> &types) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                // TODO sprawdzić czy typy istnieją
                src->ptr_to_end();
                // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
                src->write_obj(next_id, SchemeB::complex, options, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(static_cast<int>(types.size()));
                src->write_contaner(types);
            }
            return next_id;
        };

        unsigned long add_array_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                                     const unsigned long &type_array, const unsigned long &size_array = 0) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                src->ptr_to_end();
                // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 8 - type array | if{is_dynamic}: 8 - size array? 0 - array is dynamic
                src->write_obj(next_id, SchemeB::array, options, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(type_array);
                if (!is_dynamic) {
                    src->write_obj(size_array);
                }
            }
            return next_id;
        };

        unsigned long add_generic_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                                       const std::vector<unsigned long> &types) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                // TODO sprawdzić czy typy istnieją
                src->ptr_to_end();
                // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
                src->write_obj(next_id, SchemeB::generic, options, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(static_cast<int>(types.size()));
                src->write_contaner(types);
            }
            return next_id;
        };

        unsigned long add_container_type(const std::string &name,const unsigned long &type_container) {
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                // TODO sprawdzić czy typy istnieją
                src->ptr_to_end();
                // 8 - index id | 1 - type | 4 - size name | {size name} - name | 8 - type_container
                src->write_obj(next_id, SchemeB::cantainer, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(type_container);
            }
            return next_id;
        };

        unsigned long add_static_b(const std::string &name, const unsigned long &type_static) {
            std::lock_guard lg(mutex_);
            unsigned long next_id = get_next_id();
            {
                src_module::guard::IndexPtrGuard guard(*src);
                // TODO sprawdzić czy typy istnieją
                src->ptr_to_end();
                // TODO sprawdzić czy test_db już taki kontener

                // 8 - index id | 1 - type | 4 - size name | {size name} - name | 8 - type_static | {obj(type_static)}
                src->write_obj(next_id, SchemeB::static_b, static_cast<unsigned int>(name.size()));
                src->write_contaner(name);
                src->write_obj(type_static);
            }
            return next_id;
        };
        void lazy_delete_n(const unsigned long &ptr,const long &n) {
            src->lazy_delete_n(ptr,n);
        };
    };



    Bodb<src_module::provider::SrcProviderFile> open_db_file(const std::filesystem::path &db_path, const std::string &name);
}


#endif //BODB_H
