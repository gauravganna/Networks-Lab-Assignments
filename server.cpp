//Required Header files
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;

//Function to convert lowercase characters to uppercase characters.
string upper(char s[]){
	string b = "";
	for(int i=0;i<strlen(s);i++){
		if(short(s[i]) >= 97 && short(s[i]) <= 122)
			b += char(short(s[i]) - 32);
		else
			b += s[i];
	}
	return b;
}

//Arguments: One Arguments of type char *
//Port Number of Server through which all communications will take place.
//Main Function
int main(int argc,char * argv[]){
	int s_fd,new_fd;							//Variable that will keep the socket file descriptor.

	struct sockaddr_in my_addr,sender_addr;
	const int PORT = atoi(argv[argc-1]); 		//A constant variable that will store the PORT number through which the messages will be 											recieved /sent by the server.
	const int BACKLOG = 10;						//No of Connections that can be queued.

	s_fd = socket(AF_INET,SOCK_STREAM,0);		//Asking for a socket.
	my_addr.sin_family = AF_INET;				//Assigning the values to different variables inside sockaddr struct.
	my_addr.sin_port = htons(PORT);
	inet_aton("127.0.0.1",&my_addr.sin_addr);
	memset(&(my_addr.sin_zero),'\0',8);

	bind(s_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr));			// Bind socket with the port.
	listen(s_fd,BACKLOG);													//Listening For an incoming connection request.

	unsigned int sin_size = sizeof(struct sockaddr);
	new_fd = accept(s_fd,(struct sockaddr *)&sender_addr,&sin_size);		//Accepting the incoming connection request .
	cout << "Connected to client socket number " << new_fd <<endl; 			//Show Msg that we are connected to client now.
	close(s_fd);															//Closing the old socket as new is created through which data transfer will take place securely.


	int bytes_sent,bytes_read;
	//Transfer of Messages Start from here.
	char * msg_toBeSent = "Good morning! Welcome to Gaurav's Server." ;		//Welcome Message to be sent.
	bytes_sent = send(new_fd,msg_toBeSent,strlen(msg_toBeSent),0);



	char msg_recieved[10240] = {0};											// Variable which will store the incoming messages.
	bytes_read = recv(new_fd,&msg_recieved[0],10240,0);						//Read Msg from the client .
	cout << "Message from " << new_fd << ": " << msg_recieved<<endl;		//Print the msg on the screen.

	while(string(msg_recieved) != "Bye"){									//If Msg is not Bye then
		string tmp = upper(msg_recieved);									//Convert it to uppercase and return the same
		bytes_sent = send(new_fd,&tmp[0],tmp.length(),0);					//Msg Sent back with all letters uppercase.
		cout << "Replied to " << new_fd << ": " << tmp << endl;

		memset(&msg_recieved,0,10240);										//Making the msg_recieved to be identically 0
		bytes_read = recv(new_fd,&msg_recieved[0],10240,0);					//Read Msg from the client .
		cout << "Message from " << new_fd << ": " << msg_recieved<<endl;	//Print the msg on the screen.
	}

	//Replying with Bye and closing connection .
	cout << "Replied to " << new_fd << ": " << "Bye" << endl;
	close(new_fd);

}