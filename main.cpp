#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <type_traits>
#include "bodb.h"
#include "scheme_b.h"


int main() {
    // bsdb::bodb::bodb bodb("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    auto bodb = bsdb::bodb::open_db_file("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    bodb.add_simple_type(true,true,"int",4);



    return 0;
}
