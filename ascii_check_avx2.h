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

// This file is maintained and tested at
// https://github.com/rhpvorderman/ascii-check
// Please report bugs and feature requests there.

#include <stddef.h>
#include <stdint.h>
#include <immintrin.h>

#define ASCII_MASK_1BYTE 0x80

/**
 * @brief Check if a string of given length only contains ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for 
 *               terminating NULL bytes.
 * @returns 1 if the string is ASCII-only, 0 otherwise.
 */
static int
string_is_ascii(const char * string, size_t length) {
    size_t n = length;
    const char * char_ptr = string;
    typedef __m256i longword;
    char all_chars = 0;
    longword all_words = _mm256_set_epi32(0, 0, 0, 0, 0, 0, 0, 0);

    // First align the memory adress
    while ((size_t)char_ptr % sizeof(longword) && n != 0) {
        all_chars |= *char_ptr;
        char_ptr += 1;
        n -= 1;
    }
    const longword * longword_ptr = (longword *)char_ptr;
    while (n >= sizeof(longword)) {
        all_words = _mm256_or_si256(all_words, *longword_ptr);
        longword_ptr += 1;
        n -= sizeof(longword);
    }
    char_ptr = (char *)longword_ptr;
    while (n != 0) {
        all_chars |= *char_ptr;
        char_ptr += 1;
        n -= 1;
    }
    // Check the most significant bits in the accumulated words and chars.
    return !(_mm256_movemask_epi8(all_words) || (all_chars & ASCII_MASK_1BYTE));
}


/**
 * @brief Return the char * to the first non-ASCII character. Like memchr but
 * finds all non-ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for
 *               terminating NULL bytes.
 * @returns The char pointer to the first character that is not ASCII or NULL
 * if such a character was not found.
 */
static const char *
search_non_ascii(const char * string, size_t length) {
    size_t n = length;
    const char * char_ptr = string;
    typedef __m256i longword;

    // The first loop aligns the memory address.
    while ((size_t)char_ptr % sizeof(longword) && n != 0) {
        if (*char_ptr & ASCII_MASK_1BYTE) {
            return char_ptr;
        }
        char_ptr += 1;
        n -= 1;
    }
    const longword * longword_ptr = (longword *)char_ptr;
    while (n >= sizeof(longword)) {
        // _mm256_movemask_epi8 checks the most significant 8th bit of each char
        // and returns a 32-bit integer that contains all 32 checked bits.
        if (_mm256_movemask_epi8(*longword_ptr)) {
            // Character is in longword, find it below.
            break;
        }
        longword_ptr += 1;
        n -= sizeof(longword);
    }
    char_ptr = (char *)longword_ptr;
    while (n != 0) {
        if (*char_ptr & ASCII_MASK_1BYTE) {
            return char_ptr;
        }
        char_ptr += 1;
        n -= 1;
    }
    return NULL;
}
