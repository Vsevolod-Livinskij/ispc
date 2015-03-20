// Copyright (c) 2014-2015, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Intel Corporation nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// ==========================================================
// Author: Vsevolod Livinskiy
// ==========================================================

#include "knc_test_driver_core.h"

/////////////////////////////////////////////////////////////////////////////////////////////

#define BROADCAST_TEST(TYPE, VEC_TYPE, FUNC_NAME)                                           \
void FUNC_NAME(TYPE *data) {                                                                \
    printf ("%-40s", #FUNC_NAME ":");                                                       \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        copy_data[i] = data[i];                                                             \
                                                                                            \
    VEC_TYPE input;                                                                         \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        __insert_element(&input, i, (TYPE) copy_data[i]);                                   \
                                                                                            \
    VEC_TYPE output;                                                                        \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        output = __##FUNC_NAME(input, i);                                                   \
        for (int32_t j = 0; j < 16; j++){                                                   \
        if (__extract_element(output, j) != data[i])                                        \
            err_counter++;                                                                  \
        }                                                                                   \
        output = __##FUNC_NAME(input, -i);                                                  \
        for (int32_t j = 0; j < 16; j++){                                                   \
        if (check_and_print(__extract_element(output, j), data[(16 - i) % 16], err_counter))\
            err_counter++;                                                                  \
        }                                                                                   \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
}

BROADCAST_TEST(double , __vec16_d  , broadcast_double)
BROADCAST_TEST(float  , __vec16_f  , broadcast_float )
BROADCAST_TEST(int8_t , __vec16_i8 , broadcast_i8    )
BROADCAST_TEST(int16_t, __vec16_i16, broadcast_i16   )
BROADCAST_TEST(int32_t, __vec16_i32, broadcast_i32   )
//BROADCAST_TEST(int64_t, __vec16_i64, broadcast_i64   )

/////////////////////////////////////////////////////////////////////////////////////////////

void test_broadcast() {
    InputData inpData;

    broadcast_double(inpData.d_32);
    broadcast_float(inpData.f_32);
    broadcast_i8(inpData.i8);
    broadcast_i16(inpData.i16);
    broadcast_i32(inpData.i32);
    //broadcast_i64(inpData.i64);

}

