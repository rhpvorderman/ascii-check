# ascii-check
Header-only C library for ASCII string checking using various optimizations. 

## Usage

Simply copy [`ascii_check.h`](ascii_check.h) and/or [`ascii_check_sse41.h`](
ascii_check_sse41.h) into your project and include them, or use a git submodule
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
`ascii_check_sse41.h` might provide a noticeable improvement at the cost of 
only supporting x86 CPUs that are not old (Intel from 2007 onwards, AMD from 
2011 onwards.) 

## Optimizations

Optimizations were inspired by the [GNU libc](https://www.gnu.org/software/libc/)
`memchr` implementation and [CPython](https://github.com/python/cpython). 

`ascii_check.h` uses an ASCII mask that is 8 bytes (64-bits) wide for much
faster checking.

SSE4.1 introduced test instructions that allowed checking bitmasks more quickly.
`ascii_check_sse41` uses the `_mm_testz_si128` instruction to check 
with a 16-bytes (128-bits) ASCII mask.
