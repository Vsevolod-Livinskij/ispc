
export uniform int width() { return programCount; }



export void f_v(uniform float RET[]) {
    int i=0, j=0;
    do {
        ++j;
        if (i >= 5) cbreak;
        ++j;
        } while (++i < 10);
    RET[programIndex] = j;
}

export void result(uniform float RET[]) { RET[programIndex] = 11; }

