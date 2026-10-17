#ifndef MINI_REDIS_H
#define MINI_REDIS_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_drawer
{
	char			*value;
	char			*key;
	struct s_drawer	*next;
} t_drawer;

typedef struct s_table
{
	t_drawer	**buckets;
	size_t		capacity;
	size_t		size;
} t_table;

unsigned long	hash(const char *s);
t_table			*table_new(size_t capacity);
void			table_free(t_table *tab);
char			*table_get(t_table *tab, char *key);
int			table_set(t_table *tab, char *key, char *value);
int				table_del(t_table *tab, char *key);
int				table_resize(t_table *tab);

#endif