//
// Created by bogusz on 03.01.24.
//

#ifndef BODB_H
#define BODB_H
#include <filesystem>
#include <vector>

#include "src_provider.h"

namespace bsdb::bodb {

    class bodb {
    private:
        src_provider<src_provider_impl_file> *src;

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
