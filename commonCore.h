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
#include <pthread.h>
#include <signal.h>
#include <sys/sem.h>

#define CONNECTMSGID 98
#define DIEMSGID 99
#define BODYMSGID 6
#define MTEXTLEN 1024
#define MQKEY 6969

struct my_message
 {
    long mtype;
    char mtext[MTEXTLEN];
 };


int send_message( int qid, struct my_message *qbuf );

int read_message (int qid, long type, struct my_message *qbuf );

int read_message_noblock (int qid, long type, struct my_message *qbuf );

int open_queue (key_t keyval);

#endif