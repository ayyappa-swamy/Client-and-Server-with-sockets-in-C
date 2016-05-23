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

void Get_file(SOCKET s)
{
	char *fil_name = (char*)calloc(15, sizeof(char));
	char *msg = (char*)calloc(1000, sizeof(char));
	int no_ch, j;

	printf("Enter filename:");
	fflush(stdin);
	gets(fil_name);
	fflush(stdin);

	send(s, fil_name, 15, 0);
	FILE *fp = fopen(fil_name, "wb+");
	recv(s, &no_ch, sizeof(int), 0);

	for (j = 1; j < no_ch; j++){
		recv(s, msg, 1000, 0);
		fwrite(msg, sizeof(char), 1000, fp);
	}
	recv(s, msg, 1000, 0);
	fwrite(msg, sizeof(char), strlen(msg), fp);
	fclose(fp);
}

void Attach_file(SOCKET s)
{
	FILE *fp;
	int j = 0,i = 0, no_ch;
	char *fil_name = (char*)calloc(15, sizeof(char));
	printf("\nEnter file name : \t\t<including extension>\n:>");
	fflush(stdin);
	gets(fil_name);
	fflush(stdin);

	char* msg = (char*)calloc(1000, sizeof(char));

	fp = fopen(fil_name, "rb+");

	while (fgetc(fp) != EOF)
		i++;

	no_ch = (i / 1000) + 1;

	send(s, fil_name, strlen(fil_name), 0);

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

void Add_details(SOCKET s, char *roll)
{
	int numb;
	STUDENT stu;

	system("cls");
	printf("\nEnter your details ...>>\n");
	
	printf("\nEnter your name : ");
	fflush(stdin);
	gets(stu.stu_det.name);
	fflush(stdin);

	printf("\nEnter your father name : ");
	fflush(stdin);
	gets(stu.stu_det.fname);
	fflush(stdin);
	
	printf("\nEnter your mother name : ");
	fflush(stdin);
	gets(stu.stu_det.mname);
	fflush(stdin);

	printf("\nEnter your address : \t\t<fields separated by commas>\n:>");
	fflush(stdin);
	gets(stu.stu_det.address);
	fflush(stdin);

	strcpy(stu.stu_det.rollno, roll);

	if ((numb = send(s, (char*)&stu, sizeof(STUDENT), 0)) < 0){
		printf("\nError : Details sending failed. ");
	}

	printf("\nDetails sent : %d bytes.",numb);

	return;
}

void Add_marks(SOCKET s)
{
	int i,numb;
	int ch;
	MARKS stumar;

	printf("\nEnter the year\t(1/2) :");
	scanf("%d", &ch);

	send(s, (char*)&ch, sizeof(ch), 0);

	printf("\nEnter your marks ...>\n");
	if (ch == 1){
		for (i = 0; i < 6; i++){
			printf("Subject %d : ", i);
			scanf("%d", &stumar.year1[i]);
		}
	}
	else if (ch == 2){
		for (i = 0; i < 6; i++){
			printf("Subject %d : ", i);
			scanf("%d", &stumar.year2[i]);
		}
	}
	else
		printf("\nInvalid Option.");
	
	if ((numb = send(s, (char*)&stumar, sizeof(MARKS), 0)) < 0){
		printf("\nError : Marks sending failed. ");
	}

	printf("\nMarks sent : %d bytes.", numb);
	getchar();
	return;
}

void View_details(SOCKET s)
{
	DETAILS *det;
	char *instr = (char*)calloc(160, sizeof(char));
	int numb;

	if ((numb = recv(s, instr, sizeof(STUDENT), 0)) < 0){
		printf("Failed in fetching details\n");
		return;
	}
	det = (DETAILS *)instr;

	printf("\nYour details are : >>");
	printf("\nName : %s", det->name);
	printf("\nRoll No : %s", det->rollno);
	printf("\nFather name : %s", det->fname);
	printf("\nMother name : %s", det->mname);
	printf("\nAddress : %s", det->address);

	return;
}

void View_marks(SOCKET s)
{
	MARKS *stumar;
	char *instr = (char*)calloc(100, sizeof(char));
	int numb, ch, i;
	printf("\nEnter the year : ");
	scanf("%d", &ch);

	if ((numb = recv(s, instr, sizeof(MARKS), 0)) < 0){
		printf("Failed in fetching details\n");
		return;
	}
	stumar = (MARKS *)instr;

	printf("Your Marks of year - %d are .. \n", ch);
	if (ch == 1){
		for (i = 0; i < 6; i++){
			printf("\nSubject %d : %d", i,stumar->year1[i]);
		}
	}
	else if (ch == 2){
		for (i = 0; i < 6; i++){
			printf("\nSubject %d : %d", i,stumar->year2[i]);
		}
	}
	else
		printf("\nInvalid Option.");

	getchar();
	return;
}

int main()
{
	int choice;
	SOCKET s;
	WSADATA wsa;
	struct sockaddr_in server;
	char *rollno = (char*)calloc(10,sizeof(char));

	printf("Initializing Winsock....\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error: %d", WSAGetLastError());
		return 1; 
	}
	printf("Winsock Initialised.\n");

	printf("Creating Socket..\n");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		printf("Failed. Error : %d", WSAGetLastError());
		return 1;
	}
	printf("Socket created ...\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Connection Error.\n");
		return 1;
	}
	printf("Connected\n");

	printf("\n1.Add Details\n2.Add Marks\n3.Upload Files\n4.View Details\n5.View Marks\n6.Download Files\n");
	scanf("%d", &choice);
	
	if (send(s, (char*)&choice, sizeof(choice), 0) < 0)
	{
		printf("Choice sending failed.\n");
		return 1;
	}
	printf("Choice sent.\n");
	
	printf("Enter your roll number : ");
	fflush(stdin);
	gets(rollno);
	fflush(stdin);

	if (send(s, rollno, strlen(rollno), 0) < 0)
	{
		printf("Roll Number sending failed.\n");
		return 1;
	}
	printf("Roll Number sent.\n");

	switch (choice)
	{
	case 1:Add_details(s,rollno);
		break;
	case 2:Add_marks(s);
		break;
	case 3:Attach_file(s);
		break;
	case 4:View_details(s);
		break;
	case 5:View_marks(s);
		break;
	case 6:Get_file(s);
		break;
	default: printf("Invalid Option\n");
	}

	getchar();
	closesocket(s);
	WSACleanup();
	return 0;
}