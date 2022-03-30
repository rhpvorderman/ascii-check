# ascii-check
Header-only C library for ASCII string checking using various optimizations. 

Features:

* Header-only library, simply include the files or code in your project.
* Permissive MIT license. No barriers to including the files.
* Fast ASCII checking using 8-byte wide masking
* Accelerated ASCII checking using SSE2 instructions which are available on 
  every 64-bit x86 CPU.

This library was made for fast ASCII checks for ASCII-only file formats. It 
was put here in the hope that others may find it useful or might learn from 
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

## Optimizations

Optimizations were inspired by the [GNU libc](https://www.gnu.org/software/libc/)
`memchr` implementation and [CPython](https://github.com/python/cpython). 

`ascii_check.h` uses an ASCII mask that is 8 bytes (64-bits) wide for much
faster checking.

SSE2 introduced test instructions that allowed checking bitmasks more quickly.
`ascii_check_sse2` uses the `_mm_movemask_epi8` instruction to check the most
signicificant bit of 16 characters at the same time.
