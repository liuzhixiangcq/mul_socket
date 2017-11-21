/* @author page
 * @date 2017/11/21
 */

#include "client.h"
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int init_connection(char* ip,int port)
{
	int fd;
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
	{
		printf("init_connection:socket failed\n");
		return -1;
	}
	struct sockaddr_in s_addr;
	memset(&s_addr,0,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr(ip);
	s_addr.sin_port = htons(port);

	int ret = connect(fd,(struct sockaddr*)&s_addr,sizeof(struct sockaddr));
	if(ret < 0)
	{
		printf("init_connection:connect failed\n");
		return -1;
	}
	return fd;
}
int main(int argc,char* argv[])
{
	char cmd[256];
	char data[256];
	int fd;
	fd = init_connection("127.0.0.1",10000);
	if(fd < 0)
	{
		printf("init connection failed\n");
		return -1;
	}
	while(1)
	{
		scanf("%s",cmd);
		printf("exec cmd:%s\n",cmd);
		if(strcmp(cmd,"read") == 0)
		{
			write(fd,cmd,sizeof(cmd));
			read(fd,data,256);
			printf("read data:%s\n",data);
		}
		else if(strcmp(cmd,"write") == 0)
		{
			write(fd,cmd,sizeof(cmd));
			printf("input the send data:\n");
			scanf("%s",data);
			write(fd,data,sizeof(data));
		}
		else if(strcmp(cmd,"quit") == 0)
		{
			write(fd,cmd,sizeof(cmd));
			read(fd,data,256);
			if(strcmp(data,"quit") == 0)
			{
				break;
			}
		}
	}
	printf("client exit\n");
	return 0;
}
