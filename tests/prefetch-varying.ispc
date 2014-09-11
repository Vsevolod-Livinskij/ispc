
export uniform int width() { return programCount; }

int zero = 0;
void *gptr;

export void f_f(uniform float RET[], uniform float aFOO[]) {
    uniform int64 a[programCount];
    for (uniform int i = 0; i < programCount; ++i)
        a[i] = aFOO[i];

    int64 *ptr = (aFOO[0] == 1234) ? (int64 * varying)gptr : (a + programIndex);
    prefetch_l1(ptr);
    prefetch_l2(ptr);
    prefetch_l3(ptr);
    prefetch_nt(ptr);
    int g = *ptr;
    RET[programIndex] = g;
}

export void result(uniform float RET[]) {
    RET[programIndex] = 1 + programIndex;
}
