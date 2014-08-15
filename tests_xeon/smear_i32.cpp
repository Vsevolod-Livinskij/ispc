#include <stdio.h>
#define __STDC_LIMIT_MACROS // enable intN_t limits from stdint.h
#include <stdint.h>
#include "knc-i1x16.h"

int width() { return 16; }

/*export*/ void f_fi(float RET[], float a[], int b[]) {
    __vec16_i32 a1 = __smear_i32<__vec16_i32>((int32_t) b[0]);
    for (int i = 0; i < 16; ++i){
        RET[i] = a1[i];
    }
    __vec16_i32 a2 = __smear_i32<__vec16_i32>((int32_t) INT32_MAX);
    __vec16_i32 a3 = __smear_i32<__vec16_i32>((int32_t) INT32_MIN);

    for (int i = 0; i < 16; ++i)
        if (i % 3 == 0)
            RET[i] = a1[i];
        else if (i % 3 == 1)
            RET[i] = a2[i];
        else
            RET[i] = a3[i];
}

void result(float RET[]) {
    for (int i = 0; i < 16; ++i)
        if (i % 3 == 0)
            RET[i] = 2;
        else if (i % 3 == 1)
            RET[i] = INT32_MAX;
        else
            RET[i] = INT32_MIN;
}

