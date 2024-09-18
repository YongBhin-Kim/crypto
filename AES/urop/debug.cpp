#include "debug.h"

void dump_bytes(const uint8_t *src, const size_t len) {
    size_t i;

    for (i = 0; i < len; i++) {
        printf("%02x:", src[i]);

        if ( (i + 1) % 16 == 0 ) {
            printf("\n");
        }
    }
}
