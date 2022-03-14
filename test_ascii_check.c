// Copyright (c) 2022 Ruben Vorderman

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "ascii_check.h"
#include <stdio.h>
#include <string.h>

#define ASCII_STRING "In het Nederlands komen bijzondere leestekens niet vaak voor."

#define NON_ASCII_STRING "In sp\xe4terer Zeit trat Umlaut sehr h\xe4ufig analogisch ein."

#define TEST(check, fail_message) \
if (!(check)) {\
    printf(fail_message); \
    failed += 1; \
} else { \
    successfull += 1; \
}

int main() {
    size_t ascii_len = strlen(ASCII_STRING);
    size_t non_ascii_len = strlen(NON_ASCII_STRING);

    int successfull = 0;
    int failed = 0; 
    TEST(string_is_ascii(ASCII_STRING, ascii_len), 
         "string_is_ascii returned 0 with ASCII-only string.\n")
    TEST(!string_is_ascii(NON_ASCII_STRING, non_ascii_len), 
         "string_is_ascii returned 0 with ASCII-only string.\n")
    
    size_t i = 0;
    char buffer[128]; 
    // Series of check to make sure string_is_ascii does not check characters
    // after the string.
    while (i < (ascii_len)) {
        memcpy(buffer, ASCII_STRING, i);
        buffer[i] = 0xff; // definitely not ASCII
        TEST(string_is_ascii(buffer, i), 
             "string_is_ascii incorrectly evaluated one extra character.\n")
        i += 1;
    }
    i = 0;
    while (i < (ascii_len)) {
        memcpy(buffer, ASCII_STRING, i);
        buffer[i] = 0xff; // definitely not ASCII
        TEST(!string_is_ascii(buffer, i + 1), 
             "string_is_ascii did not evaluate the last character.\n")
        i += 1;
    }

    // Basic sanity checks
    i = 0;
    char c;
    while (i < 128) {
        c = (char)i;
        TEST(string_is_ascii(&c, 1), "string_is_ascii failed on ASCII char.\n")
        i += 1;
    }
    while (i < 256) {
        c = (char)i;
        TEST(!string_is_ascii(&c, 1), "string_is_ascii succeeded on non-ASCII char.\n")
        i += 1; 
    }
    

    if (failed) {
        return 1;
    } else {
        return 0;
    }
}
