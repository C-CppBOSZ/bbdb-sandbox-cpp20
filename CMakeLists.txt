cmake_minimum_required(VERSION 3.28)
project(mainBSDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
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

set(TESTS
        tests/test.cpp
        tests/src_provider_impl_file_test.cpp)

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