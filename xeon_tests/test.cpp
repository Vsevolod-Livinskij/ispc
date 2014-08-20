#define __STDC_LIMIT_MACROS // enable intN_t limits from stdint.h
#include <stdint.h>

#ifdef KNC_H
    #include "knc.h"
#else
    #include "knc-i1x16.h"
    #include <stdio.h>
#endif

template <typename T>
void allocator(T **array) {
    uint64_t seed = 123456789;
    int m = 100;
    int a = 1103515245;
    int c = 12345;
    T* tmp[4];
    seed = (a * seed + c) % m;
    void* tmp1 = (void*) malloc(seed);

    for (int j = 0; j < 4; j++) {
        for (int i = 4 * j; i < 4 * (j + 1); i++)
            array[i] = (T*) malloc(sizeof(T));
        seed = (a * seed + c) % m;
        tmp[j] = (T*) malloc(seed * sizeof(T));
    }

    for (int j = 0; j < 4; j++)
        free(tmp[j]);
    
    free(tmp1);

}

/////////////////////////////////////////////////////////////////////////////////////////////
/*
    int8_t copy_data[16];
    int copy_m[16];
    for (uint32_t i = 0; i < 16; i++) {
        copy_data[i] = data[i];
        copy_m[i] = m[i];
    }


    int err_counter = 0;
    for (uint32_t i = 0; i < 16; i++){
        if (m[i] != 0 && __extract_element(output, i) != data[i])
            err_counter++;
    }
    if (err_counter != 0)
        printf(" errors %d\n", err_counter);
    else
         printf(" no fails\n");
*/
/////////////////////////////////////////////////////////////////////////////////////////////
#define INSERT_EXTRACT_ELEMENT(TYPE, VEC_TYPE, FUNC_NAME)                                   \
void FUNC_NAME(TYPE *data) {                                                                \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        copy_data[i] = data[i];                                                             \
                                                                                            \
    VEC_TYPE input;                                                                         \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        __insert_element(&input, i, (TYPE)copy_data[i]);                                    \
                                                                                            \
    TYPE output[16];                                                                        \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        output[i] = __extract_element(input, i);                                            \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (output[i] != data[i])                                                           \
            err_counter++;                                                                  \
        if (copy_data[i] != data[i])                                                        \
            err_counter++;                                                                  \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
         printf(" no fails\n");                                                             \
}

INSERT_EXTRACT_ELEMENT(double , __vec16_d  , insert_extract_element_double)
INSERT_EXTRACT_ELEMENT(float  , __vec16_f  , insert_extract_element_float )
INSERT_EXTRACT_ELEMENT(int8_t , __vec16_i8 , insert_extract_element_i8    )
INSERT_EXTRACT_ELEMENT(int16_t, __vec16_i16, insert_extract_element_i16   )
INSERT_EXTRACT_ELEMENT(int32_t, __vec16_i32, insert_extract_element_i32   )
INSERT_EXTRACT_ELEMENT(int64_t, __vec16_i64, insert_extract_element_i64   )

/////////////////////////////////////////////////////////////////////////////////////////////

#define LOAD(TYPE, VEC_TYPE, FUNC_NAME, ALIGN_NUM)                                          \
void FUNC_NAME(TYPE *data) {                                                                \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        copy_data[i] = data[i];                                                             \
                                                                                            \
    VEC_TYPE ptrs;                                                                          \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        __insert_element(&ptrs, i, (TYPE)copy_data[i]);                                     \
                                                                                            \
    VEC_TYPE output;                                                                        \
    output = __load<ALIGN_NUM>(&ptrs);                                                      \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (__extract_element(output, i) != data[i])                                        \
            err_counter++;                                                                  \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
         printf(" no fails\n");                                                             \
}

LOAD(double , __vec16_d  , load_double, 128)
LOAD(float  , __vec16_f  , load_float , 64)
LOAD(int8_t , __vec16_i8 , load_i8    , 16)
LOAD(int16_t, __vec16_i16, load_i16   , 32)
LOAD(int32_t, __vec16_i32, load_i32   , 64)
LOAD(int64_t, __vec16_i64, load_i64   , 128)

/////////////////////////////////////////////////////////////////////////////////////////////
#define STORE(TYPE, VEC_TYPE, FUNC_NAME, ALIGN_NUM)                                         \
void FUNC_NAME(TYPE *data) {                                                                \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        copy_data[i] = data[i];                                                             \
                                                                                            \
    VEC_TYPE input;                                                                         \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        __insert_element(&input, i, (TYPE)data[i]);                                         \
                                                                                            \
    VEC_TYPE output;                                                                        \
    __store<ALIGN_NUM>(&output, input);                                                     \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (__extract_element(output, i) != data[i])                                        \
            err_counter++;                                                                  \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
}

STORE(double , __vec16_d  , store_double, 128)
STORE(float  , __vec16_f  , store_float , 64)
STORE(int8_t , __vec16_i8 , store_i8    , 16)
STORE(int16_t, __vec16_i16, store_i16   , 32)
STORE(int32_t, __vec16_i32, store_i32   , 64)
STORE(int64_t, __vec16_i64, store_i64   , 128)

/////////////////////////////////////////////////////////////////////////////////////////////

#define GATHER(GATHER_SCALAR_TYPE, GATHER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)   \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    int copy_m[16];                                                                         \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    GATHER_VEC_TYPE ptrs;                                                                   \
    TYPE *b[16];                                                                            \
    allocator<TYPE>(b);                                                                     \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        *b[i] = (TYPE) copy_data[i];                                                        \
        __insert_element(&ptrs, i, (GATHER_SCALAR_TYPE)b[i]);                               \
    }                                                                                       \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    VEC_TYPE output;                                                                        \
    output = FUNC_CALL(ptrs, mask);                                                         \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (m[i] != 0 && __extract_element(output, i) != data[i])                           \
            err_counter++;                                                                  \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
         printf(" no fails\n");                                                             \
                                                                                            \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        free(b[i]);                                                                         \
}

GATHER(int32_t, __vec16_i32, double , __vec16_d  , gather32_double, __gather32_double)
GATHER(int32_t, __vec16_i32, float  , __vec16_f  , gather32_float , __gather32_float)
GATHER(int32_t, __vec16_i32, int8_t , __vec16_i8 , gather32_i8    , __gather32_i8)
GATHER(int32_t, __vec16_i32, int16_t, __vec16_i16, gather32_i16   , __gather32_i16)
GATHER(int32_t, __vec16_i32, int32_t, __vec16_i32, gather32_i32   , __gather32_i32)
GATHER(int32_t, __vec16_i32, int64_t, __vec16_i64, gather32_i64   , __gather32_i64)

GATHER(int64_t, __vec16_i64, double , __vec16_d  , gather64_double, __gather64_double)
GATHER(int64_t, __vec16_i64, float  , __vec16_f  , gather64_float , __gather64_float)
GATHER(int64_t, __vec16_i64, int8_t , __vec16_i8 , gather64_i8    , __gather64_i8)
GATHER(int64_t, __vec16_i64, int16_t, __vec16_i16, gather64_i16   , __gather64_i16)
GATHER(int64_t, __vec16_i64, int32_t, __vec16_i32, gather64_i32   , __gather64_i32)
GATHER(int64_t, __vec16_i64, int64_t, __vec16_i64, gather64_i64   , __gather64_i64)

/////////////////////////////////////////////////////////////////////////////////////////////

#define GATHER_OFFSETS(GATHER_SCALAR_TYPE, GATHER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)   \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    int copy_m[16];                                                                         \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    TYPE *b[16];                                                                            \
    allocator<TYPE>(b);                                                                     \
    uint8_t *base = (uint8_t*) b[0];                                                        \
    uint32_t scale = sizeof(TYPE);                                                          \
    GATHER_VEC_TYPE offsets;                                                                \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        *b[i] = (TYPE) copy_data[i];                                                        \
        __insert_element(&offsets, i, (GATHER_SCALAR_TYPE)(b[i] - b[0]));                   \
    }                                                                                       \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    VEC_TYPE output;                                                                        \
    output = FUNC_CALL(base, scale, offsets, mask);                                         \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (m[i] != 0 && __extract_element(output, i) != data[i])                           \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        free(b[i]);                                                                         \
                                                                                            \
}

GATHER_OFFSETS(int32_t, __vec16_i32, double , __vec16_d  , gather_base_offsets32_double, __gather_base_offsets32_double)
GATHER_OFFSETS(int32_t, __vec16_i32, float  , __vec16_f  , gather_base_offsets32_float , __gather_base_offsets32_float)
GATHER_OFFSETS(int32_t, __vec16_i32, int8_t , __vec16_i8 , gather_base_offsets32_i8    , __gather_base_offsets32_i8)
GATHER_OFFSETS(int32_t, __vec16_i32, int16_t, __vec16_i16, gather_base_offsets32_i16   , __gather_base_offsets32_i16)
GATHER_OFFSETS(int32_t, __vec16_i32, int32_t, __vec16_i32, gather_base_offsets32_i32   , __gather_base_offsets32_i32)
GATHER_OFFSETS(int32_t, __vec16_i32, int64_t, __vec16_i64, gather_base_offsets32_i64   , __gather_base_offsets32_i64)

GATHER_OFFSETS(int64_t, __vec16_i64, double , __vec16_d  , gather_base_offsets64_double, __gather_base_offsets64_double)
GATHER_OFFSETS(int64_t, __vec16_i64, float  , __vec16_f  , gather_base_offsets64_float , __gather_base_offsets64_float)
GATHER_OFFSETS(int64_t, __vec16_i64, int8_t , __vec16_i8 , gather_base_offsets64_i8    , __gather_base_offsets64_i8)
GATHER_OFFSETS(int64_t, __vec16_i64, int16_t, __vec16_i16, gather_base_offsets64_i16   , __gather_base_offsets64_i16)
GATHER_OFFSETS(int64_t, __vec16_i64, int32_t, __vec16_i32, gather_base_offsets64_i32   , __gather_base_offsets64_i32)
GATHER_OFFSETS(int64_t, __vec16_i64, int64_t, __vec16_i64, gather_base_offsets64_i64   , __gather_base_offsets64_i64)

/////////////////////////////////////////////////////////////////////////////////////////////

#define SCATTER(SCATTER_SCALAR_TYPE, SCATTER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)  \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_m[16];                                                                        \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    SCATTER_VEC_TYPE ptrs;                                                                  \
    VEC_TYPE input;                                                                         \
    TYPE *b[16];                                                                            \
    allocator(b);                                                                           \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        __insert_element(&ptrs,  i, (SCATTER_SCALAR_TYPE) b[i]);                            \
        __insert_element(&input, i, (TYPE) data[i]);                                        \
    }                                                                                       \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    FUNC_CALL(ptrs, input, mask);                                                           \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        TYPE *p =  (TYPE*) __extract_element(ptrs, i);                                      \
        if (m[i] != 0 && *p  != data[i])                                                    \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        free(b[i]);                                                                         \
                                                                                            \
}

SCATTER(int32_t, __vec16_i32, double , __vec16_d  , scatter32_double, __scatter32_double)
SCATTER(int32_t, __vec16_i32, float  , __vec16_f  , scatter32_float , __scatter32_float)
SCATTER(int32_t, __vec16_i32, int8_t , __vec16_i8 , scatter32_i8    , __scatter32_i8)
SCATTER(int32_t, __vec16_i32, int16_t, __vec16_i16, scatter32_i16   , __scatter32_i16)
SCATTER(int32_t, __vec16_i32, int32_t, __vec16_i32, scatter32_i32   , __scatter32_i32)
SCATTER(int32_t, __vec16_i32, int64_t, __vec16_i64, scatter32_i64   , __scatter32_i64)

SCATTER(int64_t, __vec16_i64, double , __vec16_d  , scatter64_double, __scatter64_double)
SCATTER(int64_t, __vec16_i64, float  , __vec16_f  , scatter64_float , __scatter64_float)
SCATTER(int64_t, __vec16_i64, int8_t , __vec16_i8 , scatter64_i8    , __scatter64_i8)
SCATTER(int64_t, __vec16_i64, int16_t, __vec16_i16, scatter64_i16   , __scatter64_i16)
SCATTER(int64_t, __vec16_i64, int32_t, __vec16_i32, scatter64_i32   , __scatter64_i32)
SCATTER(int64_t, __vec16_i64, int64_t, __vec16_i64, scatter64_i64   , __scatter64_i64)

/////////////////////////////////////////////////////////////////////////////////////////////

#define SCATTER_OFFSETS(SCATTER_SCALAR_TYPE, SCATTER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)  \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_m[16];                                                                        \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        copy_m[i] = m[i];                                                                   \
                                                                                            \
    TYPE *b[16];                                                                            \
    allocator<TYPE>(b);                                                                     \
    uint8_t *base = (uint8_t*) b[0];                                                        \
    uint32_t scale = sizeof(TYPE);                                                          \
    SCATTER_VEC_TYPE offsets;                                                               \
    VEC_TYPE input;                                                                         \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        __insert_element(&offsets, i, (SCATTER_SCALAR_TYPE) (b[i] - b[0]));                 \
        __insert_element(&input,   i, (TYPE) data[i]);                                      \
    }                                                                                       \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    FUNC_CALL(base, scale, offsets, input, mask);                                           \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (m[i] != 0 && *b[i] != data[i])                                                  \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        free(b[i]);                                                                         \
                                                                                            \
}

SCATTER_OFFSETS(int32_t, __vec16_i32, double , __vec16_d  , scatter_base_offsets32_double, __scatter_base_offsets32_double)
SCATTER_OFFSETS(int32_t, __vec16_i32, float  , __vec16_f  , scatter_base_offsets32_float , __scatter_base_offsets32_float)
SCATTER_OFFSETS(int32_t, __vec16_i32, int8_t , __vec16_i8 , scatter_base_offsets32_i8    , __scatter_base_offsets32_i8)
SCATTER_OFFSETS(int32_t, __vec16_i32, int16_t, __vec16_i16, scatter_base_offsets32_i16   , __scatter_base_offsets32_i16)
SCATTER_OFFSETS(int32_t, __vec16_i32, int32_t, __vec16_i32, scatter_base_offsets32_i32   , __scatter_base_offsets32_i32)
SCATTER_OFFSETS(int32_t, __vec16_i32, int64_t, __vec16_i64, scatter_base_offsets32_i64   , __scatter_base_offsets32_i64)

SCATTER_OFFSETS(int64_t, __vec16_i64, double , __vec16_d  , scatter_base_offsets64_double, __scatter_base_offsets64_double)
SCATTER_OFFSETS(int64_t, __vec16_i64, float  , __vec16_f  , scatter_base_offsets64_float , __scatter_base_offsets64_float)
SCATTER_OFFSETS(int64_t, __vec16_i64, int8_t , __vec16_i8 , scatter_base_offsets64_i8    , __scatter_base_offsets64_i8)
SCATTER_OFFSETS(int64_t, __vec16_i64, int16_t, __vec16_i16, scatter_base_offsets64_i16   , __scatter_base_offsets64_i16)
SCATTER_OFFSETS(int64_t, __vec16_i64, int32_t, __vec16_i32, scatter_base_offsets64_i32   , __scatter_base_offsets64_i32)
SCATTER_OFFSETS(int64_t, __vec16_i64, int64_t, __vec16_i64, scatter_base_offsets64_i64   , __scatter_base_offsets64_i64)

/////////////////////////////////////////////////////////////////////////////////////////////

#define MASKED_LOAD(TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)                                   \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    int copy_m[16];                                                                         \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    TYPE ptrs[16];                                                                          \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        ptrs[i] = (TYPE) copy_data[i];                                                      \
    }                                                                                       \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    VEC_TYPE output;                                                                        \
    output = FUNC_CALL(ptrs, mask);                                                         \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (m[i] != 0 && __extract_element(output, i) != data[i])                           \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
}

MASKED_LOAD(double , __vec16_d  , masked_load_double, __masked_load_double)
MASKED_LOAD(float  , __vec16_f  , masked_load_float , __masked_load_float)
MASKED_LOAD(int8_t , __vec16_i8 , masked_load_i8    , __masked_load_i8)
MASKED_LOAD(int16_t, __vec16_i16, masked_load_i16   , __masked_load_i16)
MASKED_LOAD(int32_t, __vec16_i32, masked_load_i32   , __masked_load_i32)
MASKED_LOAD(int64_t, __vec16_i64, masked_load_i64   , __masked_load_i64)

/////////////////////////////////////////////////////////////////////////////////////////////

#define MASKED_STORE(TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)                                  \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    int copy_m[16];                                                                         \
    for (uint32_t i = 0; i < 16; i++) {                                                     \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    TYPE ptrs[16];                                                                          \
    VEC_TYPE input;                                                                         \
    for (uint32_t i = 0; i < 16; i++)                                                       \
        __insert_element(&input, i, (TYPE) copy_data[i]);                                   \
                                                                                            \
    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],             \
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],             \
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],            \
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);           \
                                                                                            \
    FUNC_CALL(ptrs, input, mask);                                                           \
                                                                                            \
    int err_counter = 0;                                                                    \
    for (uint32_t i = 0; i < 16; i++){                                                      \
        if (m[i] != 0 && ptrs[i] != data[i])                                                \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
}

MASKED_STORE(double , __vec16_d  , masked_store_double, __masked_store_double)
MASKED_STORE(float  , __vec16_f  , masked_store_float , __masked_store_float)
MASKED_STORE(int8_t , __vec16_i8 , masked_store_i8    , __masked_store_i8)
MASKED_STORE(int16_t, __vec16_i16, masked_store_i16   , __masked_store_i16)
MASKED_STORE(int32_t, __vec16_i32, masked_store_i32   , __masked_store_i32)
MASKED_STORE(int64_t, __vec16_i64, masked_store_i64   , __masked_store_i64)

MASKED_STORE(double , __vec16_d  , masked_store_blend_double, __masked_store_blend_double)
MASKED_STORE(float  , __vec16_f  , masked_store_blend_float , __masked_store_blend_float)
MASKED_STORE(int8_t , __vec16_i8 , masked_store_blend_i8    , __masked_store_blend_i8)
MASKED_STORE(int16_t, __vec16_i16, masked_store_blend_i16   , __masked_store_blend_i16)
MASKED_STORE(int32_t, __vec16_i32, masked_store_blend_i32   , __masked_store_blend_i32)
MASKED_STORE(int64_t, __vec16_i64, masked_store_blend_i64   , __masked_store_blend_i64)
/////////////////////////////////////////////////////////////////////////////////////////////

void movmsk(int *m) {
    printf ("movmsk: ");
    
    int copy_m[16];
    for (uint32_t i = 0; i < 16; i++) 
        copy_m[i] = m[i];

    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);

    __vec16_i1 copy_mask = mask;

    __vec16_i1 output;
    output = __movmsk(copy_mask);

    if (output != mask)
        printf(" error 1\n");
    else
        printf(" no fails\n");
}
