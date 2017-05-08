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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;
int find_length(char input[])
{
	int count = 0;
	for(int i = 0; input[i] != '\0'; ++i)
	{
		++count;
	}
	return count;
}

// the port client will be connecting to
#define PORT 70042
// max number of bytes we can get at once
#define MAXDATASIZE 600000

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    struct hostent *he;
    struct sockaddr_in their_addr;// connector\ufffd\ufffd\ufffds address information
    if(argc < 2)    // if no command line argument supplied
    {
        fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
        exit(1);
    }
    if(argc < 3)
    {
		cout << "No files provided\n";
		fflush(stdout);
		exit(1);
	}
    // get the host info
    if((he=gethostbyname(argv[1])) == NULL)
    {
        perror("gethostbyname()");
        exit(1);
    }
    else
        printf("Client-The remote host is: %s\n", argv[1]);
     
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }
    else
    printf("Client-The socket() sockfd is OK...\n");  
    their_addr.sin_family = AF_INET;// host byte order
    printf("Server-Using %s and port %d...\n", argv[1], PORT);// short, network byte order
    fflush(stdout);
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);// zero the rest of the struct
    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        exit(1);
    }
    else
        printf("Client-The connect() is OK...\n");
        fflush(stdout);
    for(int i = 2; i < argc; ++i)
    {
		send(sockfd, argv[i], find_length(argv[i]), 0);
		char buf[MAXDATASIZE];
		read(sockfd, buf, MAXDATASIZE);
		cout << buf;
		memset(&buf, 0 ,MAXDATASIZE);
		read(sockfd, buf, MAXDATASIZE);
		cout << buf;
		memset(&buf, 0 ,MAXDATASIZE);
	}
    printf("Client-Closing sockfd\n");
    close(sockfd);
    return 0;
}
