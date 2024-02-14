cmake_minimum_required(VERSION 3.28)
project(mainBSDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(mainBSDB main.cpp
        src/bsdb/bodb/bodb.cpp
        src/bsdb/bodb/bodb.h
        src/bsdb/bodb/type_provider.cpp
        src/bsdb/bodb/type_provider.h
        src/bsdb/bodb/scheme_b.cpp
        src/bsdb/bodb/scheme_b.h
        src/bsdb/src_provider.cpp
        src/bsdb/src_provider.h
        src/huf/huf.h
        src/bsdb/index_ptr_guard.cpp
        src/bsdb/index_ptr_guard.h
        src/bsdb/ptr_guard.cpp
        src/bsdb/ptr_guard.h)
