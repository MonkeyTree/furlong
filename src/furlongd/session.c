#include "session.h"

#define BUF_SIZ 128
#define HT_SIZ 2048

static hashtable_t *hashtable = NULL;
static char buffer [BUF_SIZ];

void session_init()
{
	hashtable = ht_create( HT_SIZ );
}

void session_new(int fd)
{
	session_t *session = (session_t *)malloc(sizeof(session_t));
	session->fd = fd;
	snprintf (buffer, BUF_SIZ, "%d",fd); 
	ht_set( hashtable, buffer, (void*)session );	
}

void session_close(int fd)
{
	snprintf (buffer, BUF_SIZ, "%d",fd); 
	session_t *session = ht_get( hashtable, buffer); 
	free(session);
}

void session_data(int fd, char* buffer, int count)
{
	snprintf (buffer, BUF_SIZ, "%d",fd); 
	session_t *session = ht_get( hashtable, buffer); 
}