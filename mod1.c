
#include<stdio.h>

#include<fcntl.h>

#include<unistd.h>

#include<stdlib.h>

#include<string.h>

#include<sys/socket.h>

#include<sys/types.h>

#include<unistd.h>

#include<netinet/in.h>

#include <errno.h>  

#include <stddef.h>

#include <time.h>

#define OUTLEN 255

#define PORT 6666

void split(char dst[][12], char* str)
{
     int n = 0;
    char *result = NULL;
    result = strtok(str, "+");
    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, "+");
    }   
}
int main(int argc,char **argv)
{	
    int fd1=creat("2nd.log",0765);
	open("2nd.txt",O_RDWR);
	int ser_sockfd,cli_sockfd;
	int err,n;
	int addlen;
	struct sockaddr_in ser_addr;
	struct sockaddr_in cli_addr;
	char recvline[4][100],sendline[200];
	ser_sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(ser_sockfd==-1)
	{
		printf("socket error:%s\n",strerror(errno));
		return -1;
	}
	bzero(&ser_addr,sizeof(ser_addr));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	ser_addr.sin_port=htons(PORT);
	err=bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
	if(err==-1)
	{
		printf("bind error:%s\n",strerror(errno));
		return -1;
	}
	err=listen(ser_sockfd,5);
	if(err==-1)
	{
		printf("listen error\n");
		return -1;
	}
	printf("在服务中:\n");
	char str[4][12];
	while(1)
	{	
		addlen=sizeof(struct sockaddr);
		cli_sockfd=accept(ser_sockfd,(struct sockaddr *)&cli_addr,&addlen);
		if(cli_sockfd==-1)
		{
			printf("接收错误\n");
		}
		while(1)
	  {	
			printf("等待客户办理业务...\n");
			printf("请告知你的号码和需要的业务:\n");	
     		for (int i = 0; i < 4; i++) 
     	   {	
 	 	    n=recv(cli_sockfd,recvline[i],1024,0);
      		recvline[i][n]='\0';
      	   }
      	  for (int i = 0; i < 4; i++) 
    	{
    		char dst[2][12];
    		split(dst,recvline[i]);
    		if(strcmp(dst[0],"finish")==0) 
			{ 
		    	printf("停止办理\n");
		    	exit(0);
			}
			if(n==-1)
			{
				printf("接收数据错误\n");
			}
			else if(strlen(dst[0])!=11 || strcmp(dst[1],"停机")!=0 && strcmp(dst[1],"复话")!=0)
		 {				
			strcpy(sendline,"succeed-return-1");
			send(cli_sockfd,sendline,strlen(sendline),0);
			printf("%s:",dst[0]);
			printf("输入错误\n");
		 }	
			else 
		  {
			strcpy(sendline,"succeed-return-0");
			send(cli_sockfd,sendline,strlen(sendline),0);			
			time_t timep;
			time (&timep);
			write(fd1,asctime(gmtime(&timep)),20);
			write(fd1,dst[0],11);
			if(strcmp(dst[1],"停机")==0)
			{
				write(fd1," off\n",5);
			}
			else if(strcmp(dst[1],"复话")==0)
			{
				write(fd1," on\n",4);
			}	   	
   		   printf("办理的号码是:%s\n",dst[0]);		
   		   printf("申请的业务是:%s\n",dst[1]);	
   		   printf("输入正确\n");				
		 }  		  
   	 }	    	   
  }			
		close(cli_sockfd);
}	
	close(ser_sockfd);
	close(fd1);
	return 0;
}

