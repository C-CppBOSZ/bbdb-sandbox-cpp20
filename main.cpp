#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits>



#include "bodb.h"
#include "scheme_b.h"



int main() {
    // bsdb::bodb::bodb bodb("/home/bogusz/CLionProjects/mainBSDB/test", "test");
    // bodb.add_simple_type(true,true,"int",4);

    std::cout << "Maksymalna wartość dla long: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "Maksymalna wartość dla unsigned long: " << std::numeric_limits<unsigned int>::max() << std::endl;

    unsigned int shr = 922;  // Przekracza zakres unsigned long
    std::cout << shr << std::endl;  // Wyświetli wartość modulo maksymalną wartość unsigned long
    std::cout << -shr << std::endl;  // Wyświetli wartość modulo maksymalną wartość unsigned long


    return 0;
}
