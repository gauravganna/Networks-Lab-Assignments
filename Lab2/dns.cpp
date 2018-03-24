//C Code to Query DNS for IP Address of given hostname
//Author : Gaurav Ganna
//Date : 06/03/2018

//Include the Required Header Files
#include <iostream>			//For I/O Operations
#include <string.h>			//To use strcat
#include <stdlib.h>			//To use atoi function
#include <sys/socket.h>    	//For using the socket API of Unix
#include <arpa/inet.h> 		//inet_addr , inet_ntoa , ntohs etc
#include <netinet/in.h>		
using namespace std;

//Function prototypes . More details in definition of functions. 
char * getIP(string hostname,char *dnsIP);
void convertName(unsigned char * buf,string hostname);

//Main function with one argument which is the IP Address of DNS Server.
//While running the exec give the argument
int main(int argv,char * argc[] ){

	//A Loop which will query untill Bye is given as input.
	while(1){
		string hostname ;		//Variable to store hostname
		cout << "Enter Website : ";
		cin >> hostname;		
		if(hostname == "Bye")	//If Bye is written then exit the program.
			break;
		cout << "IP of " << hostname << " is " << getIP(hostname,argc[argv-1]) << endl; 
	}
}

//Function which returns the IP address of given hostname.
//Takes two argument.
//1) Hostname 	2) DNS IP Address
char * getIP(string hostname,char * dnsIP){
	int s_fd;						//Socket file descriptor
	int len = 0;					//To maintain the length of the query message.
	unsigned char buf[1024];		//Buffer Space of 1024 bytes 
	struct sockaddr_in dest;		
	struct sockaddr_in s;			

	s_fd = socket( AF_INET , SOCK_DGRAM , 0 );			//Asking for a socket to the OS.
	dest.sin_family = AF_INET;							//AF_INET for IPv4
	dest.sin_port = htons(53);							//Port number for DNS is 53
	inet_aton(dnsIP,&dest.sin_addr);					//IP address of DNS Server


	//Start of Header Section
	buf[len++] = 10;		//Msg Id of 2 byte - Part I
	buf[len++] = 255;		//Msg Id of 2 byte - Part II	Random value is assigned.
	//QR(1 bit) - Value "0"(Is a Query) | Opcode(4 bit) - Value "0000"(Std Query) | AA(1 bit) - Value "0"(Not an Authorative Answer) | TC(1 bit) - Value "0"(not a Truncated Answer) | RD(1 bit) - Value "1"(Recursion is desired)
	buf[len++] = 1;
	//RA(1 bit) - Value "0"(Is set in response) | Z(3 bit) - Value "000"(Is reserved!!) | RCODE(4 bit) - Value "0000" (Is set in Response)
	buf[len++] = 0;			
	buf[len++] = 0;			//2 byte - PI (Value - "01" ) 
	buf[len++] = 1;			//P II - No. of Questions in Msg
	buf[len++] = 0;			//2 bytes - PI (Value - "00" Is a Query)
	buf[len++] = 0;			//P II - No. of Answers in Msg
	buf[len++] = 0;			//2 bytes - PI (Value - "00" Is a Query)
	buf[len++] = 0;			//P II - No. of Authorititative Answers
	buf[len++] = 0;			//2 bytes - PI (Value - "00" Is a Query)
	buf[len++] = 0;			//P II - No. of Additional Answer	
	//End of Header Section


	//Start of Question Section
	unsigned char * name = &buf[len];		//Pointer to the 12th index in buf	
	convertName(name,hostname);				//Covert the hostname to the dns vaild format. QName := Hostname
	len += strlen((const char *)name);		//Len = Len + length of hostname
	buf[len++] = 0;			//0 at the end of the hostname 
	buf[len++] = 0;			//PI (Value - "01") 
	buf[len++] = 1;			//P II - Type of Query - 1 denotes type A
	buf[len++] = 0;			//PI (Value - "01")
	buf[len++] = 1;			//P II - Class of Query - 1 denotes class IN for Internet
	//End of Question Section

	// Sending Packet containing the Query to the DNS Server.
	if(sendto(s_fd,(char *)buf,len,0,(struct sockaddr *)&dest,sizeof(dest)) < 0)
		return "Error Sending Info!!";		//If Error.

	unsigned int i = sizeof dest;


	//Receiving the Answer from the DNS Server.
	if(recvfrom(s_fd,(char *)buf , 1024 , 0 ,(struct sockaddr *)&dest ,&i) < 0)
		return "Error receiving Info!!";	//If Error.
	
	
	// < -- PARSING THE RESPONSE FOR THE IP OF THE HOSTNAME -- >
	int pos = 0;			//position to determine the start of RR of type A

	
	//Loop parses the RR of type A from RR of other types.
	while(1){
		//Checks whether the Response type is Type A. If yes it exits the loop 
		if( ntohs(*((unsigned short *)&buf[len + pos + 2])) == 1)
			break;
		//If response is not of type A then it goes to next RR.
		pos = pos + 12 + ntohs(*(unsigned short *)&buf[len + pos + 10]);
	}



	//Allocating 4 byte of memory to store IP( IPv4 - 32 bit)
	unsigned char * response = (unsigned char *)malloc(4);

	//It will store the IP Address in the response variable
	for(int j=0;j<4;j++)
		response[j] = buf[len + 12 + pos + j];
	response[4] = '\0';				//Ends with end of line character.

	long *p;
	p = (long *) response;
	s.sin_addr.s_addr = (*p);		//Converts the IPv4 to human readable form.

	return inet_ntoa(s.sin_addr);	//Return the IP Address .
}

//This will convert the hostname to format in which dns stores hostname.
//Takes two argument 1) Empty String which will store the changed hostname 2) Original hostname
// Will convert www.iitgoa.ac.in to 3www6iitgoa2ac2in
void convertName(unsigned char * buf,string hostname){
	int lock = 0;
	hostname = hostname + ".";
	for(int i = 0 ; i < hostname.length(); i++){
		if(hostname[i] == '.'){
			*buf++= i - lock;
			for(lock;lock<i;lock++){
				*buf++= hostname[lock];
			}
			lock++;
		}
	}
	*buf++='\0';
}