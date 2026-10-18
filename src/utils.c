
#include "mini_redis.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = strlen(s1);
	len_s2 = strlen(s2);
	str = malloc(len_s1 + len_s2 + 1);
	if (!str)
		return (NULL);
	memcpy(str, s1, len_s1);
	memcpy(str + len_s1, s2, len_s2 + 1);
	return (str);
}

unsigned long hash(const char *s)
{
    unsigned long h = 5381;
    while (*s)
        h = h * 33 + *s++;
    return h;
}
