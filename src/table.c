
#include "mini_redis.h"

t_table	*table_new(size_t capacity)
{
	t_table	*tab;

	if (capacity == 0)
		return (NULL);
	tab = malloc(sizeof(t_table));
	if (!tab)
		return (NULL);
	tab->buckets = calloc(capacity, sizeof(t_drawer *));
	if (!tab->buckets)
	{
		free(tab);
		return (NULL);
	}
	tab->capacity = capacity;
	tab->size = 0;
	return (tab);
}

int	table_set(t_table *tab, char *key, char *value)
{
	t_drawer	*cur;
	char		*tmp;
	size_t		index;

	index = hash((const char *) key) % tab->capacity;
	cur = tab->buckets[index];
	while (cur && strcmp(cur->key, key))
		cur = cur->next;
	if (cur)
	{
		tmp = cur->value;
		cur->value = strdup(value);
		if (!cur->value)
		{
			cur->value = tmp;
			return (-1);
		}
		free(tmp);
	}
	else
	{
		if ((tab->size + 1) * 4 > tab->capacity * 3)
		{
			if (table_resize(tab) == -1)
				return (-1);
			index = hash((const char *) key) % tab->capacity;
		}
		cur = malloc(sizeof(t_drawer));
		if (!cur)
			return (-1);
		cur->value = strdup(value);
		cur->key = strdup(key);
		if (!cur->value || !cur->key)
		{
			free(cur->value);
			free(cur->key);
			free(cur);
			return (-1);
		}

		cur->next = tab->buckets[index];
		tab->size++;
		tab->buckets[index] = cur;
	}
	return (1);
}

char	*table_get(t_table *tab, char *key)
{
	size_t		index;
	t_drawer	*cur;

	index = hash((const char *) key) % tab->capacity;
	cur = tab->buckets[index];
	while (cur)
	{
		if (!strcmp(cur->key, key))
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

void	table_free(t_table *tab)
{
	t_drawer	*cur;
	t_drawer	*tmp;
	size_t		i;

	i = 0;
	while (i < tab->capacity)
	{
		cur = tab->buckets[i];
		while(cur)
		{
			tmp = cur->next;
			free(cur->value);
			free(cur->key);
			free(cur);
			cur = tmp;
		}
		i++;
	}
	free(tab->buckets);
	free(tab);
}

int	table_del(t_table *tab, char *key)
{
	size_t		index;
	t_drawer	*cur;
	t_drawer	*tmp;

	index = hash((const char *) key) % tab->capacity;
	cur = tab->buckets[index];
	tmp = NULL;
	while (cur && strcmp(cur->key, key))
	{
		tmp = cur;
		cur = cur->next;
	}
	if (cur)
	{
		if (tmp)
			tmp->next = cur->next;
		else
		{
			tmp = cur->next;
			tab->buckets[index] = tmp;
		}
		free(cur->value);
		free(cur->key);
		free(cur);
		tab->size--;
		return (1);
	}
	return (-1);
}

int	table_resize(t_table *tab)
{
	t_drawer	**new_buckets;
	t_drawer	*cur;
	t_drawer	*next;
	size_t		i;
	size_t		cap;
	size_t		index;

	if (tab->capacity == 0)
		return (-1);
	cap = tab->capacity;
	new_buckets = calloc(tab->capacity * 2, sizeof(t_drawer *));
	if (!new_buckets)
		return (-1);
	tab->capacity *= 2;
	i = 0;

	while (i < cap)
	{
		cur = tab->buckets[i];
		while (cur)
		{
			index = hash(cur->key) % tab->capacity;
			next = cur->next;
			cur->next = new_buckets[index];
			new_buckets[index] = cur;
			cur = next;
		}
		i++;
	}
	free(tab->buckets);
	tab->buckets = new_buckets;
	return (1);
}