#include "mini_redis.h"

int	main(void)
{
	int	server_fd;
	int	fd;
	int	i;
	int	x;
	int	epfd;
	struct epoll_event events[64];
	
	signal(SIGPIPE, SIG_IGN);
	server_fd = create_server_socket(6380);
	if (server_fd == -1)
		return (1);
	if (set_nonblocking(server_fd) == -1)
	{
		perror("Server cannot be initialize");
		close(server_fd);
		return (1);
	}
	epfd = epoll_create1(0);
	if (epfd == -1)
	{
		perror("epfd can't be initialize");
		close(server_fd);
		return (1);
	}
	if (epoll_add(epfd, server_fd) == -1)
	{
		close(server_fd);
		close(epfd);
		return (1);
	}
	while (1)
	{
		x = epoll_wait(epfd, events, 64, -1);
		if (x == -1)
		{
			if (errno == EINTR)
				continue ;
			else
			{
				perror ("Waiting Problem");
				break ;
			}
		}
		while (i < x)
		{
			fd = events[i].data.fd;
			if (fd == server_fd)
				accept_new_client(epfd, server_fd);
			else
				handle_client(epfd, fd);
			i++;
		}
		i = 0;	
	}
	close(server_fd);
	close(epfd);
	return (0);
}