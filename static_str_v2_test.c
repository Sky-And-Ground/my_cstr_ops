#include "static_str_v2.h"
#include <stdio.h>

void test_case_1(void) {
    char buf[32];
    int len = 0;

    str_from_float_v2(buf, &len, 32, 257.949, 2);
    printf("%s\n", buf);
}

int main(void) {
    test_case_1();
    return 0;
}