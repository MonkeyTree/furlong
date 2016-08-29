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

#define PID_FILE     "/var/run/furlongd.pid"

void daemonize(void);
pid_t create_pidfile(void);
int destroy_pidfile(void);