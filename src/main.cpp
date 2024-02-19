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
// bsdb

template<typename Func>
concept InvocableWithInt = requires(Func func, int i) {
    { func(i) } -> std::convertible_to<int>;
};

template<InvocableWithInt Func>
int invokeFun(Func func) {
    int i = 4;
    return func(i);
}


int fun1(int i) {

}

int fun2(long i) {

}

int fun3(std::string i) {

}

int main() {
    std::cout << "main";


    invokeFun(fun1);
    invokeFun(fun2);



    // bsdb::Bodb::Bodb Bodb("/home/bogusz/CLionProjects/mainBSDB/test_db", "test_db");
    // auto Bodb = bsdb::Bodb::open_db_file("/home/bogusz/CLionProjects/mainBSDB/test_db", "test_db");
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
