#include "pid.h"

/*
 * Static variables used by the getopts_long function.
 */
static int help_flag = 0;
static int version_flag = 0;

static int had_hup = 0;

/*
 * Create the pid file used to keep track of this daemon's current
 * process id.
 */
pid_t create_pidfile(void)
{
    int fd;
    pid_t pid;
    char buf[20];

    if ((fd = open(PID_FILE, O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR)) == -1) {
        syslog(LOG_ERR, "Unable to open or create pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        printf("Unable to open or create pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        syslog(LOG_ERR, "Unable to lock pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        printf("Unable to lock pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* delete contents of file */
    if (ftruncate(fd, 0) == -1) {
        syslog(LOG_ERR, "Unable to truncate pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        printf("Unable to truncate pid file %s: (%d)%s", PID_FILE, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pid = getpid();
    snprintf(buf, sizeof(buf), "%ld\n", (long)pid);
    if (write(fd, buf, strlen(buf)) != strlen(buf)) {
        syslog(LOG_ERR, "Unable to write pid to %s: (%d)%s", PID_FILE, errno, strerror(errno));
        printf("Unable to write pid to %s: (%d)%s", PID_FILE, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return pid;
}

/*
 * Destroy the pid file used to keep track of this daemon's current
 * process id.
 */
int destroy_pidfile(void)
{
    return unlink(PID_FILE);
}

/*
 * Handle the HUP signal by indicating it has occurred.
 */
void sighup_handler(int signum)
{
    struct sigaction act;
    memset(&act, 0, sizeof (act));

    had_hup = 1;
    syslog(LOG_NOTICE, "HUP received...");

    sigemptyset(&act.sa_mask);
    act.sa_handler = sighup_handler;
    sigaction(SIGHUP, &act, NULL);
}

/*
 * Handle the TERM signal by exiting.
 */
void sigterm_handler(int signum)
{
    syslog(LOG_NOTICE, "Exiting...");
    closelog();
    destroy_pidfile();
    exit(EXIT_SUCCESS);
}

/*
 * This handler is only present while the parent is running and is intended
 * to handle signals generated by the child.
 */
void child_handler(int signum)
{
    switch (signum) {
    case SIGALRM: exit(EXIT_FAILURE); break;
    case SIGUSR1: exit(EXIT_SUCCESS); break;
    case SIGCHLD: exit(EXIT_FAILURE); break;
    }
}

/*
 * Convert the executable to run as a daemon.
 */
void daemonize(void)
{
    pid_t pid, sid, parent;
    int lfp = -1;

    /* Already a daemon? */
    if (getppid() == 1) return;

#ifdef SUPPORT_RUN_AS
    if (getuid() == 0 || geteuid() == 0) {
        struct passwd *pw = getpwnam(RUN_AS_USER);
        if (pw) {
            syslog(LOG_NOTICE, "Setting user to " RUN_AS_USER);
            setuid(pw->pw_uid);
        }
    }
#endif

    /* Trap signals we expect to receive */
    signal(SIGCHLD, child_handler);
    signal(SIGUSR1, child_handler);
    signal(SIGALRM, child_handler);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "Unable to fork daemon, error (%d)%s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, we can exit the parent process */
    if (pid > 0) {
        /* Wait for confirmation from the child via SIGTERM of SIGCHLD, or
         * for two seconds to elapse (SIGALRM). pause() should not return.
         */
        alarm(2);
        pause();
        exit(EXIT_FAILURE);
    }

    /* At this point we are executing as the child process */
    parent = getppid();

    /* Catch, Ignore and handle signals */
    signal(SIGCHLD, SIG_IGN); /* Ignore child */
    signal(SIGTSTP, SIG_IGN); /* Ignore tty signals */
    signal(SIGTTOU, SIG_IGN); /* Ignore terminal output signal */
    signal(SIGTTIN, SIG_IGN); /* Ignore terminal input signal */
    signal(SIGHUP,  sighup_handler);
    signal(SIGTERM, sigterm_handler);

    /* Change the file mode mask */
    umask(027);

    /* Create a new SID for the child process */
    if (setsid() < 0) {
        syslog(LOG_ERR, "Unable to create a new session, error (%d)%s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Change running directory */
    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "Unable to change directory to /, error (%d)%s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    freopen("/dev/null", "r", stdin);
    //freopen("/dev/null", "w", stdout);
    //freopen("/dev/null", "w", stderr);

    /* Tell the parent process that we are okay */
    kill(parent, SIGUSR1);
}
