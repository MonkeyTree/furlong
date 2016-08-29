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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <regex.h>
#include <errno.h>
#include <syslog.h>
#include <time.h>
#ifdef SUPPORT_RUN_AS
#include <pwd.h>
#endif
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <linux/limits.h>

#define MAXEVENTS 64

int start_net_tcp (int port);