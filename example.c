/*--------- showmsg.c: Show message queue details ----*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


struct my_message
  {
    long mtype;
    char mtext[256];
  }message;


/*--- Function prototypes -----------------*/
void mqstat_print (key_t mkey, int mqid, struct msqid_ds *mstat);
int send_message( int msg_qid, struct my_message *qbuf );
int read_message (int qid, long type, struct my_message *qbuf );

int main (int argc , char *argv[])
{
  key_t mkey;
  pid_t pid;
  int msq_id;
  struct msqid_ds   msq_status;
  

  if (argc != 2)
  {
    fprintf (stderr, "Usage: showmsg keyval\n");
    exit(1);
  }

    /*---- Get message queue identifier ------*/
    mkey = (key_t) atoi(argv[1]);
    if ((msq_id = msgget (mkey, IPC_CREAT)) < 0)
    {
      perror ("msgget failed!");
      exit(2);
    }

    /*--- get status info -----------------*/
    if (msgctl (msq_id, IPC_STAT, &msq_status) < 0)
    {
      perror ("msgctl (get status)failed!");
      exit(3);
    }

    /*-- print out status information -------*/
    mqstat_print (mkey, msq_id, &msq_status);
        
   

    pid = fork();

    if(pid == 0) {
      strcpy(message.mtext, "test");
      message.mtype = 1;
      send_message(msq_id, &message);
    } 
    else
    {
      sleep(1);
      read_message(msq_id, 1, &message);
      printf("%s\n", message.mtext);

      // Remove  the message queue
      if (msgctl (msq_id, IPC_RMID, 0) < 0)
      {
        perror ("msgctl (remove queue) failed!");
        exit (3);
      }
    }

    exit(0);
}

/*--------- status info print function ---------*/
void mqstat_print (key_t mkey, int mqid, struct msqid_ds *mstat)
{
  /*-- call the library function ctime ----*/
  char *ctime();

  printf ("\nKey %d, msg_qid %d\n\n", mkey, mqid);
  printf ("%d messages on queue\n\n", (int)mstat->msg_qnum);
  printf ("Last send by proc %d at %s\n",
           mstat->msg_lspid, ctime(&(mstat->msg_stime)));
  printf ("Last recv by proc %d at %s\n",
           mstat->msg_lrpid, ctime(&(mstat->msg_rtime)));
}


int send_message( int msg_qid, struct my_message *qbuf )
{
 int result, length;
 /* The length is essentially the size of the structure minus sizeof(mtype) */
 length = sizeof (struct my_message) - sizeof(long);
 if( (result = msgsnd ( msg_qid, qbuf, length, 0) ) == -1)
 {
 return (-1);
 }

 return (result);
} 

int read_message (int qid, long type, struct my_message *qbuf )
{
 int result, length;
 // The length is essentially the size of the structure minus sizeof(mtype)
 length = sizeof(struct my_message) - sizeof(long);
 if ( (result = msgrcv ( qid, qbuf, length, type, 0) ) == -1)
 {
 return(-1);
 }

 return(result);
} 

