#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
int main()
{
	char msg[40] = "";
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)
	{
		perror("socket failed :");
		exit(1);
	}
	else
	{
		printf("sfd : %d\n",sfd);
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(8888);
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
		int len = sizeof(server);		
		char buff[500];
		int c = connect(sfd,(struct sockaddr *)&server,len);
		if(c == -1)
		{
			perror("connection failed :");
			exit(1);
		}
		else
		{
			while(1)
			{
				printf("Enter the command # ");
				fgets(msg,40,stdin);
				//scanf("%s",msg);
				int len = strlen(msg);
				//printf("message to be send from server : %s\n",msg);
				send(sfd,msg,len-1,0);
//				printf("Waiting for message from server : \n");
				int r = recv(sfd,buff,sizeof(buff),0);
				buff[r] = '\0';
				//printf("message received from server : %s\n",buff);
				printf("%s\n",buff);
			}
		}
	}
}
