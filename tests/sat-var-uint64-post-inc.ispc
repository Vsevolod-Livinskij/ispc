
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    if (programIndex % 3 == 0) {
        varying saturated unsigned int64 tmp1 = 0xFFFFFFFFFFFFFFFF; // max unsigned int64
        tmp1++;
        RET[programIndex] = tmp1;
    }
    else if (programIndex % 3 == 1) {
        varying saturated unsigned int64 tmp2 = 0; // min unsigned int64
        RET[programIndex] = tmp2++;
    }
    else {
        varying saturated unsigned int64 tmp3 = 0; // min unsigned int64
        tmp3++;
        RET[programIndex] = tmp3;
    }   
}

export void result(uniform float RET[]) {
    if (programIndex % 3 == 0) {
        RET[programIndex] = (varying saturated unsigned int64) 0xFFFFFFFFFFFFFFFF; // max unsigned int64
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (varying saturated unsigned int64) 0; // min unsigned int64
    }
    else {
        RET[programIndex] = (varying saturated unsigned int64) 1; // min unsigned int64 + 1
    }
}
