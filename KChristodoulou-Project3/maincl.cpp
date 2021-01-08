#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */
#include <iostream>
#include <fstream>
#include "strlist.h"
#include <sstream>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#define MSGBUF 2048

using namespace std;

void tokenize(const string& str,StrList& tokens,const string& delimiters = " ") //logo xronou tin pira etoimi apo
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);			//http://www.sbin.org/doc/HOWTO/C++Programming-HOWTO-7.html
    // Find first "non-delimiter".
    string::size_type pos  = str.find_first_of(delimiters, lastPos);			//alla anti vector xrisimopoiisa ti diki mou lista

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.addNode(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

int main(int argc, char* argv[]){
	int port, sock,sleepflag=0;
    char* filename;
    char* serveraddress;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;
    if(argc!=7){
		cout<<"Wrong number of arguments"<<endl;
		return 0;
    }
    if (strcmp(argv[1],"-h")==0)
		serveraddress=argv[2];
	else if(strcmp(argv[1],"-p")==0)
		port=atoi(argv[2]);
	else if(strcmp(argv[1],"-i")==0)
		filename=argv[2];
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
	if (strcmp(argv[3],"-h")==0)
		serveraddress=argv[4];
	else if(strcmp(argv[3],"-p")==0)
		port=atoi(argv[4]);
	else if(strcmp(argv[3],"-i")==0)
		filename=argv[4];
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
    if (strcmp(argv[5],"-h")==0)
		serveraddress=argv[6];
	else if(strcmp(argv[5],"-p")==0)
		port=atoi(argv[6]);
	else if(strcmp(argv[5],"-i")==0)
		filename=argv[6];
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	perror("socket");
	cout<<serveraddress<<endl;
    if ((rem = gethostbyname(serveraddress)) == NULL) {
	   herror("gethostbyname"); exit(1);
    }
    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);
    if (connect(sock, serverptr, sizeof(server)) < 0)
	   perror("Connect");
    printf("Connecting to port %d\n", port);
    ifstream myfile(filename);
	if(!myfile.is_open()){
		cout<<"Error: Opening "<<filename<<endl;
		write(sock,"EXITING",sizeof("EXITING"));
		close(sock);
		return 0;
	}
	string command;
	while(getline(myfile,command)){
		char* bufcpy=new char[command.length()+1];
		strncpy(bufcpy,command.c_str(),command.length());
		char* save;
		char* token=strtok_r(bufcpy," ",&save);
		if(token==NULL)
			break;
		if(strcmp(token,"sleep")==0){
			sleepflag=atoi(strtok_r(NULL, " ", &save));
		}
		else if(strcmp(token,"exit")==0){
			write(sock,to_string(sizeof("EXITING")).c_str(),MSGBUF);
			write(sock,"EXITING",sizeof("EXITING"));
			close(sock);
			return 1;
		}
		else{
			string readstr;
			char readbuf[MSGBUF];
			if(sleepflag!=0){
				usleep(sleepflag);
				sleepflag=0;
			}
			write(sock,to_string(command.length()+1).c_str(),MSGBUF);
			if(write(sock,command.c_str(),command.length()+1)){
				char readsize[MSGBUF];
				if(read(sock,readsize,MSGBUF))
					if(read(sock,readbuf,atoi(readsize)))
						cout<<readbuf<<endl;
			}
			command.clear();
		}
	}
	myfile.close();
	command.clear();
	getline(cin,command);
	while(command!="exit"){
		StrList tokens;
		cout<<command<<endl;
		tokenize(command,tokens," ");
		if(tokens.fetchFirst()->fetchID()== "sleep"){
			if(tokens.fetchCounter()==2)
				if(atoi(tokens.fetchFirst()->fetchNext()->fetchID().c_str())>0){
					sleepflag=atoi(tokens.fetchFirst()->fetchNext()->fetchID().c_str());
				}
				else
					cout<<"Wrong command"<<endl;

			else
				cout<<"Wrong command"<<endl;
		}
		else{
			char readbuf[MSGBUF];
			if(sleepflag!=0){
				usleep(sleepflag);
				sleepflag=0;
			}
			write(sock,to_string(command.length()+1).c_str(),MSGBUF);
			write(sock,command.c_str(),command.length()+1);
			char readsize[MSGBUF];
			if(read(sock,readsize,MSGBUF))
				if(read(sock,readbuf,atoi(readsize)))
					cout<<readbuf<<endl;
		}
		command.clear();
		getline(cin,command);
	}
	write(sock,to_string(sizeof("EXITING")+1).c_str(),MSGBUF);
	write(sock,"EXITING",sizeof("EXITING")+1);
	close(sock);
	return 1;
}
