//
// Created by bogusz on 05.01.24.
//

#ifndef TYPE_M_H
#define TYPE_M_H

namespace bsdb::bodb {

enum class scheme_b : unsigned char{
    none,
    end,
    undefine,
    simple,
    complex,
    template_array,
    array,
    template_set,
    set,
    template_map,
    map,
    template_container,
    cantainer,
    static_b,

};

inline unsigned int scheme_b_version = 1;


}

#endif //TYPE_M_H
