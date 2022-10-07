#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h> 

int main(int argc, char * argv[])
{
	if(argc == 5){
        int sockid;
	int server_port = atoi(argv[2]);
	char *server_ip = argv[1];
	char *msg = argv[3];
        char *msg1 = "   ";
	char *msg2 = argv[4];

	sockid = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	connect(sockid,(struct sockaddr *)&server_addr, sizeof(server_addr));
	send(sockid, (const char *)msg, strlen(msg),0);
	send(sockid, (const char *)msg1, strlen(msg1),0);
	send(sockid, (const char *)msg2, strlen(msg2),0);

	close(sockid);}
	else{printf("read the readme.txt");}
}
