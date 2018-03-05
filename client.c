#include "commonCore.h"

int main (int argc , char *argv[])
{
	if (argc != 3)
	{
		errno = EINVAL;
		perror("Correct client invocation is ./client \"FileName\" \"priority\" \nError");
		return 0;
	}

	// Copy argument to variable
	char FileName[strlen(argv[1])];
	strcpy(FileName, argv[1]);
	char* priority = argv[2];

	// Get pid to use as indentifier
	int pid = getpid();

	// https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
	char strPid[(int)((ceil(log10(pid))+1)*sizeof(char))];
	sprintf(strPid, "%d", pid);
	

	struct my_message msgSend;
	struct my_message recvBuf;
	int qid = open_queue(6969);


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
		printf("%s\n", recvBuf.mtext);
	}
	while(recvBuf.mtext[0] != 4);

	

 	

	return 0;
}