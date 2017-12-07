#include <stdio.h>
#include "server.h"
#include <dirent.h>
void *connection_handler(void *);

void return_pid(char *result);
void return_thread(char *result,char *number);
void return_child(char *result,char *number);
void return_name(char *result,char *number);
void return_state(char *result,char *number);
void return_cmdline(char *result,char *number);
void parent_Pid(char *result,char *number);
void return_ancient(char *result,char *number);
void return_VmSize(char *result,char *number);
void return_VmRss(char *result,char *number);

int main(int argc, char **argv)
{
	int server_socket,new_socket,client_fd,*socket_pointer;
	struct sockaddr_in server,client;

	//build socket
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket < 0) {
		printf("Could not create socket:(");
	}

	//server address bind
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(59487);
	if(bind(server_socket,(struct sockaddr *)&server,sizeof(server)) < 0) {
		puts("Bind Failed");
	} else {
		puts("Bind Done");
		listen(server_socket,5);
		puts("Waiting for incoming connection:");
	}
	client_fd = sizeof(struct sockaddr_in);

	//interact with incoming client
	while(new_socket = accept(server_socket,(struct sockaddr *)&client,
	                          (socklen_t*)&client_fd)) {
		puts("Connection Accepted");
		pthread_t thread_detect;
		socket_pointer = malloc(1);
		*socket_pointer = new_socket;

		if(pthread_create(&thread_detect,NULL,connection_handler,
		                  (void*)socket_pointer) < 0) {
			perror("could not create thread");
			return 1;
		}
		puts("Handler assigned");
	}
	if(new_socket < 0) {
		perror("Accept Failed");
		return 1;
	}
	free(socket_pointer);
	return 0;
}
void *connection_handler(void *socket_desc)
{
	int sock = *(int*)socket_desc;
	int read_size;
	char message[2048] = {};
	char recv_opt;
	char recv_pid[32] = {};
	char quest[10] = {"[pid?]"};
	char error[16] = {"input error!!"};
	while((read_size = recv(sock,&recv_opt,1,0)) > 0) {
		switch(recv_opt) {
		case 'a':
			return_pid(message);
			break;
		case 'b': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_thread(message,recv_pid);
		}
		break;
		case 'c': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_child(message,recv_pid);
		}
		break;
		case 'd': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_name(message,recv_pid);
		}
		break;
		case 'e': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_state(message,recv_pid);
		}
		break;
		case 'f': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_cmdline(message,recv_pid);
		}
		break;
		case 'g': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			parent_Pid(message,recv_pid);
		}
		break;
		case 'h': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_ancient(message,recv_pid);
		}
		break;
		case 'i': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_VmSize(message,recv_pid);
		}
		break;
		case 'j': {
			write(sock,quest,strlen(quest));
			recv(sock,recv_pid,sizeof(recv_pid),0);
			return_VmRss(message,recv_pid);

		}
		break;
		default: {
			write(sock,error,strlen(error));
		}
		break;
		}
		write(sock,message,strlen(message));
		printf("write success\n");
		memset(message,0,sizeof(message));

	}

	if(read_size == 0) {
		puts("Nothing received");
		fflush(stdout);
	}

	else if(read_size == -1) {
		perror("recv failed");
	}
	return 0;
}

void return_pid(char *result)
{
	char test[10];
	char buffer[2048];
	struct dirent* ent = NULL;
	DIR *pDir;
	if((pDir = opendir("/proc")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int i = 0;
	while((ent = readdir(pDir)) != NULL) {
		strcpy(test,ent->d_name);
		if(test[0] == '1')
			i = 1;
		if(i) {
			strcat(buffer,ent->d_name);
			strcat(buffer," ");
		}
	}
	closedir(pDir);
	sprintf(result,"%s",buffer);
}

void return_thread(char *result,char *number)
{
	char buffer[128];
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/task");
	struct dirent* ent = NULL;
	DIR *pDir;

	if((pDir = opendir(addr)) == NULL) {
		strcat(buffer,"None");
	} else {
		int i = 0;
		while((ent = readdir(pDir)) != NULL) {
			if(i >1) {
				strcat(buffer,ent->d_name);
				strcat(buffer," ");
			}
			i++;
		}
	}
	closedir(pDir);
	sprintf(result,"%s",buffer);

}

void return_child(char *result,char *number)
{
	FILE *pfile;
	char buffer[16] = {};
	char result_buf[2048] = {};
	char addr[128] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/task/");
	strcat(addr,number);
	strcat(addr,"/children");

	pfile = fopen(addr,"r");
	while(fscanf(pfile,"%s",buffer) != EOF) {
		strcat(result_buf,buffer);
		strcat(result_buf," ");
	}
	if(result_buf[0] == 0)
		strcat(result_buf,"No child");
	fclose(pfile);
	sprintf(result,"%s",result_buf);
}

void return_name(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/status");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int count = 0;
	while(fgets(buffer,20,pfile)!=NULL) {
		if(count == 1)
			break;
		strcat(output,buffer);
		count++;
	}
	char name[10]= {};
	int i;
	int size = strlen(output);
	for(i = 6; i < size; i++)
		name[i-6] = output[i];
	fclose(pfile);
	sprintf(result,"%s",name);
}
void return_state(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/status");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int count = 0;
	while(fgets(buffer,20,pfile)!=NULL) {
		if(count == 1) {
			strcat(output,buffer);
			break;
		}
		count++;
	}
	char name[10]= {};
	name[0] = output[7];
	fclose(pfile);
	sprintf(result,"%s",name);
}

void return_cmdline(char *result,char*number)
{
	FILE *pfile;
	char buffer[16] = {};
	char result_buf[2048] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/cmdline");

	pfile = fopen(addr,"r");
	while(fscanf(pfile,"%s",buffer) != EOF) {
		strcat(result_buf,buffer);
		strcat(result_buf," ");
	}
	if(result_buf[0] == 0)
		strcat(result_buf,"No cmdline");
	fclose(pfile);
	sprintf(result,"%s",result_buf);
}

void parent_Pid(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/status");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int i;
	while(fgets(buffer,20,pfile)!=NULL) {
		if(buffer[0] == 'P' && buffer[1] == 'P') {
			strcat(output,buffer);
			break;
		}
	}
	char name[10]= {};
	int size = strlen(output);
	for(i = 6; i < size-1; i++)
		name[i-6] = output[i];
	fclose(pfile);
	sprintf(result,"%s",name);
}

void return_ancient(char *result,char *number)
{
	char buffer[64];
	int i = 0;
	do {
		parent_Pid(result,number);
		strcat(buffer,result);
		strcat(buffer," ");
		number = result;
		i++;
	} while(result[0] != '0');
	sprintf(result,"%s",buffer);
}

void return_VmSize(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/status");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int i;
	while(fgets(buffer,20,pfile)!=NULL) {
		if(buffer[0] == 'V' && buffer[2] == 'S') {
			strcat(output,buffer);
			break;
		}
	}
	char name[10]= {};
	int size = strlen(output);
	for(i = 8; i < size; i++)
		name[i-8] = output[i];
	if(name[0] == 0)
		strcat(name,"No VmSize");
	fclose(pfile);
	sprintf(result,"%s",name);
}
void return_VmRss(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};
	char addr[50] = {};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/status");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	while(fgets(buffer,20,pfile)!=NULL) {
		if(buffer[0] == 'V' && buffer[2] == 'R') {
			strcat(output,buffer);
			break;
		}
	}
	char name[10]= {};
	int i;
	int size = strlen(output);
	for(i = 7; i < size; i++)
		name[i-7] = output[i];
	if(name[0] == 0)
		strcat(name,"No VmRss");
	fclose(pfile);
	sprintf(result,"%s",name);
}
