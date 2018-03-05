#include "commonCore.h"

int send_message( int qid, struct my_message *qbuf )
{
	 int result, length;

	 /* The length is essentially the size of the structure minus sizeof(mtype) */
	 length = sizeof (struct my_message) - sizeof(long);

	 if( (result = msgsnd ( qid, qbuf, length, 0) ) == -1)
	 {
	 	return (-1);	
	 }

	 return (result);
} 

int read_message (int qid, long type, struct my_message *qbuf )
{
	 int result, length;

	 /* The length is essentially the size of the structure minus sizeof(mtype) */
	 length = sizeof(struct my_message) - sizeof(long);

	 if ( (result = msgrcv ( qid, qbuf, length, type, 0) ) == -1)
	 {
	 	return(-1);
	 }

	 return(result);
} 

int read_message_noblock (int qid, long type, struct my_message *qbuf )
{
	 int result, length;

	 /* The length is essentially the size of the structure minus sizeof(mtype) */
	 length = sizeof(struct my_message) - sizeof(long);

	 if ( (result = msgrcv ( qid, qbuf, length, type, IPC_NOWAIT) ) == -1)
	 {
	 	return(-1);
	 }

	 return(result);
} 

int open_queue (key_t keyval)
{
	 int qid;

	 if( (qid = msgget (keyval, IPC_CREAT | 0660 ) ) == -1 )
	 {
	 	return (-1);
	 }

	 return (qid);
}