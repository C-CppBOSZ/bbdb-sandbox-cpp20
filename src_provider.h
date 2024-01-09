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

    template <typename Derived>
    class src_provider {
    public:
        virtual ~src_provider() = default;

        virtual unsigned long get_ptr() = 0;

        virtual void set_ptr(const unsigned long &ptr) = 0;

        virtual void ptr_to_end() = 0;

        virtual void shift_ptr(const long &shift) = 0;

        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template write_obj(args...);
        };

        template<contaner_out... Args>
        unsigned int write_contaner(const Args &... args) {
            return dynamic_cast<Derived*>(this)->template write_container(args...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            return dynamic_cast<Derived*>(this)->template read_obj(args...);
        };
    };

    class src_provider_impl_file : virtual public src_provider<src_provider_impl_file> {
    private:
        std::fstream file;

    public:
        explicit src_provider_impl_file(const std::string& path, std::ios_base::openmode mode);

        ~src_provider_impl_file() override;

        void open(const std::string& path, std::ios_base::openmode mode);

        void close();

        bool is_open();

        unsigned long get_ptr() override;

        void set_ptr(const unsigned long &ptr) override;

        void ptr_to_end() override;

        void shift_ptr(const long &shift) override;

        // template<typename... Args>
        // unsigned int write_obj(const Args &... args) {
        //     // if constexpr (sizeof...(Args) > 0) {
        //         (file.write(reinterpret_cast<const char *>(args), sizeof(Args)), ...);
        //         return (sizeof(Args) + ...);
        //     // } else {
        //     //     return 0;
        //     // }
        // };
        template<typename... Args>
        unsigned int write_obj(const Args &... args) {
            ((file.write(reinterpret_cast<const char *>(&args), sizeof(args))), ...);
            return (sizeof(args) + ...);
        };



        template<contaner_out... Args>
        unsigned int write_container(const Args &... args) {
            ((file.write(reinterpret_cast<const char *>(args.data()), args.size() * sizeof(args.at(0)))), ...);
            return ((args.size() * sizeof(args.at(0))) + ...);
        };

        template<typename... Args>
        unsigned int read_obj(Args &... args) {
            ((file.read(reinterpret_cast<char *>(&args), sizeof(Args))), ...);
            return (sizeof(Args) + ...);
        };
    };


} // bsdb

#endif //SRC_PROVIDER_H
