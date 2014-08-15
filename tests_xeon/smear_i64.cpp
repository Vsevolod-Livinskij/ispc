#include <stdio.h>
#define __STDC_LIMIT_MACROS // enable intN_t limits from stdint.h
#include <stdint.h>
#include "knc-i1x16.h"

int width() { return 16; }

/*export*/ void f_fi(float RET[], float a[], int b[]) {
    __vec16_i64 a1 = __smear_i64<__vec16_i64>((int64_t) b[0]);
    for (int i = 0; i < 16; ++i){
        RET[i] = a1[i];
    }
    __vec16_i64 a2 = __smear_i64<__vec16_i64>((int64_t) INT64_MAX);
    __vec16_i64 a3 = __smear_i64<__vec16_i64>((int64_t) INT64_MIN);
    
    for (int i = 0; i < 16; ++i)
        RET[i] = 0;

    for (int i = 0; i < 16; ++i)
        if (i % 3 == 0)
            if (a1[i] == 2)
                RET[i] = 1;
        else if (i % 3 == 1)
            if (a2[i] == INT64_MAX)
                RET[i] = 1; 
        else  
            if (a3[i] == INT64_MIN)
                RET[i] = 1;
}

void result(float RET[]) {
    for (int i = 0; i < 16; ++i)
        RET[i] = 1;
}

