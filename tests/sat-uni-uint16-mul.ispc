
export uniform int width() { return programCount; }

export void f_fu(uniform float RET[], uniform float aFOO[], uniform float b) {
    uniform saturated unsigned int16 a_max = 0xFFFF, a_min = 0; // max and min unsigned int16
    uniform unsigned int64 b_conv = b;
    if (programIndex % 3 == 0) {
        RET[programIndex] = a_max * b_conv;
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = a_min * (-b_conv);
    }
    else {
        uniform saturated unsigned int16 c = b;
        RET[programIndex] = c * c;
    } 
}

export void result(uniform float RET[]) {
    if (programIndex % 3 == 0) {
        RET[programIndex] = (uniform saturated unsigned int16) 0xFFFF; // max unsigned int16
    }
    else if (programIndex % 3 == 1) {
        RET[programIndex] = (uniform saturated unsigned int16) 0; // min unsigned int16
    }
    else {
        RET[programIndex] = (uniform saturated unsigned int16) 25;
    } 
}
