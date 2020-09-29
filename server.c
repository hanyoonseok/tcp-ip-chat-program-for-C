#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFSIZE 1024

int main(int argc, char**argv)
{
	int servsock;
	int clntsock;
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	int sendlen;
	int recvlen;

	struct sockaddr_in servaddr;
	struct sockaddr_in clntaddr;

	int clntaddrsize;

	servsock = socket(PF_INET, SOCK_STREAM, 0);

	if(argc!= 2)
	{
	printf("usage : %s <port> \n", argv[0]);
	exit(1);
	}

	if(servsock == -1)
	printf("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if(bind(servsock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	printf("bind error");

	if(listen(servsock, 5) == -1)
	printf("listen error");

	printf("1:1 chatting server is open. waiting for client access..\n");

	clntaddrsize = sizeof(clntaddr);
	clntsock = accept(servsock, (struct sockaddr*)&clntaddr, &clntaddrsize);

	if(clntsock == -1)
	printf("accept error");

	printf("chatting start! \n");
	printf("if you want to exit, enter /quit \n");
	
	while(1)
	{
	memset(recvbuf, 0, sizeof(recvbuf));

	ssize_t recvnum = recv(clntsock, recvbuf, BUFSIZE, 0);
	if(recvnum == -1)
	printf("recv error");

	printf("client : %s", recvbuf);
	recvbuf[strlen(recvbuf)-1] = '\0';

	if(strcmp(recvbuf, "/quit")==0)
	break;

	memset(recvbuf, 0, sizeof(recvbuf));
	memset(sendbuf, 0, sizeof(sendbuf));

	printf("server : ");
	fgets(sendbuf, BUFSIZE, stdin);

	if(send(clntsock, sendbuf, strlen(sendbuf), 0)==-1)
	printf("send() error");

	if(strcmp(sendbuf, "/quit\n")==0)
	break;
	}

	fputc('\n', stdout);
	printf("chat has ended \n");
	close(clntsock);	
	close(servsock);
	return 0;
}
