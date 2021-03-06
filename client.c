/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.c - 
--
-- PROGRAM: Message Queue Assignment
--
-- FUNCTIONS:
-- void* checkInput()
-- void* checkForKill()
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
-- This program is the client process. It connects to the server and requests a file via message passing. If it receives
-- a positive response, it starts to read messages containing the file data. This program starts two threads, one thread
-- monitors for user input (the letter q), and if it is received, begins a quit process by sending a quit message to the
-- server. The other thread monitors for kill messages from the server and terminates the process if one is received.
----------------------------------------------------------------------------------------------------------------------*/
#include "commonCore.h"


void* checkInput();
void* checkForKill();

int qid;
int pid;
struct my_message msgSend;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Tim Bruecker
--
-- PROGRAMMER: Tim Bruecker
--
-- INTERFACE: main (int argc , char *argv[])
--
-- RETURNS: 0 to quit.
--
-- NOTES:
-- This is the entry point for the program described in the program header.
----------------------------------------------------------------------------------------------------------------------*/
int main (int argc , char *argv[])
{
	if (argc != 3)
	{
		errno = EINVAL;
		perror("Correct client invocation is ./client \"FileName\" \"priority\" \nError");
		return 0;
	}

	pthread_t qThread;
	pthread_t dieThread;

	/* Copy argument to variable */
	char FileName[strlen(argv[1])];
	strcpy(FileName, argv[1]);
	char* priority = argv[2];

	/* Get pid to use as indentifier */
	pid = getpid();

	/* 
	Line below sourced from:
	https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
	*/
	char strPid[(int)((ceil(log10(pid))+1)*sizeof(char))];
	sprintf(strPid, "%d", pid);
	

	
	struct my_message recvBuf;
	qid = open_queue(MQKEY);

	pthread_create(&qThread, NULL, checkInput, NULL);
	pthread_create(&dieThread, NULL, checkForKill, NULL);


	memset(msgSend.mtext, '\0', MTEXTLEN);

	msgSend.mtype = CONNECTMSGID;
	strcat(msgSend.mtext, FileName);
	strcat(msgSend.mtext, " ");
	strcat(msgSend.mtext, strPid);
	strcat(msgSend.mtext, " ");
	strcat(msgSend.mtext, priority);

	send_message(qid, &msgSend);
	printf("%s\n", msgSend.mtext);

	/* Receive file data back */
	do
	{
		read_message(qid, pid, &recvBuf);
		printf("%s", recvBuf.mtext);
	}
	while(recvBuf.mtext[0] != 4);

	fflush(stdout);

	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: checkInput
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Tim Bruecker
--
-- PROGRAMMER: Tim Bruecker
--
-- INTERFACE: void* checkInput( void )
--
-- RETURNS: void.
--
-- NOTES:
-- This function blocks on getchar and waits for the user to enter a q. If it detects a q was entered, it send a kill 
-- message to the server, initiating complete shutdown. This is run by a thread concurrently to the clients main funtion.
----------------------------------------------------------------------------------------------------------------------*/
void* checkInput()
{
	while(1)
	{
		char temp = getchar();
		if(temp == 'q')
		{
			msgSend.mtext[0] = '\x03';
			msgSend.mtype = CONNECTMSGID;
			send_message(qid, &msgSend);
		}
	}
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: checkForKill
--
-- DATE: March 7, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Tim Bruecker
--
-- PROGRAMMER: Tim Bruecker
--
-- INTERFACE: void* checkForKill( void )
--
-- RETURNS: void.
--
-- NOTES:
-- This function waits for a die message to be received from the server. If the message is received it kills the process.
-- This is run by a thread concurrently to the clients main funtion.
----------------------------------------------------------------------------------------------------------------------*/
void* checkForKill()
{
	struct my_message recvKillBuf;
	read_message(qid, DIEMSGID, &recvKillBuf);
	printf("%s\n", "Terminated by q on a client");
	exit(0);
	return 0;
}