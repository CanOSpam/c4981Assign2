#ifndef COMMONCORE_H_
#define COMMONCORE_H_

/*--------- includes for message passing ----*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <signal.h>

#define CONNECTMSGID 5
#define BODYMSGID 6
#define MSGDELIMITER 22
#define MTEXTLEN 256
#define MAXCLIENTS 30

struct my_message
 {
    long mtype;
    char mtext[MTEXTLEN];
 };

struct connected_client
{
	FILE *file;
	int pid;
	int priority;
};


int send_message( int qid, struct my_message *qbuf );

int read_message (int qid, long type, struct my_message *qbuf );

int read_message_noblock (int qid, long type, struct my_message *qbuf );

int open_queue (key_t keyval);

#endif