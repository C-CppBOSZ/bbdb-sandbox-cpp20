//
// Created by bogusz on 08.01.24.
//

#ifndef SRC_PROVIDER_H
#define SRC_PROVIDER_H
#include <cstddef>
#include <fstream>
#include <utility>

namespace bsdb {
    template<typename T>
    concept contaner_out = requires(T a)
    {
        { a.size() } -> std::same_as<std::size_t>;
        { a.data() } -> std::same_as<typename T::value_type *>;
        { a.at(std::declval<std::size_t>()) } -> std::same_as<typename T::reference>;
    };

    class src_provider {
    public:
        virtual ~src_provider() = default;

        virtual long get_ptr();

        virtual void set_ptr(const long &ptr);

        virtual void shift_ptr(const long &shift);

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            return 0;
        };

        template<contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            return 0;
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            return 0;
        };
    };

    class src_probider_impl_file : virtual public src_provider {
    private:
        std::fstream file;

    public:
        explicit src_probider_impl_file(const std::string& path, std::ios_base::openmode mode)
       : file(path, mode) {}

        void open(const std::string& path, std::ios_base::openmode mode) {
            file.open(path, mode);
        }

        void close() {
            file.close();
        }

        bool is_open() {
            return file.is_open();
        }

        long get_ptr() override {
            return file.tellg();
        };

        void set_ptr(const long &ptr) override {
            file.seekg(ptr, std::ios::beg);
            file.seekp(ptr, std::ios::beg);
        };

        void shift_ptr(const long &shift) override {
            file.seekg(shift, std::ios::cur);
            file.seekp(shift, std::ios::cur);
        };

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            (file.write(reinterpret_cast<const char *>(args), sizeof(Args)), ...);
            return (sizeof(Args) + ...);
        };

        template<contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            (file.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0))), ...);
            return ((args.size() * sizeof(args.at(0))) + ...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            (file.read(reinterpret_cast<const char *>(args), sizeof(Args)), ...);
            return (sizeof(Args) + ...);
        };
    };

    // class src_probider_impl_mem : virtual public src_provider {
    // private:
    //
    //
    // public:
    //     unsigned long get_ptr() override;
    //
    //     void set_ptr(const long &ptr) override;
    // };
} // bsdb

#endif //SRC_PROVIDER_H
