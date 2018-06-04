#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
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
		struct sockaddr_in server,client;
		server.sin_family = AF_INET;
		server.sin_port = htons(8888);
		server.sin_addr.s_addr = INADDR_ANY;
			//int len = sizeof(server);		
		
		int c = bind(sfd,(struct sockaddr *)&server,sizeof(server));
		if(c == -1)
		{
			perror("connection failed :");
			exit(1);
		}
		else
		{
			int l = listen(sfd,1);
			if(sfd == -1)
			{
				perror("listen failed :");
				exit(1);
			}
			
			int len = sizeof(client);
		 	while(1)
			{
			int cfd = accept(sfd,(struct sockaddr *)&client,&len);
			if(cfd == -1)
			{
				perror("accept failed :");	
				exit(1);
			}
			int ret = fork();
			if(ret == 0)
			{	
			char command[20]=" > temp.txt";		
			printf("client ip : %s\n",inet_ntoa(client.sin_addr));
			while(1)
			{
				strcpy(msg,"");
//				printf("Waiting for message from client : \n");
				int r = recv(cfd,msg,40,0);
				if(r == -1)
				{
					perror("recv failed :");
					exit(1);
				}
				else if(r == 0)
				{
					printf("client disconnected : %s\n",inet_ntoa(client.sin_addr));
					exit(1);
				}
				else
				{
					msg[r] = '\0';
//					printf("message received from client : %s\n",msg);
					if(strcmp(msg,"cd"))
					{
						strcat(msg,command);
//						printf("msg after strcat: %s\n",msg);
						system(msg);
						char buff[500];
						int fd = open("temp.txt",O_RDONLY,0666);
						if(fd == -1)
						{
							perror("open failed :");
							exit(1);
						}
						else
						{
							int r1 = read(fd,buff,sizeof(buff));
							if(r1 == -1)
								perror("read failed : ");
							else
							{
								buff[r1]='\0';
								send(cfd,buff,sizeof(buff),0);
							}
						}
					}
					else
					{
//						printf("cd detected\n");
						send(cfd,"Enter the directory path :",27,0);
						int rv = recv(cfd,msg,sizeof(msg),0);
						msg[rv]='\0';
						chdir(msg);
						send(cfd,"Directory changed",18,0);
						
					}
				//	int cmd_sz = sizeof(msg);
				//	strcat(command,msg);
				//	printf("command : %s",command);
					//system(msg);
					
					//printf("Enter the message to client : ");
					//fgets(msg,40,stdin);
					//send(cfd,buff,sizeof(buff),0);
				}
			}
			}
			}
		}
	}
}
