
#include "mini_redis.h"

unsigned long hash(const char *s)
{
    unsigned long h = 5381;
    while (*s)
        h = h * 33 + *s++;
    return h;
}
