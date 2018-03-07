#include "commonCore.h"


void getSem(int sid);
void releaseSem(int sid);
int initsem (key_t key);
void quitEverything(int numClients, int qid, int semid);

extern int errno;

int main (int argc , char *argv[])
{
	struct my_message rcvConnects;
	int qid = open_queue(MQKEY);
	int newConnection;
	int semKey = 100;
	int semid;
	int numClients;

	semid = initsem ((key_t)semKey);
	releaseSem(semid);


	// Polling Loop
	while(1)
	{
		newConnection = read_message(qid, CONNECTMSGID, &rcvConnects);

		if(rcvConnects.mtext[0] == '\x03')
		{
			quitEverything(numClients, qid, semid);
		}
		else if(newConnection != -1)
		{
			FILE *fileHandle;
			char fileName[MTEXTLEN];
			int pid;
			int priority;
			sscanf(rcvConnects.mtext, "%s %d %d", fileName, &pid,  &priority);
			printf("New client wants: %s\nAt pid: %d\nWith priority: %d\n\n", fileName, pid, priority);



			if(fork())
			{
				struct my_message sendBuf;
				int fileSent = 0;
				// Try to Open the file
				fileHandle = fopen(fileName, "r");
				if(fileHandle == NULL)
				{
					// Send error string to client
					strcpy(sendBuf.mtext, "\x04 Error: Invalid filename");
					sendBuf.mtype = pid;
					send_message(qid, &sendBuf);
					perror("Problem opening file");
				}
				else
				{
					// Send Loop
					while(!fileSent)
					{
						// Start Writing * Priority
						getSem(semid);
						printf("%s%d%s%d\n", "Got sem ID: ", pid, " priority: ", priority);
						for(int j = 0; j < priority; j++)
						{
							printf("Writing to: %d\n", pid);
							fgets(sendBuf.mtext, MTEXTLEN, fileHandle);
							sendBuf.mtype = pid;

							send_message(qid, &sendBuf);
							
							if(feof(fileHandle))
							{
								strcpy(sendBuf.mtext, "\x04");
								send_message(qid, &sendBuf);
								fileSent = 1;
								break;
							}
						}

						releaseSem(semid);
						printf("%s%d\n\n", "Released sem ID: ", pid);
						sched_yield();
					}
					exit(0);
				}
				
			}

			// Reset Connection;
			numClients++;
			newConnection = -1;
		}
		
	}
	
	return 0;
}



int initsem (key_t key)
{
  int sid;
  sid = semget( (key_t)key, 1, IPC_CREAT | 0660 );
  if(sid == -1)
  {
  	perror("semget error:\n");
  }
  return sid;
}


void getSem(int sid)     /* acquire semophore */
{
    struct sembuf sembuf_ptr = {0, -1, 0};

    if ((semop(sid, &sembuf_ptr, 1)) == -1)
    {
    	perror("semop error\n");
    }
}

void releaseSem(int sid)     /* release semaphore */
{
	struct sembuf sembuf_ptr = {0, 1, 0};

    if ((semop(sid, &sembuf_ptr, 1)) == -1)
    {
    	perror("semop error\n");
    }
}

void quitEverything(int numClients, int qid, int semid)
{
	for(int i = 0; i < numClients; i++)
			{
				struct my_message killStruct;
				killStruct.mtype = DIEMSGID;
				send_message(qid, &killStruct);
			}

			msgctl (qid, IPC_RMID, 0);
			semctl(semid, 0, IPC_RMID, 0);
			kill(0, SIGKILL);
}