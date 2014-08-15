#include <stdio.h>
#include "knc-i1x16.h"

int width() { return 16; }

/*export*/ void f_fi(float RET[], float a[], int b[]) {
    __vec16_i1 a1 = __smear_i1<__vec16_i1>((int32_t) 0);
    for (int i = 0; i < 16; ++i){
        printf("%x\n", ((int*)&a1)[i]);
        RET[i] = ((uint*)&a1)[i];
  }
}

void result(float RET[]) {
    for (int i = 0; i < 16; ++i)
        RET[i] = 5;
}
