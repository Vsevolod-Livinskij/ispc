
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    varying saturated int16 a_max = 0x7FFF, a_min = -0x8000; // max and min int16
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
        RET[programIndex] = (varying saturated int16) -0x8000; // min signed int16
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (varying saturated int16) 0x7FFF; // max signed int16
    }
    else {
        RET[programIndex] = (varying saturated int16) 0x7FFA;
    } 
}
