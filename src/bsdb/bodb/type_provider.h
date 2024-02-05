//
// Created by bogusz on 04.01.24.
//

#ifndef HEADER_TYPE_H
#define HEADER_TYPE_H
#include <functional>
#include <string>
#include <vector>

namespace bsdb::bodb {

class type_provider {
private:
   // std::vector<unsigned long> struct_ptr;
   // size_t unstruct_ptr;
   long id;
   bool is_nullable;
   bool is_dynamic;
   std::string name;

protected:
    std::function<void(const std::vector<unsigned long>&,const size_t&)> jump_f;
    std::function<void(const std::vector<unsigned long>&,const size_t&,const long&)> explor_f;
    std::function<void(const std::vector<unsigned long>&,const size_t&)> exit_f;


   // virtual void jump(std::vector<unsigned long> &struct_ptr,size_t &unstruct_ptr);
   // virtual void explor(std::vector<unsigned long> &struct_ptr,size_t &unstruct_ptr,const long &index = 0);
   // virtual void exit(std::vector<unsigned long> &struct_ptr,size_t &unstruct_ptr);

};

}

#endif //HEADER_TYPE_H
