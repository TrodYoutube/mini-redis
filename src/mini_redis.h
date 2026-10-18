#ifndef MINI_REDIS_H
#define MINI_REDIS_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>


// TABLE : 

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

t_table			*table_new(size_t capacity);
void			table_free(t_table *tab);
char			*table_get(t_table *tab, char *key);
int				table_set(t_table *tab, char *key, char *value);
int				table_del(t_table *tab, char *key);
int				table_resize(t_table *tab);
unsigned long	hash(const char *s);




// SERVER :

int				accept_client(int server_fd);
int				echo_loop(int client_fd);
int     		create_server_socket(int port);
int				set_nonblocking(int fd);
int				epoll_add(int epfd, int fd);
int				handle_client(int epfd, int client_fd);
int				close_client(int epfd, int fd);
int				accept_new_client(int epfd, int server_fd);



#endif