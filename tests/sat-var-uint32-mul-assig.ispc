
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    uniform unsigned int64 b_conv = b;
    if (programIndex % 3 == 0) {
        varying saturated unsigned int32 tmp1 = 0xFFFFFFFF; // max unsigned int32
        tmp1 *= b_conv;
        RET[programIndex] = tmp1;
    }
    else if (programIndex % 3 == 1) {
        varying saturated unsigned int32 tmp2 = 0; // min unsigned int32
        tmp2 *= (-b_conv);
        RET[programIndex] = tmp2;
    }
    else {
        varying saturated unsigned int32 c = b;
        c *= c;
        RET[programIndex] = c;
    } 
}

export void result(uniform float RET[]) {
    if (programIndex % 3 == 0) {
        RET[programIndex] = (varying saturated unsigned int32) 0xFFFFFFFF; // max unsigned int32
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (varying saturated unsigned int32) 0; // min unsigned int32
    }
    else {
        RET[programIndex] = (varying saturated unsigned int32) 25;
    } 
}
