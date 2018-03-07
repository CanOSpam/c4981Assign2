/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: commonCore.c - 
--
-- PROGRAM: Message Queue Assignment
--
-- FUNCTIONS:
-- int send_message( int qid, struct my_message *qbuf )
-- int read_message (int qid, long type, struct my_message *qbuf )
-- int read_message_noblock (int qid, long type, struct my_message *qbuf )
-- int open_queue (key_t keyval)
--
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Tim Bruecker
--
-- PROGRAMMER: Tim Bruecker
--
-- NOTES:
-- This library contains the functions common to both the client and the server, and is included in both. It
-- contains wrapper functions for opening a queue, sending messages, reading messages, and reading messages without blocking.
----------------------------------------------------------------------------------------------------------------------*/
#include "commonCore.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: send_message
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- INTERFACE: int send_message(int qid, struct my_message *qbuf)
-- 					int qid: ID of the queue to write to.
--					struct my_message *qbuf: Pointer to the struct to read from.
--
-- RETURNS: -1 on error, otherwise returns 0.
--
-- NOTES:
-- This function writes the specified struct the message queue. This wrapper function is sourced from milliways.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_message
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- INTERFACE: int read_message(int qid, long type, struct my_message *qbuf)
-- 					int qid: ID of the queue to read from.
-- 					long type: The type of message to read.
--					struct my_message *qbuf: Pointer to the struct to write to.
--
-- RETURNS: -1 on error, otherwise returns the number of bytes copied into the mtext array of the qbuf struct.
--
-- NOTES:
-- This function reads a message from the message queue and saves it to qbuf. 
-- This wrapper function is sourced from milliways.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: open_queue
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- INTERFACE: int open_queue(key_t keyval)
-- 					key_t keyval: A key to use to generate a queue id.
--
-- RETURNS: -1 on error, otherwise returns a queue ID generated from the key.
--
-- NOTES:
-- This function opens a message queue using the provided key value. This wrapper function is sourced from milliways.
----------------------------------------------------------------------------------------------------------------------*/
int open_queue (key_t keyval)
{
	 int qid;

	 if( (qid = msgget (keyval, IPC_CREAT | 0660 ) ) == -1 )
	 {
	 	return (-1);
	 }

	 return (qid);
}