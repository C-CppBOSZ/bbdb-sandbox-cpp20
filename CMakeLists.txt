cmake_minimum_required(VERSION 3.28)
project(mainBSDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
        src/bsdb/bodb/Bodb.cpp
        src/bsdb/bodb/Bodb.h
        src/bsdb/bodb/TypeProvider.cpp
        src/bsdb/bodb/TypeProvider.h
        src/bsdb/bodb/SchemeB.cpp
        src/bsdb/bodb/SchemeB.h
        src/bsdb/src_module/base/SrcProviderBase.cpp
        src/bsdb/src_module/base/SrcProviderBase.h
        src/bsdb/src_module/impl/SrcProviderFile.cpp
        src/bsdb/src_module/impl/SrcProviderFile.h
        src/huf/huf.h
        src/bsdb/src_module/guard/IndexPtrGuard.cpp
        src/bsdb/src_module/guard/IndexPtrGuard.h
        src/bsdb/src_module/guard/PtrGuard.cpp
        src/bsdb/src_module/guard/PtrGuard.h
        src/bsdb/src_module/base/SrcTransactionBase.cpp
        src/bsdb/src_module/base/SrcTransactionBase.h
        src/bsdb/src_module/impl/SrcTransaction.cpp
        src/bsdb/src_module/impl/SrcTransaction.h
        src/bsdb/src_module/guard/SrcQueryGuard.cpp
        src/bsdb/src_module/guard/SrcQueryGuard.h
        src/bsdb/src_module/interface.h)

set(TESTS
        tests/test.cpp
        tests/SrcProviderFileTest.cpp
        src/bsdb/src_module/interface.h
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

enable_testing()

add_executable(
        ${PROJECT_NAME}-ut
        ${SOURCES}
        ${TESTS}
)
target_link_libraries(
        ${PROJECT_NAME}-ut
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)