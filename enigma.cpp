#include <cstdio>
#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <cstring>
using namespace std;
unsigned char *fgetus(unsigned char *dst, int max, FILE *fp);
//void send(unsigned char msg[]);
//void receive(unsigned char encrypted[]);
void sendMain(int r1[],int r2[],int r3[],int r1Pos,int r2Pos,int r3Pos);
void receiveMain(int r1[],int r2[],int r3[],int r1Pos,int r2Pos,int r3Pos);

void encode(int r1[],int r2[], int r3[], int r1Pos,int r2Pos, int r3Pos, string msg, unsigned char msgEncrypted[]);
void decode(int r1[],int r2[], int r3[], int r1Pos,int r2Pos, int r3Pos, unsigned char msgEncrypted[], unsigned char msgUnencrypted[]);

int strlen(unsigned char msg[]);

int validInput(char word[30]);
int rotorNotUnique(int rotor[255],int spot);
void print_rotor(int rotor[255]);
int findRotorSpot(int val,int rotor[255]);
int mod(int num);

int main(){

	int seed,r1Pos,r2Pos,r3Pos,r1[255],r2[255],r3[255];
	char thing0[30]="Enter seed value",thing1[30]="Enter Rotor 1 position",thing2[30]="Enter Rotor 2 position",thing3[30]="Enter Rotor 3 position";

	seed=validInput(thing0);
	r1Pos=validInput(thing1);
	r2Pos=validInput(thing2);
	r3Pos=validInput(thing3);

    mt19937 generator(seed);
    uniform_int_distribution<int> gen_rotor(1,255);
	for(int i=0;i<1000;i++)//use the generator a bit so that the numbers start to deviate more, this is an issue with mersenne's twister algorithm that I saw.	
		gen_rotor(generator);

//Generate rotors randomly, ensure no duplicates on the same rotor.
	for(int i=0;i<255;i++)
		do
			r1[i]=gen_rotor(generator);
		while(rotorNotUnique(r1,i));
	for(int i=0;i<255;i++)
		do
			r2[i]=gen_rotor(generator);
		while(rotorNotUnique(r2,i));
	for(int i=0;i<255;i++)
		do
			r3[i]=gen_rotor(generator);
		while(rotorNotUnique(r3,i));
/*
	print_rotor(r1);
	cout<<endl<<endl;
	print_rotor(r2);
	cout<<endl<<endl;
	print_rotor(r3);
	cout<<endl<<endl;
*/

	char thing4[50]="0 to decode, 1 or higher to encode: ";
	int encodeOrNah=validInput(thing4);

	system("clear");
	if(encodeOrNah){
		system("gcc client.c -o c.out");
		sendMain(r1,r2,r3,r1Pos,r2Pos,r3Pos);

/*
*/
	}else{

		system("gcc server.c -o b.out");
		receiveMain(r1,r2,r3,r1Pos,r2Pos,r3Pos);
/*
		unsigned char msgUnencrypted[strlen(msgEncrypted)+1];
		decode(r1,r2,r3,r1Pos,r2Pos,r3Pos,msgEncrypted,msgUnencrypted);
*/
	}
//cout<<(string)msgUnencrypted<<endl;
return 0;}
void decode(int r1[],int r2[], int r3[], int r1Pos,int r2Pos, int r3Pos, unsigned char msgEncrypted[], unsigned char msgUnencrypted[]){
	cout<<"Encrypted message\n";
	for(int k=0;k<strlen(msgEncrypted);k++)
		printf("%c",msgEncrypted[k]);
	cout<<endl;
//	cout<<r1Pos<<" "<<r2Pos<<" "<<r3Pos<<endl;
	for(int i=0;i<strlen(msgEncrypted);i++){
		if(r1Pos>=255){
			r1Pos=0;
			r2Pos++;
			if(r2Pos>=255){
				r2Pos=0;
				r3Pos++;
				if(r3Pos>=255)
					r3Pos=0;
			}
		}
		msgUnencrypted[i]=mod(findRotorSpot(mod(findRotorSpot(mod(findRotorSpot(msgEncrypted[i],r3)-r3Pos),r2)-r2Pos),r1)-r1Pos);
		r1Pos++;
			
	}
}
void encode(int r1[],int r2[], int r3[], int r1Pos,int r2Pos, int r3Pos,string msg, unsigned char msgEncrypted[]){

	msgEncrypted[msg.length()]='\0';
	for(int i=0;i<msg.length();i++){
		if(r1Pos>=255){
			r1Pos=0;
			r2Pos++;
			if(r2Pos>=255){
				r2Pos=0;
				r3Pos++;
				if(r3Pos>=255)
					r3Pos=0;
			}	
		}
	
	unsigned short res0,res1,res2;
		msgEncrypted[i]=r3[(r3Pos+r2[(r2Pos+r1[(r1Pos+msg[i])%255])%255])%255];
	/*	
		This is the non-one line way of writing it.
		
		res0=r1[(r1Pos+msg[i])%255];
		res1=r2[(res0+r2Pos)%255];
		res2=r3[(res1+r3Pos)%255];
	*/
//		msgEncrypted[i]=res2;
		r1Pos++;
//		cout<<(int)msgEncrypted[i]<<endl;
	}


} 

int validInput(char word[50]){
	int val;
	cout<<word<<endl;
	cin>>val;

	while(val<0){
		cout<<"Enter valid value > 0\n";
		cout<<word<<endl;
		cin>>val;
	}

return val;}


int rotorNotUnique(int rotor[255],int spot){
	for(int i=0;i<spot;i++)
		if(rotor[i]==rotor[spot])
			return 1;

return 0;}
void print_rotor(int rotor[255]){
	for(int i=0;i<254;i++)
		cout<<rotor[i]<<" ";
	cout<<endl;
}
int findRotorSpot(int val,int rotor[255]){
	for(int i=0;i<255;i++){
		if(rotor[i]==val)
			return i;
	}
return -1;}
int mod(int num){
	if(num<=0)
		return num+255;
return num;}


int strlen(unsigned char msg[]){
	int len=0;

	while(msg[len]!='\0')
		len++;

return len;}

void sendMain(int r1[],int r2[],int r3[],int r1Pos,int r2Pos,int r3Pos){

	int portno;

	ofstream msgFile;
	msgFile.open("tmpmsg");
	string msg,jnk;

	cout<<"Enter message to encrypt, under 1472 characters: ";

	getline(cin,jnk);//Consume all the garbage
	getline(cin,msg);
	unsigned char msgEncrypted[msg.length()+1];

	cout<<"Enter port number to send on (1-65535): ";
	cin>>portno;
	cout<<"Enter host to send to: ";

	char jnk2;
	scanf("%c",&jnk2);
	char serveraddr[100]="";
	cin>>serveraddr;

	encode(r1,r2,r3,r1Pos,r2Pos,r3Pos,msg,msgEncrypted);
	msgFile<<msgEncrypted;
	msgFile.close();
	string cmd="./c.out ";

	cmd+=serveraddr;
	cmd+=" ";
	cmd+=to_string(portno);
	cmd+='\n';
	
	char newcmd[cmd.length()+1];
	strcpy(newcmd,cmd.c_str());

	system(newcmd);
	cout<<"Sent\n";
	system("rm c.out;rm tmpmsg");
}

void receiveMain(int r1[],int r2[],int r3[],int r1Pos,int r2Pos,int r3Pos){
	unsigned char encrypted[1473]="",unencrypted[1473]="";
	int portno;
	
	
	cout<<"Enter port number to listen on (1-65535): ";
	cin>>portno;

	string cmd="./b.out ";
	cmd+=to_string(portno);
	cmd+='\n';
	char newcmd[cmd.length()+1];
	strcpy(newcmd,cmd.c_str());

	system(newcmd);

	FILE *encMsg=fopen("out","r");
	fgetus(encrypted,1472,encMsg);	
/*	int len=0;
	char tmp;
//	fscanf(encMsg,"%hhu",&tmp);

	while(fscanf(encMsg,"%hhu",&tmp)!=EOF){
		unencrypted[len++]=tmp;
		cout<<len<< " "<<unencrypted[len]<<endl;

//		fscanf(encMsg,"%hhu",&tmp);
	}
*/
	decode(r1,r2,r3,r1Pos,r2Pos,r3Pos,encrypted,unencrypted);
	cout<<"\n\n                        Unencrypted message\n\n";
	for(int i=0;i<strlen(unencrypted);i++)
		printf("%c",unencrypted[i]);
	cout<<endl;	
	cout<<"1 to receive again, 0 to exit: ";
	int tmp2;
	cin>>tmp2;

	if(tmp2==1)
		receiveMain(r1,r2,r3,r1Pos,r2Pos,r3Pos);
	else{
		system("rm b.out");
	//	system("rm out");
		exit(0);
	}
}
/*
void send(unsigned char msg[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char serveraddr[20]="";
	cout<<portno<<endl;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	cout<<serveraddr<<endl;
	server = gethostbyname(serveraddr);
	cout<<server<<endl;;
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
	n = write(sockfd,msg,strlen(msg));
	close(sockfd);
}


void receive(unsigned char encrypted[]){
	int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	bzero((char *) &serv_addr, sizeof(serv_addr));

    char thing[50]="Enter port number to listen on (1-65535): ";
	portno=validInput(thing);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
	listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
		(struct sockaddr *) &cli_addr, 
		&clilen);
	bzero(encrypted,1473);
	n = read(newsockfd,encrypted,1472);
    printf("Encrypted message: %s\n",encrypted);

    close(newsockfd);
    close(sockfd);
}
*/
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
//		if (c == '\n')
//			break;
	}
	*p = 0;
	if (p == dst || c == EOF)
		return NULL;
	return (p);
}

