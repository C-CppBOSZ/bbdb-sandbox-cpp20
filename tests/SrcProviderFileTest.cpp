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
        const auto to = file_string + "@" + suffix + ".bbdb";
        std::remove(to.c_str());
        std::filesystem::copy(file_string, to);
        src_file->open(file_string, std::ios::in | std::ios::out | std::ios::binary);
    }

protected:
    void SetUp() override {
        std::remove(file_string.c_str());

        src_file = new bbdb::src_module::impl::SrcProviderFile(file_string,
                                                               std::ios::in | std::ios::out | std::ios::binary );
        if (!src_file->is_open()) {
            src_file->open(file_string, std::ios::out | std::ios::binary);
            src_file->close();
            src_file->open(file_string, std::ios::in | std::ios::out | std::ios::binary);
        }
    };

    void TearDown() override {
        delete src_file;
        // std::remove(file_string.c_str());
    };
};

TEST_F(SRCProviderImplFileTest, ShiftRight) {
    src_file->write_obj(1234);
    src_file->simple_shift_right_content(0,3);
    const int ptr = src_file->get_ptr();
    src_file->shift_ptr(3);
    int tmp = 0;
    src_file->read_obj(tmp);
    EXPECT_EQ(ptr,0);
    EXPECT_EQ(tmp,1234);
    // save_file("ShiftRigth");
}

TEST_F(SRCProviderImplFileTest, ShiftRight2) {
    int tab[]       = {1,2,3,4,    5,6,7,8,9,  10,11,12,13,14,15};
    int tab_out[]   = {1,2,3,4,0,0,5,6,7,8,9,0,10,11,12,13,14,15}; // 18

    src_file->write_obj(tab);
    // save_file("ShiftRigth2-1");

    src_file->simple_shift_right_content(4*4,8,10);
    src_file->simple_shift_right_content(10*4,4,10);

    unsigned long size = 0;
    // TODO get size trzeba dodać
    src_file->ptr_to_end();
    size = src_file->get_ptr();
    EXPECT_EQ(size,(15*4+12));

    src_file->set_ptr(0);
    for (int i = 0; i < 18; ++i) {
        int tmp = 0;
        src_file->read_obj(tmp);
        if (tab_out[i] != 0)
        EXPECT_EQ(tmp,tab_out[i]);
    }

    // save_file("ShiftRigth2-2");
}

TEST_F(SRCProviderImplFileTest, ShiftLeft) {
    src_file->write_obj(849032892);
    src_file->simple_shift_left_content(2,2);
    const int ptr = src_file->get_ptr();
    src_file->shift_ptr(3);
    // int tmp = 0;
    // src_file->read_obj(tmp);
    EXPECT_EQ(ptr,2);
    // EXPECT_EQ(tmp,1234);
    save_file("ShiftLeft");
}