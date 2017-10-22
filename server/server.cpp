#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#include "server.h"
void FServer::test(){
	printf("hello world!");
}
int FServer::flisten(int port){
	this->sockserver = socket(AF_INET, SOCK_STREAM, 0);
	if(this->sockserver == 11)
	{
		printf("socket");
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);		//set listen port
	addr.sin_addr.s_addr = INADDR_ANY;
	int res = bind(this->sockserver, (struct sockaddr*)&addr, sizeof(addr));
	if(res == -1)
	{
		perror("bind");
		return -1;
	}
	if(listen(this->sockserver, 1024) == -1)
	{
		perror("listen");
		return -1;
	}
	return 0;
}
void FServer::wait(){
	fd_set fdSockets;
	FD_ZERO(&fdSockets);
	FD_SET(this->sockserver, &fdSockets);	//add server socket
	//wait client connection
	while(true)
	{
		fd_set fdReadSockets;//= fdSockets;
		FD_ZERO(&fdReadSockets);
		FD_SET(this->sockserver, &fdReadSockets);
		for(int i = 0; i < this->iClientCount; i ++)
		{
			FD_SET(this->sClientSockets[i], &fdReadSockets);
		}
		int iResult = select(GetMaxClient(), &fdReadSockets, NULL, NULL, NULL);
		if(iResult > 0)
		{

			if(FD_ISSET(this->sockserver, &fdReadSockets)) //new client
			{
				struct sockaddr_in sAddrClient;
				socklen_t len = sizeof(sockaddr_in);
				int socketClient = accept(this->sockserver, (struct sockaddr*)&sAddrClient, &len);
				if(this->AddClient(socketClient) == 0)
				{
					FD_SET(socketClient, &fdSockets);
					printf("a new client:%s\n", inet_ntoa(sAddrClient.sin_addr));
				}
				else							//max and close this socket
				{
					close(socketClient);
				}
			}
			else
			{
				for(int i = 0; i < this->iClientCount; i ++)
				{
					//have client message
					if(FD_ISSET(this->sClientSockets[i], &fdReadSockets))
					{
						ssize_t st = recv(this->sClientSockets[i], this->buffer, sizeof(this->buffer), 0);
						if(st > 0)
						{
							this->buffer[st] = 0;
							printf("%ssend message:%s\n", "", this->buffer);
							strcpy(this->buffer, "hello!This is my game server\0");
							send(this->sClientSockets[i], this->buffer, strlen(this->buffer), 0);
						}
						else			//clien close connection
						{
							printf("close client\n");
                            printf("client count:%d\n", this->iClientCount);
							FD_CLR(this->sClientSockets[i], &fdSockets);
							int client = this->RemoveClient(this->sClientSockets[i]);	//remove client
							if(client != -1)
								close(client);											//close client
						}
					}
				}
			}
		}
	}
}

//int main(void){
//	FServer f;
//	f.test();
//  return 0;
//}
