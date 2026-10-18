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

int	echo_loop(int client_fd)
{
	ssize_t		x;
	ssize_t		v;
	char		buff[1024];

	x = 1;
	while (x > 0)
	{
		x = read(client_fd, buff, sizeof(buff));
		if (x > 0)
		{
			v = write(client_fd, buff, x);
			if (0 <= v && v < x)
			{
				perror("partial write failed");
				return (-1);
			}
			if (v < x)
			{
				perror("write failed");
				return (-1);
			}
		}
		else if (x < 0)
		{
			perror("read");
			return (-1);
		}
	}
	return (1);
}