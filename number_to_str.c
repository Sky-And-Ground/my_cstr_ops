#include <stdio.h>
#include <string.h>
#include <math.h>

void str_reverse(char* buf, int len);
int int_to_str(int number, char* buf, int limit);
int uint_to_str_with_base(unsigned int number, int base, char* buf, int limit);
int float_to_str(float number, int precision, char* buf, int limit);

void str_reverse(char* buf, int len) {
    int i, j;
    char c;

    for (i = 0, j = len - 1; i < j; ++i, --j) {
        c = buf[i];
        buf[i] = buf[j];
        buf[j] = c;
    }
}

int int_to_str(int number, char* buf, int limit) {
    char inner_buf[64];
    int sign = number;
    int len = 0;

    if (sign < 0) {
        number = -number;
    }

    do {
        inner_buf[len] = number % 10 + '0';
        ++len;
        number /= 10;
    } while(number > 0);

    if (sign < 0) {
        inner_buf[len] = '-';
        ++len;
    }

    str_reverse(inner_buf, len);
    
    /* copy back. */
    if (len + 1 <= limit) {
        memcpy(buf, inner_buf, len * sizeof(char));
        buf[len] = '\0';
        return len;
    }
    else {
        memcpy(buf, inner_buf, (limit - 1) * sizeof(char));
        buf[limit - 1] = '\0';
        return limit - 1;
    }
}

int uint_to_str_with_base(unsigned int number, int base, char* buf, int limit) {
    char inner_buf[64];
    int len = 0;

    if (base != 2 && base != 8 && base != 10 && base != 16) {
        return -1;
    }

    if (base == 16) {
        char c;

        do {
            c = number % 16;
            inner_buf[len] = (c <= 9 ? c + '0' : c - 10 + 'a');
            ++len;
            number /= 16;
        } while(number > 0);
    }
    else {
        do {
            inner_buf[len] = number % base + '0';
            ++len;
            number /= base;
        } while(number > 0);
    }

    str_reverse(inner_buf, len);

    if (len + 1 <= limit) {
        memcpy(buf, inner_buf, len * sizeof(char));
        buf[len] = '\0';
        return len;
    }
    else {
        memcpy(buf, inner_buf, (limit - 1) * sizeof(char));
        buf[limit - 1] = '\0';
        return limit - 1;
    }
}

int float_to_str(float number, int precision, char* buf, int limit) {
    #define FLOAT_ZERO_LIMITATION  0.000001f

    char inner_buf[64];
    int i = 0, len = 0;
    float added;

    if (number < FLOAT_ZERO_LIMITATION && number > -FLOAT_ZERO_LIMITATION) {
        memcpy(buf, "0", 2);
        return 1;
    }

    if (isinf(number)) {
        if (number > 0) {
            memcpy(buf, "inf", 4);
            return 3;
        }
        else {
            memcpy(buf, "-inf", 5);
            return 4;
        }
    }

    if (isnan(number)) {
        memcpy(buf, "nan", 4);
        return 3;
    }

    added = 0.5f;
    for (i = 0; i < precision; ++i) {
        added /= 10.0;
    }

    if (number < 0) {
        number = -number;
        inner_buf[len] = '-';
        ++len;
    }

    number += added;
    len += int_to_str((int)number, inner_buf + len, sizeof(inner_buf) / sizeof(char) - len);

    if (precision > 0) {
        inner_buf[len] = '.';
        ++len;

        number -= (int)number;
        
        for (i = 0; i < precision; ++i) {
            number *= 10;
        }

        len += int_to_str((int)number, inner_buf + len, sizeof(inner_buf) / sizeof(char) - len);
    }

    /* copy back. */
    if (len + 1 <= limit) {
        memcpy(buf, inner_buf, len * sizeof(char));
        buf[len] = '\0';
        return len;
    }
    else {
        memcpy(buf, inner_buf, (limit - 1) * sizeof(char));
        buf[limit - 1] = '\0';
        return limit - 1;
    }
}

int main(void) {
    char buf[32];
    int len = uint_to_str_with_base(17, 8, buf, 32);
    printf("%d, %s\n", len, buf);
    return 0;
}
