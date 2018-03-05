#include "commonCore.h"

int getEmptyClientID(struct connected_client clientList[]);
void initClientList(struct connected_client clientList[]);
void deleteClient(struct connected_client clientList[], int id);


int main (int argc , char *argv[])
{
	struct my_message rcvConnects;
	struct my_message sendBuf;
	int qid = open_queue(6969);
	int running = 1;
	int newConnection;
	int currClients = 0;

	struct connected_client clients[MAXCLIENTS];
	initClientList(clients);

	// Polling Loop
	while(running)
	{
		newConnection = read_message_noblock(qid, CONNECTMSGID, &rcvConnects);

		if(newConnection != -1 && currClients < MAXCLIENTS)
		{

			char tempFileName[MTEXTLEN];
			int tempPid;
			int tempPriority;
			int emptyClient = getEmptyClientID(clients);
			sscanf(rcvConnects.mtext, "%s %d %d", tempFileName, &tempPid, &tempPriority);
			printf("New client wants: %s\nAt pid: %d\nWith priority: %d\n\n", tempFileName, tempPid, tempPriority);

			clients[emptyClient].file = fopen(tempFileName, "r");
			clients[emptyClient].pid = tempPid;
			clients[emptyClient].priority = tempPriority;

			if(clients[emptyClient].file == NULL)
			{
				// Send error string to client
				strcpy(sendBuf.mtext, "\x04 Error: Invalid filename");
				sendBuf.mtype = clients[emptyClient].pid;
				send_message(qid, &sendBuf);
				deleteClient(clients, emptyClient);
				perror("Problem opening file");
			}
			else
			{
				currClients++;
			}

			
			newConnection = -1;
		}
		else if (currClients >= MAXCLIENTS)
		{
			printf("%s\n", "Error: Max Clients reached, wait for a transmission to finish");
		}

		for(int i = 0; i < MAXCLIENTS; i++)
		{
			if(clients[i].pid != 0)
			{
				for(int j = 0; j < clients[i].priority; j++)
				{
					//printf("%s %d\n", "Wrote to", clients[i].pid);
					fgets(sendBuf.mtext, MTEXTLEN, clients[i].file);
					sendBuf.mtype = clients[i].pid;

					send_message(qid, &sendBuf);
					
					if(feof(clients[i].file))
					{
						strcpy(sendBuf.mtext, "\x04");
						send_message(qid, &sendBuf);
						deleteClient(clients, i);
						currClients--;
						break;
					}
					
				}
			}
		}
		
	}
	
	return 0;
}


int getEmptyClientID(struct connected_client clientList[])
{
	for(int i = 0; i < MAXCLIENTS; i++)
	{
		if(clientList[i].pid == 0)
		{
			return i;
		}
	}

	return -1;
}

void initClientList(struct connected_client clientList[])
{
	for(int i = 0; i < MAXCLIENTS; i++)
	{
		clientList[i].file = NULL;
		clientList[i].pid = 0;
		clientList[i].priority = 1;
	}
}

void deleteClient(struct connected_client clientList[], int id)
{
	fclose(clientList[id].file);
	clientList[id].file = NULL;
	clientList[id].pid = 0;
	clientList[id].priority = 0;
}
