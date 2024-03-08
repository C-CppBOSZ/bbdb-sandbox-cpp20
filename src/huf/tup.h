#ifndef TUP_H
#define TUP_H

#include <iostream>
#include <tuple>
#include <vector>
#include <cstring>

namespace bdb::tup {
    template <std::size_t Index = 0, typename... Ts>
    inline void tuple_to_vector_helper(const std::tuple<Ts...>& t, std::vector<char>& v) {
        if constexpr (Index < sizeof...(Ts)) {
            const char* data = reinterpret_cast<const char*>(&std::get<Index>(t));
            v.insert(v.end(), data, data + sizeof(std::get<Index>(t)));

            tuple_to_vector_helper<Index + 1, Ts...>(t, v);
        }
    }

    template <typename... Ts>
    inline void tuple_to_vector(const std::tuple<Ts...>& t, std::vector<char>& v) {
        tuple_to_vector_helper(t, v);
    }

    template <std::size_t Index = 0, typename... Ts>
    inline void vector_to_tuple_helper(const std::vector<char>& v, std::tuple<Ts...>* t, std::size_t& offset) {
        if constexpr (Index < sizeof...(Ts)) {
            const std::size_t size = sizeof(std::get<Index>(*t));
            const std::size_t newOffset = offset + size;

            if (newOffset <= v.size()) {
                std::memcpy(&std::get<Index>(*t), v.data() + offset, size);

                offset = newOffset;

                vector_to_tuple_helper<Index + 1, Ts...>(v, t, offset);
            } else {
                std::cerr << "Błąd: Nie wystarczająca ilość danych w wektorze." << std::endl;
            }
        }
    }

    template <typename... Ts>
    inline void vector_to_tuple(const std::vector<char>& v, std::tuple<Ts...>* t) {
        std::size_t offset = 0;
        vector_to_tuple_helper(v, t, offset);
    }
}

#endif // TUP_H


//std::tuple<int, short, long> myTuple(42, 10, 100);
//
//// Konwertuj tuple na vector<char>
//std::vector<char> charVector;
//bdb::tup::tuple_to_vector(myTuple, charVector);
//
//// Wyświetl zawartość wektora
//std::cout << "Zawartość wektora<char>: ";
//for (char c : charVector) {
//std::cout << static_cast<int>(c) << " ";
//}
//std::cout << std::endl;
//
//// Przygotuj wskaźnik do tupla
//std::tuple<int, short, long> restoredTuple;
//
//// Konwertuj vector<char> z powrotem na tuple przy użyciu wskaźnika
//bdb::tup::vector_to_tuple(charVector, &restoredTuple);
//
//// Wyświetl zawartość przywróconego tupla
//std::cout << "Przywrócony tuple: "
//<< std::get<0>(restoredTuple) << " "
//<< std::get<1>(restoredTuple) << " "
//<< std::get<2>(restoredTuple) << std::endl;
