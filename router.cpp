//Including the required Header files
#include <iostream>
#include "network.h"
#include <vector>
using namespace std;

//Struct to store vertices
struct Node {
	bool isRouter ;						//Whether given node is router or a host machine
	string IP = "";						//IP of machine in case machine is a host machine
	unsigned short noOfInterfaces = 0; 	//No. of Interfaces in case a router
	vector <string> IPs ;				//To store IPs of interfaces if node is a router.

	//Constructor to store Routers and their interfaces information.
	Node(short noInterfaces,vector <string> ips){
		isRouter = 1;
		noOfInterfaces = noInterfaces;
		IPs = ips; 
	}

	//Constructor to store Hosts(Endpoint in a network) and it's information
	Node(string ip){
		isRouter = 0;
		IP = ip;	
	}
}

struct edge {
	unsigned int destID;		//Node IP of v in edge (u,v)
	unsigned short dist = 0;	//Distance between two vertices
	string sourceIP = "";		//Interface IP of u in (u,v)
	string destIP = "";			//Interface IP of v in (u,v)
}


int main(){

	//Preprocessing
	string s = IPaddress;				//Copy the string IPaddress so that it can be processed as IPaddress is a constant and cannot be changed.
	vector <Node> vertices ;			//Array of Nodes
	short index = -1;					//Index where the last ';' is found
	
	//Parse the Routers string.Create Nodes with given 
	while(1){
		short oldIndex = index;			//To store the old Index of last found ';'
		index = Routers.find(";",index+1);		//New Index of ';'
		
		//If no ';' is left find returns npos. If npos is encountered index is given length of Routers as value.
		if(index == string::npos)				
			index = Routers.length();

		string tmp = s.substr(oldIndex+1,index - oldIndex);		//Extract the string from the index (oldindex + 1) to index (index) 
		short indexComma = -1;									//Index where the last ',' is found
		

		//Traverse the substring to extract all the interfaces of a single router
		while(1){
			unsigned short count = 0;			//Count the no. of Interfaces
			vector <string> interfaces;			//To store the interfaces of a given Router
			short oldIndexComma = indexComma;	//To store old Index value of ','
			indexComma = tmp.find(",",indexComma+1);	//New Index value of ','
			
			//If no ',' is left find returns npos. If npos is encountered index is given length of Routers as value.
			if(indexComma == string::npos)
				indexComma = tmp.length();

			interfaces.push_back(tmp.substr(oldIndexComma+1,indexComma - oldIndexComma));	//Add IP address to vector interface of string.
			s.erase( s.find(interfaces.back()) , indexComma - oldIndexComma );				//Remove from the main string s of IP Addresses
			count++;			//Increment the count

			//If end of the string 
			if(indexComma == tmp.length()){
				Node n(count,interfaces);			//Create a new node n with the given information.
				vertices.push_back(n);				//Add n to the vector vertices.
				break;								//Exit the loop.
			}

		}

		//On reaching the end of the string exit the loop
		if(index == Routers.length())
			break;
	}

	index = -1;
	//Parse the string s(IPAddresses) to find & add remaining nodes
	while(1){
		short oldIndex = index;						//To store the old index of last found ','
		index = s.find(',',index+1);				//Find new index of ','

		if(index == string::npos)					//If no ',' is remaining. It is the condition to exit the loop but some preprocessing has to be done beforehand.
			index = s.length();

		Node n(s.substr(oldIndex+1,index - oldIndex));		//Create a node 
		vertices.push_back(n);								//Add node to the vertices vector

		//Loop exit condition
		if(index == s.length())
			break;
	}




	string source,dest;
	//Type 'Bye' to exit the program.
	while(source != "Bye"){
		cout << "Give Source IP : " ;
		cin >> source ;
		cout << "Give Destination IP : " ;
		cin >> dest ;
		cout << " The packet from " << source << " to " << dest << " goes via the following router interfaces : "  << endl;	
	}
}