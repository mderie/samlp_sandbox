
#ifndef TINY_LOGGER
#define TINY_LOGGER

// Log formatted... Almost similar to printf :) But the "..." format string cannot be empty !-(
#define LOGF(s, ...) logf(stringf(s, __VA_ARGS__).c_str())

// C like simple logger
void logf(const char *s);

#endif // TINY_LOGGER
