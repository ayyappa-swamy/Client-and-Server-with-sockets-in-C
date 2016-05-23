#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<fcntl.h>
#include<string.h>

typedef struct marks
{
	int year1[6];
	int year2[6];
}MARKS;

typedef struct details
{
	char name[20];
	char mname[20];
	char fname[20];
	char rollno[10];
	char address[32];
}DETAILS;

typedef struct student
{
	DETAILS stu_det;
	MARKS stu_marks;
}STUDENT;

void Get_file(SOCKET s, char *roll)
{
	FILE *fp;
	int j = 0, i = 0, no_ch;
	char *fil_name = (char*)calloc(15, sizeof(char));
	recv(s, fil_name, 15, 0);
	char* msg = (char*)calloc(1000, sizeof(char));
	
	char *ch = "/";
	strcat(roll, ch);
	strcat(roll, fil_name);

	fp = fopen(roll, "rb+");

	while (fgetc(fp) != EOF)
		i++;

	no_ch = (i / 1000) + 1;

	send(s, (char*)&no_ch, sizeof(int), 0);
	fseek(fp, 0, SEEK_SET);
	for (j = 1; j < no_ch; j++){
		fread(msg, sizeof(char), 1000, fp);
		send(s, msg, 1000, 0);
	}
	fread(msg, sizeof(char), i % 1000, fp);
	send(s, msg, strlen(msg), 0);
	printf("\nFile sent.");
	getchar();
	return;
}

void Add_file(SOCKET s, char *roll)
{
	char *fil_name = (char*)calloc(15, sizeof(char));
	char *msg = (char*)calloc(1000, sizeof(char));
	int no_ch,j;

	recv(s, fil_name, 15, 0);
	mkdir(roll);
	char *ch = "/";
	strcat(roll, ch);
	strcat(roll,"/Debug/");
	strcat(roll, fil_name);
	FILE *fp = fopen(roll, "wb+");
	recv(s, &no_ch, sizeof(int), 0);

	for (j = 1; j < no_ch; j++){
		recv(s, msg, 1000, 0);
		//printf("%s", msg);
		fwrite(msg, sizeof(char),1000, fp);
	}
	recv(s, msg, 1000, 0);
	fwrite(msg, sizeof(char), strlen(msg), fp);
	fclose(fp);
	return;
}

void View_marks(SOCKET s, char *roll)
{
	FILE *fp;
	int count;
	STUDENT stu;
	int size;

	fp = fopen("detailsdb.bin", "rb+");
	count = fgetc(fp);

	fread(&stu, sizeof(STUDENT), 1, fp);
	while ((count > 0) && (strcmp(stu.stu_det.rollno, roll) != 0)){
		fread(&stu, sizeof(STUDENT), 1, fp);
		count--;
	}
	if (strcmp(stu.stu_det.rollno, roll) == 0){
		printf("%d", stu.stu_marks.year1[3]);
		size = send(s, (char*)&stu.stu_marks, sizeof(MARKS), 0);
	}

	fclose(fp);
	return;
}

void View_details(SOCKET s, char *roll)
{
	FILE *fp;
	int count;
	STUDENT stu;
	int size;

	fp = fopen("detailsdb.bin", "rb+");
	count = fgetc(fp);

	fread(&stu, sizeof(STUDENT), 1, fp);
	while ((count > 0) && (strncmp(stu.stu_det.rollno, roll, strlen(stu.stu_det.rollno)) != 0)){
		fread(&stu, sizeof(STUDENT), 1, fp);
		count--;
	}
	if (strncmp(stu.stu_det.rollno, roll, strlen(stu.stu_det.rollno)) == 0){
		printf("%s", stu.stu_det.name);
		size = send(s, (char*)&stu, sizeof(STUDENT), 0);
	}

	fclose(fp);
	return;
}

void Add_marks(SOCKET s, char *roll)
{
	FILE *fp;
	STUDENT stu;
	MARKS *stm;
	char *rnum;
	char *instr = (char*)calloc(48, sizeof(char));
	int count,i,size,year;

	size = recv(s, &year, sizeof(int), 0);

	size = recv(s, instr, 48, 0);
	instr[size] = '\0';

	stm = (MARKS *)instr;

	fp = fopen("detailsdb.bin", "rb+");
	i = count = fgetc(fp);
	fread(&stu, sizeof(STUDENT), 1, fp);
	while ((i > 0) && (strcmp(stu.stu_det.rollno, roll) != 0)){
		fread(&stu, sizeof(STUDENT), 1, fp);
		i--;
	}
	if (strcmp(stu.stu_det.rollno, roll) == 0){
		if (year == 1){
			for (i = 0; i < 6; i++)
				stu.stu_marks.year1[i] = stm->year1[i];
		}
		else if (year == 2){
			for (i = 0; i < 6; i++)
				stu.stu_marks.year2[i] = stm->year2[i];
		}
	}

	fseek(fp, -sizeof(STUDENT), SEEK_CUR);
	fwrite(&stu, sizeof(STUDENT), 1, fp);

	fclose(fp);
	return;
}

void Add_details(SOCKET s, char * roll)
{
	FILE *fp;
	int ch = 0;
	fp = fopen("detailsdb.bin", "rb+");
	if (fp == NULL){
		fp = fopen("detailsdb.bin", "wb+");
		fputc(0,fp);
	}
	fseek(fp, 0, SEEK_SET);
	ch = fgetc(fp);
	STUDENT *stu;
	char *instr = (char*)calloc(160, sizeof(char));
	int size;

	size = recv(s, instr, sizeof(STUDENT), 0);
	instr[size] = '\0';

	stu = (STUDENT *)instr;

	fseek(fp, ch * sizeof(STUDENT), SEEK_CUR);
	fwrite(stu, sizeof(STUDENT), 1, fp);
	fseek(fp, 0, SEEK_SET);
	fputc(ch + 1,fp);

	fclose(fp);
	return;
}

DWORD WINAPI receiver(void *lp)
{
	char *instr = (char*)calloc(100, sizeof(char));
	SOCKET s = *((SOCKET *)lp);
	int option;
	int size;

	size = recv(s, instr, 4, 0);
	instr[size] = '\0';

	option = *((int *)instr);

	printf("\nReceived choice is %d", option);

	size = recv(s, instr, 10, 0);
	instr[size+1] = '\0';
	printf("\nreceived roll no. is %s", instr);

	switch (option)
	{
	case 1:Add_details(s, instr);
		break;
	case 2:Add_marks(s, instr);
		break;
	case 3:Add_file(s, instr);
		break;
	case 4:View_details(s, instr);
		break;
	case 5:View_marks(s, instr);
		break;
	case 6:Get_file(s, instr);
		break;
	}
	return 0;
}

int main()
{
	SOCKET s,new_s;
	WSADATA wsa;
	struct sockaddr_in server, client;

	int c;

	printf("Initializing Winsock....\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error: %d", WSAGetLastError());
		return 1;
	}
	printf("Winsock Initialised.\n");

	printf("Creating Socket...\n");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		printf("Failed. Error : %d", WSAGetLastError());
		return 1;
	}
	printf("Socket created .\n");

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	printf("Binding socket...\n");
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
		printf("Binding Error.\n");
		return 1;
	}
	printf("Bound \n");

	printf("Listening ...\n");
	listen(s, 3);

	c = sizeof(struct sockaddr_in);

	while ((new_s = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET){
		printf("Connection accepted.\n");
		CreateThread(0, 0, &receiver, (void *)&new_s, 0, 0);
	}

	getchar();
	closesocket(s);
	WSACleanup();

	return 0;
}