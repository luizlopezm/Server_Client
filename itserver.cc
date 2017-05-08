// Course: Software Construction 100 <Winter 2014>
//
// First Name:Luiz
// Last Name:Lopez Nunez
// CS Login:llope039
// UCR email:llope039@ucr.edu
// SID:861060042
//
// Lecture Section: <001>
// Lab Section: <021>
// TA: Bryan Marsh
//
// Assignment <HW 8>
//
// I hereby certify that the code in this file 
// is ENTIRELY my own original work.
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
 // the port client will be connecting to
#define PORT 70042
#define MYPORT PORT
#define MAXDATASIZE 300000

/* how many pending connections queue will hold */
#define BACKLOG 10
using namespace std;

void ls_command(int sockfd,char buf[] )
{
	int org;
	char *cd[4];
	cd[0] = "ls";
	cd[1] = "-l";
	cd[2] = buf;
	cd[3] = NULL;
	pid_t pid;
	int status = 0;
	switch(pid = fork())
	{
		case -1:perror("fork failed"), exit(1);
		case 0:
		{
			dup2(sockfd , 1);
			if((execvp(cd[0], cd)) == -1);
			exit(1);
		}
		default:
		{
			int pd = wait(&status);
			break;
		}
	}
	
}


int main()
{
    /* listen on sock_fd, new connection on new_fd */
    int sockfd, new_fd;
    /* my address information, address where I run this program */
    struct sockaddr_in my_addr;
    /* remote address information */
    struct sockaddr_in their_addr;
    int sin_size;
     
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
      perror("socket() error lol!");
      exit(1);
    }
    else
      printf("socket() is OK...\n");
    /* host byte order */
    my_addr.sin_family = AF_INET;
    /* short, network byte order */
    my_addr.sin_port = htons(MYPORT);
    /* auto-fill with my IP */
    my_addr.sin_addr.s_addr = INADDR_ANY;
    /* zero the rest of the struct */
    memset(&(my_addr.sin_zero), 0, 8);
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
      perror("bind() error lol!");
      exit(1);
    }
    else
      printf("bind() is OK...\n");
    if(listen(sockfd, BACKLOG) == -1)
    {
      perror("listen() error lol!");
      exit(1);
    }
    else
      printf("listen() is OK...\n");
    /* ...other codes to read the received data... */
    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size);
    if(new_fd == -1)
      perror("accept() error lol!");
    else
      printf("accept() is OK...\n");
    int numbytes;
	char buf[MAXDATASIZE];
    while((numbytes = read(new_fd, buf, MAXDATASIZE-1)) != 0)
    {
		cout << "Data Recieved..." << endl;
		buf[numbytes] = '\0';
		cout << buf;
		cout << endl;
		ls_command(new_fd, buf);
		sleep(1);
		memset(&buf, 0 ,MAXDATASIZE);
	}
    close(new_fd);
    close(sockfd);
    return 0;
}
