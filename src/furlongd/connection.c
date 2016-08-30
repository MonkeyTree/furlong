#include "connection.h"

int on_new_connection(int fd, int socket, char* hbuf, char* sbuf)
{
	printf("on_new_connection [%d, %d] [%s, %s] \n", fd, socket, hbuf, sbuf);
	return 0;
}

int on_close_connection(int fd, int socket)
{
	printf("on_close_connection [%d, %d] \n", fd, socket);
	return 0;
}

int on_data(int fd, int socket, char *buf, int count)
{
	//printf("on_data [%d, %d] [%s, %s] \n", fd, socket, buf, count);
	int s = write (1, buf, count);
	if (s == -1)
	  {
	    perror ("write");
	    //abort ();
	  }

	return 0;
}

int on_error(int fd, int socket, char *data)
{
	return 0;
}


int init_connection(struct connection_s *conn)
{
	conn->on_new_connection = &on_new_connection;
	conn->on_close_connection = &on_close_connection;
	conn->on_data = &on_data;
	conn->on_error = &on_error;
	return 0;
}