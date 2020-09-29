#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
	int sock;
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	size_t sendlen;

	struct sockaddr_in servaddr;
	struct sockaddr_in clntaddr;

	int clntaddrsize;

	if(argc!=3)
	{
	printf("usage : %s <ip> <port> \n", argv[0]);
	exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	printf("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	printf("connect error");

	printf(" chatting start \n");
	printf(" if you want to exit, enter /quit \n");

	while(1)
	{
	memset(sendbuf, 0, strlen(sendbuf));
	printf("client : ");
	fgets(sendbuf, BUFSIZE, stdin);
	sendlen = strlen(sendbuf);
	ssize_t comu_num = send(sock, sendbuf, sendlen, 0);

	if(comu_num == -1)
	printf("send error");

	if(strcmp(sendbuf, "/quit\n") == 0)
	break;

	memset(recvbuf, 0, sizeof(recvbuf));
	comu_num = recv(sock, recvbuf, BUFSIZE, 0);

	if(comu_num == -1)
	printf("recv error");

	if(strcmp(recvbuf, "/quit\n")==0)
	break;

	printf("server : %s", recvbuf);
	memset(recvbuf,0, sizeof(recvbuf));
	}

	fputc('\n', stdout);
	printf(" chat has ended \n");
	close(sock);

	return 0;
}
