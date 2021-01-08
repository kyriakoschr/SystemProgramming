#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "list.h"
#define SERMSGSZ 4096
#define PATHSZ 100
#define NAMESZ 200

using namespace std;

int main(int argc, char* argv[]){
    DIR * path_ptr ;
   	pid_t serverpid ;
	bool flag=false;
	StrList tokens;
	char servermsg[SERMSGSZ];
	char serverreadmsg[SERMSGSZ];
	int fd1,fd2,fd3,res,res1;
	char command[SERMSGSZ];
    char buffer[SERMSGSZ];
	char pipe_others_read[NAMESZ],pipe_others_write[NAMESZ],path_id[NAMESZ];
    if(argc!=2){
		cout<<"BoardPost: Wrong number of arguments"<<endl;
		exit(1);
	}
	if ((path_ptr = opendir(argv[1])) == NULL){
		cout<<"BoardPost: Open path "<<argv[1]<<" failed: "<<strerror(errno)<<endl ;
		exit(1);
	}
	signal ( SIGINT , SIG_IGN );
    strncpy(pipe_others_read,argv[1],sizeof(pipe_others_read));
	strncat(pipe_others_read,"_others_read",sizeof("_others_read"));
	strncpy(pipe_others_write,argv[1],sizeof(pipe_others_write));
	strncat(pipe_others_write,"_others_write",sizeof("_others_write"));
    strncpy(path_id,argv[1],sizeof(path_id));
	strncat(path_id,"_pid",sizeof("_pid"));
	if ( ( fd1 = open ( pipe_others_read , O_RDONLY |O_NONBLOCK)) < 0){  //check if the pipe to read from server exists
		cout<<"BoardPost: Others_Read fifo failed to open "<<strerror(errno)<<endl;
		exit (1);
    }
    if ( ( fd2 = open ( pipe_others_write , O_WRONLY)) < 0){  //check if the pipe to read from server exists
		cout<<"BoardPost: Others_Write fifo failed to open "<<strerror(errno)<<endl;
		exit (1);
    }
    fd3=open(path_id,O_RDONLY);							//openinig the pid file and read the server pid
	if((fd3==-1)&&(errno==ENOENT)){
		cout<<"BoardPost: Missing _pid - Gonna exit."<<endl;
		exit(1);
	}
	res=read(fd3,buffer,SERMSGSZ+1);
	if(res<0){
		cout<<"BoardPost: Error in reading _pid "<<strerror(errno)<<endl;
		exit(1);
	}
	serverpid=atoi(buffer);								//check if the server is alive
	if(kill(serverpid,0)==0)
		cout<<"BoardPost: Server is running "<<serverpid<<endl;
	else{
		cout<<"BoardPost: Server is dead "<<serverpid<<endl;
		exit(1);
	}
    cin.getline(command,SERMSGSZ+1);
    while(1){
        tokenize(command,tokens,",");
        if (tokens.fetchCounter()==1)						//break command to tokens and for each case check its components
            if (tokens.fetchFirst()->fetchID()=="list"){
                if(write(fd2,command,SERMSGSZ+1)<0){
					cout<<"BoardPost: Error at others write: "<<strerror(errno)<<endl;
					exit(1);
                }
                sleep(1);
                if(read(fd1,serverreadmsg,SERMSGSZ+1)>0)		//get the response from the server containing the list of channels exist
					cout<<"BoardPost: "<<serverreadmsg;
				else
					cout<<"BoardPost: No feedback from server"<<endl;
            }
            else if(tokens.fetchFirst()->fetchID()=="exit"){
                if(close(fd1)==-1){								//closing the fifos and exit
                    cout<<"BoardPost: Error closing "<<fd1<<" "<<strerror(errno)<<endl;
					exit(1);
                }
                if(close(fd2)==-1){
					cout<<"BoardPost: Error closing "<<fd2<<" "<<strerror(errno)<<endl;
                    exit(1);
					}
                exit(0);
                }
            else
                cout<<"BoardPost: Wrong command. Please try again."<<endl;
        else if (tokens.fetchCounter()==3)
            if(tokens.fetchFirst()->fetchID()=="write"){		//send the command to the server so it can add the message to the channel
				if(write(fd2,command,SERMSGSZ+1)<0){
					cout<<"BoardPost: Error at others write: "<<strerror(errno)<<endl;
					exit(1);
                }
            }
            else if (tokens.fetchFirst()->fetchID()=="send"){
				if(write(fd2,command,SERMSGSZ+1)<0){			//send the command to the server
					cout<<"BoardPost: Error at others send: "<<strerror(errno)<<endl;
					exit(1);
                }
                int tempfd=open(tokens.fetchFirst()->fetchNext()->fetchNext()->fetchID().c_str(),O_RDONLY);	//open the file that is going to be sent
                char* tempbuffer[SERMSGSZ];
                while(read(tempfd,tempbuffer,SERMSGSZ+1)>0){	//reading the file
					if(write(fd2,tempbuffer,SERMSGSZ+1)<0){		//sending the file through the fifo
						cout<<"BoardPost: Error at others sending file: "<<strerror(errno)<<endl;
						close(tempfd);
						exit(1);
					}
                }
				close(tempfd);
            }
            else
                cout<<"BoardPost: Wrong command. Please try again."<<endl;
        else
            cout<<"BoardPost: Wrong command. Please try again."<<endl;
        tokens.emptyList();
        cin.getline(command,SERMSGSZ+1);
    }
}
