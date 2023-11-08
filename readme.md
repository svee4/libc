## Experimental C library from scratch for fun. Implemented with Linux x86_64 syscalls. Does not conform to C standard library specification because it sucks.

Features:
 - Wrapper around `_start`, so you can just define `main(int argc, char** argv)`
 - Wrapper for syscalls
 - `malloc()` and `free()`
 - `string` with partial UTF8 support
 - various `print` functions for stdout
 - implementations for `vec`, `list`, and `hashmap`