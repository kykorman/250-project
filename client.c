#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int strlen22(unsigned char msg[]){
	int len=0;

	while(msg[len]!='\0')
		len++;

return len;}

unsigned char *
fgetus(unsigned char *dst, int max, FILE *fp)
{
	int c;
	unsigned char *p;

	/* get max bytes or upto a newline */

	for (p = dst, max--; max > 0; max--) {
		if ((c = fgetc (fp)) == EOF)
			break;
		*p++ = c;
		if (c == '\n')
			break;
	}
	*p = 0;
	if (p == dst || c == EOF)
		return NULL;
	return (p);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
 
int main(int argc, char *argv[])
{

	FILE *messageFile=fopen("tmpmsg","r");
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    unsigned char buffer[1473];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    bzero(buffer,1473);
    fgetus(buffer,1472,messageFile);
    n = write(sockfd,buffer,strlen22(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,1473);
    n = read(sockfd,buffer,1472);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
