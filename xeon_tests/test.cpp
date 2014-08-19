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
    
    //free(tmp1); cause seagfault

}

/////////////////////////////////////////////////////////////////////////////////////////////

#define GATHER(GATHER_SCALAR_TYPE, GATHER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)   \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_data[16];                                                                     \
    int copy_m[16];                                                                         \
    for (int i = 0; i < 16; i++) {                                                          \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    GATHER_VEC_TYPE ptrs;                                                                   \
    TYPE *b[16];                                                                            \
    allocator(b);                                                                           \
    for (int i = 0; i < 16; i++) {                                                          \
        *b[i] = (TYPE) copy_data[i];                                                        \
        ptrs[i] = (GATHER_SCALAR_TYPE) b[i];                                                \
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
    for (int i = 0; i < 16; i++){                                                           \
        if (m[i] != 0 && output[i] != data[i])                                              \
            err_counter++;                                                                  \
        if (copy_data[i] != data[i])                                                        \
            err_counter++;                                                                  \
    }                                                                                       \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
         printf(" no fails\n");                                                             \
                                                                                            \
    for (int i = 0; i < 16; i++)                                                            \
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
    for (int i = 0; i < 16; i++) {                                                          \
        copy_data[i] = data[i];                                                             \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    TYPE *b[16];                                                                            \
    allocator<TYPE>(b);                                                                     \
    uint8_t *base = (uint8_t*) b[0];                                                        \
    uint32_t scale = sizeof(TYPE);                                                          \
    GATHER_VEC_TYPE offsets;                                                                \
    for (int i = 0; i < 16; i++) {                                                          \
        *b[i] = (TYPE) copy_data[i];                                                        \
        offsets[i] = (GATHER_SCALAR_TYPE) ((b[i] - b[0]));                                  \
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
    for (int i = 0; i < 16; i++){                                                           \
        if (m[i] != 0 && output[i] != data[i])                                              \
            err_counter++;                                                                  \
        if (copy_data[i] != data[i])                                                        \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (int i = 0; i < 16; i++)                                                            \
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

#define SCATTER(GATHER_SCALAR_TYPE, GATHER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)  \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_m[16];                                                                        \
    for (int i = 0; i < 16; i++) {                                                          \
        copy_m[i] = m[i];                                                                   \
    }                                                                                       \
                                                                                            \
    GATHER_VEC_TYPE ptrs;                                                                   \
    VEC_TYPE input;                                                                         \
    TYPE *b[16];                                                                            \
    allocator(b);                                                                           \
    for (int i = 0; i < 16; i++) {                                                          \
        ptrs[i] = (GATHER_SCALAR_TYPE) b[i];                                                \
        input[i] = (TYPE) data[i];                                                          \
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
    for (int i = 0; i < 16; i++){                                                           \
        TYPE *p =  (TYPE*) ptrs[i];                                                         \
        if (m[i] != 0 && *p  != data[i])                                                    \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (int i = 0; i < 16; i++)                                                            \
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

#define SCATTER_OFFSETS(GATHER_SCALAR_TYPE, GATHER_VEC_TYPE, TYPE, VEC_TYPE, FUNC_NAME, FUNC_CALL)  \
void FUNC_NAME(TYPE *data, int *m) {                                                        \
    printf (#FUNC_NAME, ":");                                                               \
                                                                                            \
    TYPE copy_m[16];                                                                        \
    for (int i = 0; i < 16; i++)                                                            \
        copy_m[i] = m[i];                                                                   \
                                                                                            \
    TYPE *b[16];                                                                            \
    allocator<TYPE>(b);                                                                     \
    uint8_t *base = (uint8_t*) b[0];                                                        \
    uint32_t scale = sizeof(TYPE);                                                          \
    GATHER_VEC_TYPE offsets;                                                                \
    VEC_TYPE input;                                                                         \
    for (int i = 0; i < 16; i++) {                                                          \
        offsets[i] = (GATHER_SCALAR_TYPE) ((b[i] - b[0]));                                  \
        input[i] = (TYPE) data[i];                                                          \
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
    for (int i = 0; i < 16; i++){                                                           \
        if (m[i] != 0 && *b[i] != data[i])                                                  \
            err_counter++;                                                                  \
    }                                                                                       \
                                                                                            \
    if (err_counter != 0)                                                                   \
        printf(" errors %d\n", err_counter);                                                \
    else                                                                                    \
        printf(" no fails\n");                                                              \
                                                                                            \
    for (int i = 0; i < 16; i++)                                                            \
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

/*
void scatter_base_offsets32_i8(int8_t *data, int *m) {
    printf ("scatter_base_offsets32_i8:");

    int8_t copy_m[16];
    for (int i = 0; i < 16; i++)
        copy_m[i] = m[i];

    int8_t *b[16];
    allocator<int8_t>(b);
    uint8_t *base = (uint8_t*) b[0];
    uint32_t scale = sizeof(int8_t);
    __vec16_i32 offsets;
    __vec16_i8 input;
    for (int i = 0; i < 16; i++) {
        offsets[i] = (int32_t) ((b[i] - b[0]));
        input[i] = (int8_t) data[i];
    }

    __vec16_i1 mask = __vec16_i1(copy_m[0],  copy_m[1],  copy_m[2],  copy_m[3],
                                 copy_m[4],  copy_m[5],  copy_m[6],  copy_m[7],
                                 copy_m[8],  copy_m[9],  copy_m[10], copy_m[11],
                                 copy_m[12], copy_m[13], copy_m[14], copy_m[15]);
   
    printf("\n base: %d, scale: %d\n offsets   |  input  |  b[i]\n", base, scale);
    for (int i = 0; i < 16; i++) 
        printf ("%10d | %10d | %10d\n", offsets[i], input[i], b[i]);
    printf ("-------------------------------------------");
    fflush(stdout);
    __scatter_base_offsets32_i8(base, scale, offsets, input, mask);

    int err_counter = 0;
    for (int i = 0; i < 16; i++){
        if (m[i] != 0 && *b[i] != data[i])
            err_counter++;
    }

    if (err_counter != 0)
        printf(" errors %d\n", err_counter);
    else
        printf(" no fails\n");

    for (int i = 0; i < 16; i++)
        free(b[i]);

}
*/
