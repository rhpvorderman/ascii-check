# ascii-check
Header-only C library for ASCII string checking using various optimizations. 

Features:

* Header-only library, simply include the files or code in your project.
* Permissive MIT license. No barriers to including the files.
* Fast ASCII checking using 8-byte wide masking
* Accelerated ASCII checking using SSE2 instructions which are available on 
  every 64-bit x86 CPU.
* Accelerated ASCII checking using AVX2 instructions available only on modern
  64-bit x86 CPUs.

This library was made for fast ASCII checks for ASCII-only file formats. It 
also contains a non-ASCII search for utf-8 decoding purposes. 
It was put here in the hope that others may find it useful or might learn from 
the implementation.

## Usage

Simply copy [`ascii_check.h`](ascii_check.h) and/or [`ascii_check_sse2.h`](
ascii_check_sse2.h) into your project and include them, or use a git submodule
to the same effect.

Both files contain two functions:

```C
/**
 * @brief Check if a string of given length only contains ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for 
 *               terminating NULL bytes.
 * @returns 1 if the string is ASCII-only, 0 otherwise.
 */
static int
string_is_ascii(const char * string, size_t length)


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
search_non_ascii(const char * string, size_t length)
```

For small strings `ascii_check.h` is recommended. For large chunks of memory
`ascii_check_sse2.h` might provide a noticeable improvement at the cost of 
only supporting 64-bit x86-64 CPUs. 
`ascii_check_avx2` is the fastest, though the gains from SSE2 to AVX2 are less 
big than from no vectorization to SSE2 and it adds a major constraint on which 
hardware it can run on.

Please note that the vectorization works best on large chunks of memory. On 
smaller memory sizes there is typically a lot of overhead to using the 
vectorized functions as the pointer has to be aligned first to 16-byte words
for SSE2 and to 32-byte words for AVX2. All the characters leading up to a 
correct alignment are checked in single-byte mode, which is slow. Hence
the 8-byte word non-architecture-specific implementation has the least overhead 
on small strings.

## Optimizations

Optimizations were inspired by the [GNU libc](https://www.gnu.org/software/libc/)
`memchr` implementation and [CPython](https://github.com/python/cpython). 

`string_is_ascii` uses a bitwise OR on 8-byte words to OR all the characters
together. The resulting 8-byte word is then checked for significant 8th bits.
This is the fastest check if the result is expected to be ASCII. 

`search_non_ascii` uses an ASCII mask that is 8 bytes (64-bits) wide for much
faster checking.

SSE2 introduced test instructions that allowed checking bitmasks more quickly.
`ascii_check_sse2.h` uses the `_mm_movemask_epi8` instruction to check the most
signicificant bit of 16 characters at the same time for `search_non_ascii`. 
For `string_is_ascii` the `_mm_or_si128` bitwise OR instruction is used.

`ascii_check_avx2.h` uses similar optimizations as the SSE2 implementation but
using 32-byte words rather than 16-byte words.

