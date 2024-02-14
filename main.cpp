#include <algorithm>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <type_traits>

#include "src/bsdb/bodb/bodb.h"
#include "src/bsdb/bodb/scheme_b.h"


int main() {
    // bsdb::bodb::bodb bodb("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    auto bodb = bsdb::bodb::open_db_file("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    // bodb.add_simple_type(true,true,"int",4);
    bodb.lazy_delete_n(0,8);

    // auto fact = [&](int nu) {
    //     for (int i = 0; i < 10; i++) {
    //         std::string n= "t"+ std::to_string(nu) + " --- i" + std::to_string(i) + "\n";
    //         std::cout << n;
    //         bodb.add_simple_type(true,true,n,4);
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
