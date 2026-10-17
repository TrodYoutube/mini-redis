#include "mini_redis.h"

#define N 100000

static size_t	max_chain(t_table *tab)
{
	size_t		i;
	size_t		len;
	size_t		max;
	t_drawer	*cur;

	i = 0;
	max = 0;
	while (i < tab->capacity)
	{
		len = 0;
		cur = tab->buckets[i];
		while (cur)
		{
			len++;
			cur = cur->next;
		}
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

int	main(void)
{
	t_table	*tab;
	char	key[32];
	char	val[32];
	char	*res;
	size_t	errors;
	int		i;

	tab = table_new(4);
	if (!tab)
		return (1);

	i = 0;
	while (i < N)
	{
		snprintf(key, sizeof(key), "key%d", i);
		snprintf(val, sizeof(val), "val%d", i);
		table_set(tab, key, val);
		i++;
	}
	printf("apres insertions : size %zu | capacity %zu | plus longue chaine %zu\n",
		tab->size, tab->capacity, max_chain(tab));

	errors = 0;
	i = 0;
	while (i < N)
	{
		snprintf(key, sizeof(key), "key%d", i);
		snprintf(val, sizeof(val), "val%d", i);
		res = table_get(tab, key);
		if (!res || strcmp(res, val))
			errors++;
		i++;
	}
	printf("erreurs de relecture : %zu\n", errors);

	table_free(tab);
	return (0);
}