// Copyright (c) 2022 Leiden University Medical Center

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

#include <stddef.h>
#include <stdint.h>
#include <emmintrin.h>

#define ASCII_MASK_1BYTE 0x80

/**
 * @brief Check if a string of given length only contains ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for 
 *               terminating NULL bytes.
 * @returns 1 if the string is ASCII-only
 */
static int
string_is_ascii(const char * string, size_t length) {
    size_t n = length;
    const char * char_ptr = string;
    typedef __m128i longword;

    // The first loop aligns the memory address. Char_ptr is cast to a size_t
    // to return the memory address. longword is 8 bytes long, and the processor
    // handles this better when its address is a multiplier of 8. This loops
    // handles the first few bytes that are not on such a multiplier boundary.
    while ((size_t)char_ptr % sizeof(longword) && n != 0) {
        if (*char_ptr & ASCII_MASK_1BYTE) {
            return 0;
        }
        char_ptr += 1;
        n -= 1;
    }
    const longword * longword_ptr = (longword *)char_ptr;
    while (n >= sizeof(longword)) {
        // _mm_movemask_epi8 checks the most significant 8th bit of each char
        // and returns a 16-bit integer that contains all 16 checked bits.
        if (_mm_movemask_epi8(*longword_ptr)) {
            return 0;
        }
        longword_ptr += 1;
        n -= sizeof(longword);
    }
    char_ptr = (char *)longword_ptr;
    while (n != 0) {
        if (*char_ptr & ASCII_MASK_1BYTE) {
            return 0;
        }
        char_ptr += 1;
        n -= 1;
    }
    return 1;
}
