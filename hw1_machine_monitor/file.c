#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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
int main(int argc,char* argv[])
{

	char *pointing = malloc(1);
	char *number = malloc(1);
	number = "694";
	return_ancient(pointing,number);
	printf("ancient is:%s\n",pointing);
	return_pid(pointing);
	printf("pid is:%s\n",pointing);
	return_thread(pointing,number);
	printf("thread is:%s\n",pointing);
	return_name(pointing,number);
	printf("name is:%s",pointing);
	return_state(pointing,number);
	printf("state is:%s\n",pointing);
	return_cmdline(pointing,number);
	printf("cmdline is:%s\n",pointing);
	parent_Pid(pointing,number);
	printf("parent's Pid is:%s\n",pointing);
//printf("%s\nnum is %d",name,count);
	return_VmSize(pointing,number);
	printf("VmSize is:%s",pointing);
	return_VmRss(pointing,number);
	printf("VmRss is:%s",pointing);

	return 0;
}
void return_pid(char *result)
{
	char test[10];
	char buffer[256];
	struct dirent* ent = NULL;
	DIR *pDir;
	if((pDir = opendir("/proc")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int i = 0;
	while((ent = readdir(pDir)) != NULL) {
		//strcat(buffer,ent->d_name);
		//strcat(buffer,"");
		strcpy(test,ent->d_name);
		if(test[0] == '1')
			i = 1;
		if(i) {
			printf("this is %s ",ent->d_name);
			strcat(buffer,ent->d_name);
			strcat(buffer," ");
		}
	}
	closedir(pDir);
	sprintf(result,"%s",buffer);

}

void return_thread(char *result,char *number)
{
	char buffer[20];
	struct dirent* ent = NULL;
	DIR *pDir;
	if((pDir = opendir("/proc/2/task")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	int i = 0;
	while((ent = readdir(pDir)) != NULL) {
		if(i >1) {
			strcat(buffer,ent->d_name);
			strcat(buffer," ");
		}
		i++;
	}
	closedir(pDir);
	sprintf(result,"%s",buffer);
}

void return_child(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};

	char addr[50] = {};
	char num[10] = {"1"};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/task");
	strcat(addr,number);
	strcat(addr,"/children");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}

	fscanf(pfile,"%s",buffer);
	fclose(pfile);

	sprintf(result,"%s",buffer);
}

void return_name(char *result,char *number)
{
	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};

	char addr[50] = {};
	char num[10] = {"1"};
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
	char num[10] = {"2"};
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
	char buffer[32] = {};

	char addr[50] = {};
	char num[10] = {"2"};
	strcat(addr,"/proc/");
	strcat(addr,number);
	strcat(addr,"/cmdline");

	if((pfile = fopen(addr,"r")) == NULL) {
		printf("FAILED");
		exit(1);
	}
	fscanf(pfile,"%s",buffer);
	fclose(pfile);



	sprintf(result,"%s",buffer);
}

void parent_Pid(char *result,char *number)
{

	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};

	char addr[50] = {};
	char num[10] = {"2"};
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
	char *test = number;
	//printf("%s\n",test);
	do {
		parent_Pid(result,test);
		//printf("%s",result);
		if(result[0] != '1') {
			strcat(buffer,result);
			strcat(buffer," ");
			test = result;
		}
	} while(result[0] != '1');

	strcat(buffer,"1");
	sprintf(result,"%s",buffer);
}
void return_VmSize(char *result,char *number)
{

	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};

	char addr[50] = {};
	char num[10] = {"1"};
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
	fclose(pfile);
	sprintf(result,"%s",name);
}
void return_VmRss(char *result,char *number)
{

	FILE *pfile;
	char buffer[20] = {};
	char output[256] = {};

	char addr[50] = {};
	char num[10] = {"1"};
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
	fclose(pfile);



	sprintf(result,"%s",name);
}
