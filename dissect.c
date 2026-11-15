/**
 * @file dissect.c
 * @brief Read a stream of  binary message from standard input and prints a human-readable line for each, followed by a count..
 *
 * @author Pablo Rodriguez <pmr6570@rit.edu>
 * @course CSCI-243 Homework 7
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Prints the truncation error line and exits with status 1.
 */
static void truncated(void) {
    printf("error: truncated message\n");
    exit(1);
}

/**
 * Reads the six header bytes after the first one; exits if the stream ends.
 * @param hdr  array of 6 ints filled with the byte values
 */
static void read_header_rest(int hdr[6]) {
    for (int i = 0; i < 6; i++) {
        hdr[i] = getchar();
        if (hdr[i] == EOF) {
            truncated();
        }
    }
}

/**
 * Decodes messages from standard input until a clean end of stream.
 * @return 0 on success (exits with 1 on a truncated message)
 */
int main(void) {
    unsigned long count = 0;
    int b0;

    while ((b0 = getchar()) != EOF) {
        int h[6];
        read_header_rest(h);

        unsigned version = ((unsigned)b0 >> 4) & 0xF;
        unsigned type = (unsigned)b0 & 0xF;
        unsigned len = ((unsigned)h[0] << 8) | (unsigned)h[1];
        unsigned long ts = ((unsigned long)h[2] << 24) |
                           ((unsigned long)h[3] << 16) |
                           ((unsigned long)h[4] << 8) |
                           (unsigned long)h[5];

        printf("msg version=%u type=%u len=%u ts=%lu payload=\"",
               version, type, len, ts);

        for (unsigned i = 0; i < len; i++) {
            int c = getchar();
            if (c == EOF) {
                printf("\"\n");
                truncated();
            }
            putchar((c >= 32 && c < 127) ? c : '.');
        }
        printf("\"\n");
        count++;
    }

    printf("count: %lu\n", count);
    return 0;
}
