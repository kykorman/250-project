/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


int strlen22(unsigned char msg[]){
	int len=0;

	while(msg[len]!='\0')
		len++;

return len;}

void error(const char *msg)
{
    printf("%s\n",msg);
    exit(1);
}
 
int main(int argc, char *argv[])
{
	FILE *out=fopen("out","w");

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     unsigned char buffer[1473]="";
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,1473);
     n = read(newsockfd,buffer,1472);
     if (n < 0) error("ERROR reading from socket");

	for(int i=0;i<strlen22(buffer);i++){
		fprintf(out,"%c",buffer[i]);
//		printf("%c",buffer[i]);
	}
//	out<<buffer<<endl;
     close(newsockfd);
     close(sockfd);
//	system("rm out");
     return 0; 
}
