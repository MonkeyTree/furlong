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
#include "hashtable.h"

typedef struct session_s
{
	int fd

} session_t;

void session_init();
void session_new(int fd);
void session_close(int fd);
void session_data(int fd, char* buffer, int count);