//
// Created by bogusz on 17.02.24.
//
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>

#include "../src/bsdb/src_provider.h"


struct SRCProviderImplFileTest : ::testing::Test {
    std::string file_string = "/home/bogusz/CLionProjects/mainBSDB/test_db/test_tmp.bsdb";
    bsdb::src_provider_impl_file *src_file;

    void save_file(const std::string &suffix) {
        src_file->close();
        const auto to = file_string+"@"+suffix+".bsdb";
        std::remove(to.c_str());
        std::filesystem::copy(file_string,to);
        src_file->open(file_string, std::ios::in | std::ios::out | std::ios::binary);
    }

    protected:
    void SetUp() override {
        src_file = new bsdb::src_provider_impl_file(file_string,std::ios::in | std::ios::out | std::ios::binary);
        if (!src_file->is_open()) {
            src_file->open(file_string, std::ios::out | std::ios::binary);
            src_file->close();
            src_file->open(file_string, std::ios::in | std::ios::out | std::ios::binary);
        }
    };

    void TearDown() override {
        delete src_file;
        std::remove(file_string.c_str());
    };
};

TEST_F(SRCProviderImplFileTest,ShiftN) {
    src_file->write_obj(38278021);
    save_file("init");
}