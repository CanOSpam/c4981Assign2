#include "commonCore.h"

void* checkInput();
void* checkForKill();

int qid;
int pid;
struct my_message msgSend;

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

	// Copy argument to variable
	char FileName[strlen(argv[1])];
	strcpy(FileName, argv[1]);
	char* priority = argv[2];

	// Get pid to use as indentifier
	pid = getpid();

	// https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
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

	// Receive file data back
	do
	{
		read_message(qid, pid, &recvBuf);
		printf("%s", recvBuf.mtext);
	}
	while(recvBuf.mtext[0] != 4);

	fflush(stdout);

	return 0;
}

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

void* checkForKill()
{
	struct my_message recvKillBuf;
	read_message(qid, DIEMSGID, &recvKillBuf);
	printf("%s\n", "Terminated by q on a client");
	exit(0);
	return 0;
}