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

#define ASCII_MASK_8BYTE 0x8080808080808080ULL
#define ASCII_MASK_1BYTE 0x80


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
    // The first loop aligns the memory address. Char_ptr is cast to a size_t
    // to return the memory address. Uint64_t is 8 bytes long, and the processor
    // handles this better when its address is a multiplier of 8. This loops
    // handles the first few bytes that are not on such a multiplier boundary.
    while ((size_t)char_ptr % sizeof(uint64_t) && n != 0) {
        if (*char_ptr & ASCII_MASK_1BYTE) {
            return char_ptr;
        }
        char_ptr += 1;
        n -= 1;
    }
    const uint64_t *longword_ptr = (uint64_t *)char_ptr;
    while (n >= sizeof(uint64_t)) {
        if (*longword_ptr & ASCII_MASK_8BYTE){
            // The character is in this longword. Look for the individual
            // character below.
            break;
        }
        longword_ptr += 1;
        n -= sizeof(uint64_t);
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


/**
 * @brief Check if a string of given length only contains ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for 
 *               terminating NULL bytes.
 * @returns 1 if the string is ASCII-only 0 otherwise
 */
static int
string_is_ascii(const char * string, size_t length) {
    size_t n = length;
    // By performing bitwise OR on all characters in 8-byte chunks we can
    // determine ASCII status in a non-branching (except the loops) fashion.
    uint64_t all_chars = 0;
    const char * char_ptr = string;

    // The first loop aligns the memory.
    while ((size_t)char_ptr % sizeof(uint64_t) && n != 0) {
        all_chars |= *char_ptr;
        char_ptr += 1;
        n -= 1;
    }
    const uint64_t *longword_ptr = (uint64_t *)char_ptr;
    while (n >= sizeof(uint64_t)) {
        all_chars |= *longword_ptr;
        longword_ptr += 1;
        n -= sizeof(uint64_t);
    }
    char_ptr = (char *)longword_ptr;
    while (n != 0) {
        all_chars |= *char_ptr;
        char_ptr += 1;
        n -= 1;
    }
    return !(all_chars & ASCII_MASK_8BYTE);
}
