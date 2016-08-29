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

#include "pid.h"
#include "net_tcp.h"



/*
 * Define default listen port for incoming log entries
 */
#define DEFAULT_SERVER_PORT         "4099"

/*
 * Store the program name for use in logging messages.
 */
const char *prog_name;

#define xstr(s) str(s)
#define str(s) #s

/*
 * Static variables used by the getopts_long function.
 */
static int help_flag = 0;
static int version_flag = 0;






/*
 * Print usage information to the console.
 */
static void print_usage(void)
{
    printf("Usage: %s [OPTION]...\n"
           "\n"
           "\n"
           "\n"
           "\n"
           "\n"
           "The following OPTIONS are supported:\n"
           "  -p, --port PORT    list on PORT for incoming log messages\n"
           "      --help         display this help and exit\n"
           "      --version      output version information and exit\n"
           "\n", prog_name);
}

/*
 * Print program version information to the console.
 */
static void print_version(void)
{
    printf("%s %s\n", prog_name, xstr(VERSION));
}

/*
 * Main function. Process the command line arguments.
 */
int main(int argc, char *argv[])
{
    int c;
    int port_optind = -1;

    if ((prog_name = strrchr(argv[0], '/')) != NULL)
        ++prog_name;
    else
        prog_name = argv[0];

    while (1) {
        static struct option long_options[] =
        {
            {"port",    required_argument, 0, 'p'},
            {"help",    no_argument, &help_flag, 1},
            {"version", no_argument, &version_flag, 1},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "p:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            break;

        case 'p':
            port_optind = optind - 1;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            exit(EXIT_FAILURE);

        default:
            abort();
        }
    }

    if (help_flag == 1) {
        print_usage();
        exit(EXIT_SUCCESS);
    }

    if (version_flag == 1) {
        print_version();
        exit(EXIT_SUCCESS);
    }


    openlog(prog_name, LOG_PID, LOG_LOCAL5);

    daemonize();
    create_pidfile();

    start_net_tcp(port_optind != -1 ? argv[port_optind] : DEFAULT_SERVER_PORT);

    destroy_pidfile();
    closelog();
    return 0;
}


