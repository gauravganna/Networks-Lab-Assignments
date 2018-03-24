//Included the required header files.

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;


//Arguments: Two Arguments of type char *
// 1) IP Address of server.
// 2) Port Number.
//Main function
int main(int argc,char * argv[]){
	int s;									// File descriptor for socket
	int bytes_sent,bytes_recieved;			// Variable to store number of bytes sent and recieved.
	struct sockaddr_in dest_addr;			//Variable that stores the information of my IP/Port address.
	
	const int DEST_PORT = atoi(argv[2]);	//Constant Variable that stores the destination port number.

	s = socket(AF_INET,SOCK_STREAM,0);		//Asking for a socket.
	dest_addr.sin_family = AF_INET;			//Assigning the values to different variables inside sockaddr struct.
	dest_addr.sin_port = htons(DEST_PORT); 
	inet_aton(argv[1],&dest_addr.sin_addr);
	memset(&(dest_addr.sin_zero),'\0',8);	

	connect(s,(struct sockaddr *)&dest_addr,sizeof(sockaddr));	//Connecting with server.


	char msg_recieved[65536] = {0};								//Variable to store the recieved msg.
	bytes_recieved = recv(s,&msg_recieved[0],10240,0);			//Recieve Msg.
	cout  << "Server Message: " << msg_recieved << endl;		

	string tmp;													// Stores the Message to be delivered to server.
	cout << "Enter message for server: ";
	getline(cin,tmp);
	bytes_sent = send(s,&tmp[0],tmp.length(),0);				//Sending Msg.

	while(tmp != "Bye"){										//Untill tmp is not "Bye" , continue communication between server and client.
		memset(&msg_recieved,0,65536);							//Assign 0 to each element of the msg_recieved array 
		bytes_recieved = recv(s,&msg_recieved[0],65536,0);		//Recieve Msg.
		cout << "Server Message: " << msg_recieved << endl;

		cout << "Enter message for server: ";					//Take Input from the terminal which will be send to server.
		getline(cin,tmp);										
		bytes_sent = send(s,&tmp[0],tmp.length(),0);		//Send Msg.

	}

	//Printing the final Message from the server to the screen and then closing the connection.
	cout << "Server Message: " << "Bye" <<endl << "Closing Connection." << endl;
	close(s);													//Connection closed 
}