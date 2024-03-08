#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <type_traits>


#include "bsdb/bodb/Bodb.h"
#include "bsdb/bodb/SchemeB.h"

// BinarySchemaDB
// BinSDB


// BinaryBlueprintDB
// bbdb

template<typename... Args>
std::vector<char> ArgsToVector(Args... args) {
    std::vector<char> resultVector;
    resultVector.reserve((sizeof(Args)+...));
    (..., (resultVector.insert(resultVector.end(), reinterpret_cast<const char *>(&args), reinterpret_cast<const char *>(&args) + sizeof(args))));
    return resultVector;
}

template<typename... Args,size_t N = (sizeof(Args)+...)>
std::array<char,N> ArgsToArray(Args... args) {
    std::array<char, N> arr;
    char* ptr = arr.data();
    ((memcpy(ptr, &args, sizeof(args)), ptr += sizeof(args)), ...);
    return arr;
}




int main() {




    // bbdb::Bodb::Bodb Bodb("/home/bogusz/CLionProjects/mainBSDB/test_db", "test_db");
    // auto Bodb = bbdb::Bodb::open_db_file("/home/bogusz/CLionProjects/mainBSDB/test_db", "test_db");
    // Bodb.add_simple_type(true,true,"int",4);
    // Bodb.lazy_delete_n(0,8);



    // auto fact = [&](int nu) {
    //     for (int i = 0; i < 10; i++) {
    //         std::string n= "t"+ std::to_string(nu) + " --- i" + std::to_string(i) + "\n";
    //         std::cout << n;
    //         Bodb.add_simple_type(true,true,n,4);
    //     }
    // };
    // std::vector<std::thread> tr = {};
    // for (int i = 0; i < 10; i++) {
    //     std::thread t(fact, i);
    //     tr.push_back(std::move(t));  // Move semantics for thread
    // }
    // for (std::thread &thread : tr) {
    //     thread.join();
    // }

    return 0;
}
