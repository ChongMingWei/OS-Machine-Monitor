#include "client.h"
#include <stdio.h>
int main(int argc, char **argv)
{
	int client_socket;
	struct sockaddr_in client;
	char option;
	char server_response[2048] = {};
	char pid[32];

	//build the socket
	client_socket = socket(AF_INET,SOCK_STREAM,0);
	if(client_socket < 0) {
		printf("Could not create socket:(");
	}

	//server connection
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_family = AF_INET;
	client.sin_port = htons(59487);
	if(connect(client_socket,(struct sockaddr*)&client,sizeof(client)) < 0) {
		puts("Connect Error");
		return 1;
	}
	puts("Connected");

	//interaction with server

	for(;;) {
		printf("==================================================\n");
		printf("(a)list all process IDs\n");
		printf("(b)thread's IDs\n");
		printf("(c)child's PIDs\n");
		printf("(d)process name\n");
		printf("(e)state of process(D,R,S,T,t,W,X,Z)\n");
		printf("(f)command line of excuting process(cmdline)\n");
		printf("(g)parent's PID\n");
		printf("(h)all ancients of PIDs\n");
		printf("(i)virtual memory size(VmSize)\n");
		printf("(j)physical memory size(VmRss)\n");
		printf("(k)exit\n");
		printf("which one? ");
		fflush(stdin);
		scanf("%c",&option);
		getchar();
		if(option == 'k') {
			return 1;
		} else if(option == 'a') {
			if(send(client_socket,&option,sizeof(option),0) < 0) {
				puts("Send Failed");
				return 1;
			}
			if(recv(client_socket,server_response,2048,0) < 0) {
				puts("Receive Failed");
				return 1;
			}
			puts(server_response);
			memset(server_response,0,sizeof(server_response));
		} else if(option == 'b' || option == 'c' || option == 'd' || option == 'e'
		          || option == 'f' || option == 'g' || option == 'h' || option == 'i'
		          || option == 'j') {
			if(send(client_socket,&option,sizeof(option),0) < 0) {
				puts("Send Failed");
				return 1;
			}
			if(recv(client_socket,server_response,2048,0) < 0) {
				puts("Receive Failed");
				return 1;
			}
			puts(server_response);
			memset(server_response,0,sizeof(server_response));
			stdin = freopen(NULL,"r",stdin);
			scanf("%s",pid);
			getchar();
			if(send(client_socket,pid,sizeof(pid),0) < 0) {
				puts("Send Failed");
				return 1;
			}
			//printf("send success\n");  check if the message is sent
			if(recv(client_socket,server_response,2048,0) < 0) {
				puts("Receive Failed");
				return 1;
			}
			//printf("receive succes\n"); check if the message is sent
			puts(server_response);
			memset(server_response,0,sizeof(server_response));
		} else {
			if(send(client_socket,&option,sizeof(option),0) < 0) {
				puts("Send Failed");
				return 1;
			}
			if(recv(client_socket,server_response,2048,0) < 0) {
				puts("Receive Failed");
				return 1;
			}
			puts(server_response);
			memset(server_response,0,sizeof(server_response));
		}
	}
	return 0;
}

