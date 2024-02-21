//
// Created by bogusz on 17.02.24.
//
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>

#include "../src/bsdb/src_module/impl/SrcTransaction.h"
#include "../src/bsdb/src_module/impl/SrcProviderFile.h"


struct SRCProviderImplFileTest : ::testing::Test {
    std::string file_string = "/home/bogusz/CLionProjects/mainBSDB/test_db/test_tmp.bbdb";
    bbdb::src_module::impl::SrcProviderFile *src_file;

    void save_file(const std::string &suffix) {
        src_file->close();
        const auto to = file_string+"@"+suffix+".bbdb";
        std::remove(to.c_str());
        std::filesystem::copy(file_string,to);
        src_file->open(file_string, std::ios::in | std::ios::out | std::ios::binary);
    }

    protected:
    void SetUp() override {
        src_file = new bbdb::src_module::impl::SrcProviderFile(file_string,std::ios::in | std::ios::out | std::ios::binary);
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

    bbdb::src_module::impl::SrcTransaction<bbdb::src_module::impl::SrcProviderFile> controller(src_file);
    {
        auto query = controller.src_transaction();
        query.fun([](auto &s) {
            s.write_obj(12309202);
        }).fun([](auto &s) {
            s.write_obj(12309202);
        });
        query.fun([](auto &s) {
            s.write_obj(0);
        });
    }
    controller.write_obj(0);
    save_file("init");
}