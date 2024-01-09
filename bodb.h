//
// Created by bogusz on 03.01.24.
//

#ifndef BODB_H
#define BODB_H
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>

#include "src_provider.h"

namespace bsdb::bodb {
    template<typename T>
    concept contaner_i = requires(T a)
    {
        { a.size() } -> std::same_as<std::size_t>;
        { a.data() } -> std::same_as<typename T::value_type *>;
        { a.at(std::declval<std::size_t>()) } -> std::same_as<typename T::reference>;
    };


    class bodb {
    private:
        std::fstream db;
        src_provider *src;

        template<typename... type>
        int write_obj(const type... ob) {
            (db.write(reinterpret_cast<const char *>(&ob), sizeof(type)), ...);
            return (sizeof(type) + ...);
        }

        template<contaner_i... type>
        int write_contaner(const type... ob) {
            (db.write(reinterpret_cast<const char *>(ob.data()), ob.size() * sizeof(ob.at(0))), ...);
            return ((ob.size() * sizeof(ob.at(0))) + ...);
        }

    public:
        explicit bodb(const std::filesystem::path &db_path, const std::string &name);

        ~bodb() {
            delete src;
        };

        unsigned long get_next_id();

        void add_simple_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                             const int &n_byte);

        void add_complex_type(const bool &is_nullable, const bool &is_dynamic, const std::string &name,
                              const std::vector<unsigned long> &types);
    };
}


#endif //BODB_H
