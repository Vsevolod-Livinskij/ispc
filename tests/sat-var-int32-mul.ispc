
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    varying saturated int32 a_max = 0x7FFFFFFF, a_min = -0x80000000; // max and min signed int32
    uniform unsigned int64 b_conv = b;
    if (programIndex % 3 == 0) {
        RET[programIndex] = a_max * b_conv;
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = a_min * b_conv;
    }
    else {
        varying saturated int32 c = b;
        RET[programIndex] = c * c;
    } 
}

export void result(uniform float RET[]) {
    if (programIndex % 3 == 0) {
        RET[programIndex] = (varying saturated int32) 0x7FFFFFFF; // max signed int32
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (varying saturated int32) -0x80000000; // min signed int32
    }
    else {
        RET[programIndex] = (varying saturated int32) 25;
    } 
}
