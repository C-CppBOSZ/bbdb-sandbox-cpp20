//
// Created by bogusz on 03.01.24.
//

#ifndef BODB_H
#define BODB_H
#include <filesystem>
#include <vector>

#include "scheme_b.h"
#include "src_provider.h"

namespace bsdb::bodb {
    template<typename SRC_PROVIDER>
    class bodb {
    private:
        src_provider<SRC_PROVIDER> *src;

    public:
        // explicit bodb(const std::filesystem::path &db_path, const std::string &name) {
        //
        // };

        explicit bodb(src_provider<SRC_PROVIDER> *src)
            : src(src) {
        }

        ~bodb() {
            delete src;
        };

        unsigned long get_next_id() {
            const unsigned long ptr = src->get_ptr();
            src->set_ptr(number_of_obj_position);
            unsigned long id = 0;
            src->read_obj(id);
            src->set_ptr(number_of_obj_position);
            src->write_obj(id + 1);
            src->set_ptr(ptr);
            return id;
        };
        // TODO sprawdzać czy nie ma zagnierzdzonych typów takis samych żeby sie nie zrobiła pentla
        void add_simple_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                             const int &n_byte) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            const unsigned long ptr = src->get_ptr();
            src->ptr_to_end();
            // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of byte
            src->write_obj(get_next_id(), scheme_b::simple, options, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(n_byte);
            src->set_ptr(ptr);
        };

        void add_complex_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                              const std::vector<unsigned long> &types) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            const unsigned long ptr = src->get_ptr();
            // TODO sprawdzić czy typy istnieją
            src->ptr_to_end();
            // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
            src->write_obj(get_next_id(), scheme_b::complex, options, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(static_cast<int>(types.size()));
            src->write_contaner(types);
            src->set_ptr(ptr);
        };

        void add_array_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                              const unsigned long &type_array, const unsigned long &size_array = 0) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            const unsigned long ptr = src->get_ptr();
            src->ptr_to_end();
            // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 8 - type array | if{is_dynamic}: 8 - size array? 0 - array is dynamic
            src->write_obj(get_next_id(), scheme_b::array, options, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(type_array);
            if (!is_dynamic) {
                src->write_obj(size_array);
            }
            src->set_ptr(ptr);
        };

        void add_generic_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                              const std::vector<unsigned long> &types) {
            unsigned char options = 0;
            if (is_nullable) {
                options |= 0x1;
            }
            if (is_dynamic) {
                options |= 0x10;
            }
            const unsigned long ptr = src->get_ptr();
            // TODO sprawdzić czy typy istnieją
            src->ptr_to_end();
            // 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types
            src->write_obj(get_next_id(), scheme_b::generic, options, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(static_cast<int>(types.size()));
            src->write_contaner(types);
            src->set_ptr(ptr);
        };

        unsigned long add_container_type(const std::string &name,const unsigned long &type_container) {
            const unsigned long ptr = src->get_ptr();
            // TODO sprawdzić czy typy istnieją
            src->ptr_to_end();
            unsigned long next_id = get_next_id();
            // 8 - index id | 1 - type | 4 - size name | {size name} - name | 8 - type_container
            src->write_obj(next_id, scheme_b::cantainer, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(type_container);
            src->set_ptr(ptr);
            return next_id;
        };

        void add_static_b(const std::string &name,const unsigned long &type_static) {
            const unsigned long ptr = src->get_ptr();
            // TODO sprawdzić czy typy istnieją
            src->ptr_to_end();
            // TODO sprawdzić czy test już taki kontener

            // 8 - index id | 1 - type | 4 - size name | {size name} - name | 8 - type_static | {obj(type_static)}
            src->write_obj(get_next_id(), scheme_b::static_b, static_cast<unsigned int>(name.size()));
            src->write_contaner(name);
            src->write_obj(type_static);
            src->set_ptr(ptr);
        };
    };



    bodb<src_provider_impl_file> open_db_file(const std::filesystem::path &db_path, const std::string &name);
}


#endif //BODB_H
