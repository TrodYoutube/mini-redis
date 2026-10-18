#include "mini_redis.h"

int     create_server_socket(int port)
{
	int					fd;
	int					opt;
	struct sockaddr_in	addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);  
	if (fd == -1)
	{
		perror("socket");
		return (-1);
	}
	opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		close(fd);
		return (-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		perror("bind");
		close(fd);
		return (-1);
	}
	if (listen(fd, 128) == -1)
	{
		perror("listen");
		close(fd);
		return (-1);
	}
	return (fd);
}

int	accept_client(int server_fd)
{
	int	fd;

	fd = accept(server_fd, NULL, NULL);
	if (fd == -1)
	{
		perror("accept");
		return (-1);
	}
	return (fd);
}

int	set_nonblocking(int fd)
{
	int	x;

	x =	fcntl(fd, F_GETFL);
	if (x == -1)
	{
		perror("Get Flags");
		return (-1);
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK | x) == -1)	
	{
		perror("Set Flags");
		return (-1);
	}
	return (1);
}

int	epoll_add(int epfd, int fd)
{
	struct epoll_event ev;

	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		perror("epoll_ctl");
		return (-1);
	}
	return (1);
}

int	accept_new_client(int epfd, int server_fd)
{
	int	fd;

	fd = accept_client(server_fd);
	if (fd == -1)
		return (-1);
	if (set_nonblocking(fd) == -1)
	{
		close (fd);
		return (-1);
	}
	if (epoll_add(epfd, fd) == -1)
	{
		close(fd);
		return (-1);
	}
	return (1);
}

int	close_client(int epfd, int fd)
{
	if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		perror("Delete from CTL");
		close(fd);
		return (-1);
	}
	close(fd);
	return(1);
}

int	handle_client(int epfd, int client_fd)
{
	char	buff[1024];
	ssize_t	x;
	ssize_t	y;

	x = read(client_fd, buff, 1024);
	if (x > 0)
	{
		y = write(client_fd, buff, x);
		if (y == -1)
		{
			perror("Write Failed");
			close_client(epfd, client_fd);
			return (-1);
		}
		if (y < x)
		{
			fprintf(stderr, "partial write\n");
			close_client(epfd, client_fd);
			return (-1);
		}
	}
	if (x == 0)
	{
		close_client(epfd, client_fd);
		return (1);
	}
	if (x == -1)
	{
		if (errno != EAGAIN && errno != EINTR && errno != EWOULDBLOCK)
		{
			perror("Read Failed");
			close_client(epfd, client_fd);
			return (-1);
		}
	}
	return (1);
}