#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <getopt.h>
#include <sys/queue.h>
#include <regex.h>
#include <syslog.h>
#include <time.h>
#ifdef SUPPORT_RUN_AS
#include <pwd.h>
#endif
#include <arpa/inet.h>
#include <signal.h>
#include <linux/limits.h>

typedef struct connection_s
{
	int (*on_new_connection)(int fd, int socket, char* hbuf, char* sbuf);
	int (*on_close_connection)(int fd, int socket);
	int (*on_data)(int fd, int socket, char *buf, int count);
	int (*on_error)(int fd, int socket, char *buf);
} connection_t;


int on_new_connection(int fd, int socket, char* hbuf, char* sbuf);
int on_close_connection(int fd, int socket);
int on_data(int fd, int socket, char *buf, int count);
int on_error(int fd, int socket, char *buf);

int init_connection(struct connection_s *conn);
