
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    varying saturated int64 a_max = 0x7FFFFFFFFFFFFFFF, a_min = -0x8000000000000000; // max and min int64
    if (programIndex % 3 == 0) {
        RET[programIndex] = a_min - b;
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = a_max - (-b);
    }
    else {
        RET[programIndex] = a_max - b;
    } 
}

export void result(uniform float RET[]) {
    if (programIndex % 3 == 0) {
        RET[programIndex] = (varying saturated int64) -0x8000000000000000; // min signed int64
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (varying saturated int64) 0x7FFFFFFFFFFFFFFF; // max signed int64
    }
    else {
        RET[programIndex] = (varying saturated int64) 0x7FFFFFFFFFFFFFFA;
    } 
}
