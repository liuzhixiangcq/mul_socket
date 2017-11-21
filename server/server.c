/* @author page
 * @date 2017/11/21
 */

#include "server.h"
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* io_fun(void* arg)
{
	int fd = *(int*)arg;
	if(fd <= 0)
	{
		printf("io_fun:arg error\n");
		return NULL;
	}
	char cmd[256];
	char buf[256];
	while(1)
	{
		read(fd,cmd,256);
		printf("from fd:%d cmd:%s\n",fd,cmd);
		if(strcmp(cmd,"read") == 0)
		{
			write(fd,"hello world\n",256);
			printf("comlete write\n");
		}
		else if(strcmp(cmd,"write") == 0)
		{
			read(fd,buf,256);
			printf("recv data:%s\n",buf);
		}
		else if(strcmp(cmd,"quit") == 0)
		{

			write(fd,"quit",4);
			break;
		}
	}
}
int produce_io_thread(int fd)
{
	int ret;
	pthread_t tid;
	ret = pthread_create(&tid,NULL,io_fun,(void*)&fd);
	if(ret)
	{
		printf("produce_io_thread:pthread_create failed\n");
		return -1;
	}
	return 0;
}
int init_service(char* ip,int port)
{
	struct sockaddr_in s_addr;
	memset(&s_addr,0,sizeof(s_addr));

	int s_fd = socket(AF_INET,SOCK_STREAM,0);
	if(s_fd < 0)
	{
		printf("init_service:socket failed\n");
		return -1;
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr(ip);
	s_addr.sin_port = htons(port);

	int ret = bind(s_fd,(struct sockaddr*)&s_addr,sizeof(struct sockaddr));
	if(ret < 0)
	{
		printf("init_service:bind failed\n");
		return -1;
	}
	listen(s_fd,10);
	while(1)
	{
		struct sockaddr_in r_addr;
		int socklen = sizeof(struct sockaddr_in);
		int new_fd = accept(s_fd,(struct sockaddr*)&r_addr,(socklen_t*)&socklen);
		produce_io_thread(new_fd);
	}
	return 0;
}
int main(int argc,char* argv[])
{
	init_service("127.0.0.1",10000);
	return 0;
}
