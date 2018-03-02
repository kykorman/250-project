#include <iostream>
#include <string>
#include <random>
#include <fstream>
using namespace std;


int validInput(char word[30]);
int rotorNotUnique(int rotor[255],int spot);
void print_rotor(int rotor[255]);
int findRotorSpot(int val,int rotor[255]);
int main(){

	int seed,r1Pos,r2Pos,r3Pos,r1[255],r2[255],r3[255];
	char thing0[30]="Enter seed value",thing1[30]="Enter Rotor 1 position",thing2[30]="Enter Rotor 2 position",thing3[30]="Enter Rotor 3 position";
	ofstream rotor1,rotor2,rotor3;

	rotor1.open("rotor1");
	rotor2.open("rotor2");
	rotor3.open("rotor3");
	seed=validInput(thing0);
	r1Pos=validInput(thing1);
	r2Pos=validInput(thing2);
	r3Pos=validInput(thing3);

    mt19937 generator(seed);
    uniform_int_distribution<int> gen_rotor(1,255);
	for(int i=0;i<1000;i++)//use the generator a bit so that the numbers start to deviate more, this is an issue with mersenne's twister algorithm that I saw.	
		gen_rotor(generator);


	for(int i=0;i<255;i++)
		do{
			r1[i]=gen_rotor(generator);
		}while(rotorNotUnique(r1,i));
	for(int i=0;i<255;i++)
		do{
			r2[i]=gen_rotor(generator);
		}while(rotorNotUnique(r2,i));
	for(int i=0;i<255;i++)
		do{
			r3[i]=gen_rotor(generator);
		}while(rotorNotUnique(r3,i));
	for(int i=0;i<255;i++){
		rotor1<<i<<": "<<r1[i]<<endl;
		rotor2<<i<<": "<<r2[i]<<endl;
		rotor3<<i<<": "<<r3[i]<<endl;
	
	}
/*
	print_rotor(r1);
	cout<<endl<<endl;
	print_rotor(r2);
	cout<<endl<<endl;
	print_rotor(r3);
	cout<<endl<<endl;
*/
	string msg;

	cout<<"Enter message (under 1472 characters): ";
	string jnk;
	getline(cin,jnk);
	getline(cin,msg);
	unsigned char msgEncrypted[msg.length()+1];
	msgEncrypted[msg.length()]='\0';
	int r1bak=r1Pos,r2bak=r2Pos,r3bak=r3Pos;
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
		res0=r1[(r1Pos+msg[i])%255];
		res1=r2[(res0+r2Pos)%255];
		res2=r3[(res1+r3Pos)%255];

		cout<<res0<<" "<<res1<<" "<<res2<<endl;
//		msgEncrypted[i]=res2;
		r1Pos++;
//		cout<<(int)msgEncrypted[i]<<endl;
//	cout<<tring)msgEncrypted<<endl;
	}
	unsigned char msgUnencrypted[msg.length()+1];
	msgUnencrypted[msg.length()]='\0';

	r1Pos=r1bak;
	r2Pos=r2bak;
	r3Pos=r3bak;
//	cout<<r1Pos<<" "<<r2Pos<<" "<<r3Pos<<endl;
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
		msgUnencrypted[i]=(findRotorSpot((findRotorSpot((findRotorSpot(msgEncrypted[i],r3)-r3Pos+255)%255,r2)-r2Pos+255)%255,r1)-r1Pos+255)%255;
		r1Pos++;
			
	}
	for(int i=0;i<msg.length();i++)
		cout<<msgUnencrypted[i];
	cout<<endl;
//cout<<(string)msgUnencrypted<<endl;
return 0;}


int validInput(char word[30]){
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
	cout<<val<<" Failed\n";
return -1;}

