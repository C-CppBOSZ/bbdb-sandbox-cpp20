//
// Created by bogusz on 05.01.24.
//

#ifndef TYPE_M_H
#define TYPE_M_H

namespace bsdb::bodb {

enum class scheme_b : unsigned char{
    none, // -
    end, // -
    shifter, // -
    undefine, // -
    simple, // type: 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of byte; obj: {[type].{number of byte}} - value
    complex, // type: 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types;
    array, // type: 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 8 - type array | if{is_dynamic}: 8 - size array? 0 - array is dynamic;
    generic, // type: 8 - index id | 1 - type | 1 - is_nullable>>is_dynamic | 4 - size name | {size name} - name | 4 - number of type | {number of type}*8 - types;
    cantainer, // type: 8 - index id | 1 - type | 4 - size name | {size name} - name | 8 - type_container;
    static_b, //
};

inline constexpr unsigned int scheme_b_version = 1;

inline constexpr unsigned int number_of_obj_position = 0;

}

#endif //TYPE_M_H
