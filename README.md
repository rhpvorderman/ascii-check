# ascii-check
Header-only C library for ASCII string checking using various optimizations. 

## Usage

Simply copy [`ascii_check.h`](ascii_check.h) and/or [`ascii_check_sse2.h`](
ascii_check_sse2.h) into your project and include them, or use a git submodule
to the same effect.

Both files contain only one function:

```C
/**
 * @brief Check if a string of given length only contains ASCII characters.
 *
 * @param string A char pointer to the start of the string.
 * @param length The length of the string. This funtion does not check for 
 *               terminating NULL bytes.
 * @returns 1 if the string is ASCII-only
 */
static int
string_is_ascii(const char * string, size_t length)
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
