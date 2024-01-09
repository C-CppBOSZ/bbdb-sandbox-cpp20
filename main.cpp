#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits>



#include "bodb.h"
#include "scheme_b.h"



int main() {
    bsdb::bodb::bodb bodb("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    bodb.add_simple_type(true,true,"int",4);



    return 0;
}
