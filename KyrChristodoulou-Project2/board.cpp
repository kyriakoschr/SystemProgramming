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
	pid_t serverpid ;
	DIR * path_ptr ;
	bool flag=false;
	struct dirent *nextfile;
	StrList tokens;
	char filepath[PATHSZ];
	char servermsg[SERMSGSZ];
	char bpostmsg[SERMSGSZ];
	char tempfile[SERMSGSZ];
	int fd1,fd2,fd3,fd4,fd5,res,res1;
	char buffer[NAMESZ];
	char command[SERMSGSZ];
	char pipe_client_read[NAMESZ],pipe_client_write[NAMESZ],pipe_others_read[NAMESZ],pipe_others_write[NAMESZ],path_id[NAMESZ];
	if(argc!=2){
		cout<<"Client: Wrong number of arguments"<<endl;
		exit(1);
	}
	if ((path_ptr = opendir(argv[1])) == NULL){
		cout<<"Client: Open path "<<argv[1]<<" failed: "<<strerror(errno)<<endl ;
		if(mkdir(argv[1],0755)==-1){
			cout<<"Client: Create path "<<argv[1]<<strerror(errno)<<endl;
			exit(1);
		}
		else
			cout<<"Client: Path "<<argv[1]<<" created"<<endl;
	}
	signal ( SIGINT , SIG_IGN );
	strncpy(pipe_client_read,argv[1],sizeof(pipe_client_read));
	strncat(pipe_client_read,"_client_read",sizeof("_client_read"));
	strncpy(pipe_client_write,argv[1],sizeof(pipe_client_write));
	strncat(pipe_client_write,"_client_write",sizeof("_client_write"));
	strncpy(pipe_others_read,argv[1],sizeof(pipe_others_read));
	strncat(pipe_others_read,"_others_read",sizeof("_others_read"));
	strncpy(pipe_others_write,argv[1],sizeof(pipe_others_write));
	strncat(pipe_others_write,"_others_write",sizeof("_others_write"));
	strncpy(path_id,argv[1],sizeof(path_id));
	strncat(path_id,"_pid",sizeof("_pid"));
	if ( ( fd1 = open ( pipe_client_read , O_RDWR )) < 0)  //check if the pipe to read from server exists
		if(errno==ENOENT){												//if not create it
			cout<<"Client: Client_Read fifo missing"<<endl;
			if ( mkfifo ( pipe_client_read, 0600) == -1 )
				if ( errno != EEXIST ) {
					cout<<"Client: Error in creating "<<pipe_client_read<<strerror(errno)<<endl;
					exit (6);
				}
		}
	else
		if(close(fd1)==-1){
			cout<<"Client: Error closing "<<fd1<<strerror(errno)<<endl;
			exit(1);
		}
	if ( ( fd2 = open ( pipe_client_write , O_RDWR )) < 0)  //check if the pipe to write to server exists
		if(errno==ENOENT){												//if not create it
			cout<<"Client: Client_Write fifo missing"<<endl;
			if ( mkfifo ( pipe_client_write, 0600) == -1 )
				if ( errno != EEXIST ) {
					cout<<"Client: Error in creating "<<pipe_client_write<<strerror(errno)<<endl;
					exit (6);
				}
		}
	else
		if(close(fd2)==-1){
			cout<<"Client: Error closing "<<fd2<<strerror(errno)<<endl;
			exit(1);
		}
	if ( ( fd3 = open ( pipe_others_read, O_RDWR)) < 0)  //check if the pipe for others to read from server exists
		if(errno==ENOENT){												//if not create it
			cout<<"Client: Others_Read fifo missing"<<endl;
			if ( mkfifo ( pipe_others_read, 0644) == -1 )
				if ( errno != EEXIST ) {
					cout<<"Client: Error in creating "<<pipe_others_read<<strerror(errno)<<endl;
					exit (6);
				}
		}
	else
		if(close(fd3)==-1){
			cout<<"Client: Error closing "<<fd3<<strerror(errno)<<endl;
			exit(1);
		}
	if ( ( fd4 = open ( pipe_others_write, O_RDWR)) < 0)  //check if the pipe for others to write to server exists
		if(errno==ENOENT){												//if not create it
			cout<<"Client: Others_Write fifo missing"<<endl;
			if ( mkfifo ( pipe_others_write, 0666) == -1 )
				if ( errno != EEXIST ) {
					cout<<"Client: Error in creating "<<pipe_others_write<<strerror(errno)<<endl;
					exit (6);
				}
		}
	else
		if(close(fd4)==-1){
			cout<<"Client: Error closing "<<fd4<<strerror(errno)<<endl;
			exit(1);
		}
	fd5=open(path_id,O_RDWR);	//check if the pid file exists
	if((fd5==-1)&&(errno==ENOENT)){	//if it's not, create it and turn flag true
		cout<<"Client: Missing _pid - Gonna create it."<<endl;
		if(creat(path_id,0744)==-1){
			cout<<"Client: Error creating path_id "<<strerror(errno)<<endl;
			exit(1);
		}
		flag=true;
		cout<<"Client: Created _pid"<<endl;
		fd5=open(path_id,O_RDWR);
		if(fd5==-1){
			cout<<"Client: Error opening _pid "<<strerror(errno)<<endl;
			exit(1);
		}
	}
	res=read(fd5,buffer,SERMSGSZ+1);	//read from the pid file the server pid
	if(res<0){
		cout<<"Client: Error in reading _pid "<<strerror(errno)<<endl;
		exit(1);
	}
	if(flag==true){//if server is to be created in this run
		serverpid = fork ();
		if(serverpid>0){	//save the server pid in the pid file
			snprintf(buffer,NAMESZ,"%d",serverpid);
			if(res=write(fd5,buffer,NAMESZ+1)<1){
				cout<<"Client: Error at writing to _pid "<<strerror(errno)<<endl;
				exit(1);
			}
			if(close(fd5)==-1){
				cout<<"Client: Close _pid failed "<<strerror(errno)<<endl;
				exit(1);
			}
		}
		if ( serverpid == -1) {
			cout<< "Client: Failed to fork server process "<<strerror(errno)<<endl;
			exit (1);
		}
	}
	else{	//if server was not forked now
		if(close(fd5)==-1){
				cout<<"Client: Close _pid failed "<<strerror(errno)<<endl;
				exit(1);
		}
		serverpid=atoi(buffer);	//get its pid
		if(kill(serverpid,0)==0)	//and check if is alive or ded
			cout<<"Client: Server is running "<<serverpid<<endl;
		else{
			cout<<"Client: Server is dead "<<serverpid<<endl;
			exit(1);
		}
	}
	if (serverpid == 0){//server process code
		Lista channels;
		int copies=0;
		if ( ( fd1 = open ( pipe_client_read , O_WRONLY )) < 0) { //open the pipe to write to client
			cout<<"Server: Client_Read fifo open failed "<<errno<<endl;
			exit (3) ;
		}
		if ( ( fd2 = open ( pipe_client_write ,O_NONBLOCK| O_RDONLY)) < 0) {//open the pipe to read from boardclient
			cout<<"Server: Client_Write fifo open failed "<<strerror(errno)<<endl;
			exit (3) ;
		}
		if ( ( fd3 = open ( pipe_others_read, O_WRONLY)) < 0) {//open the pipe for others to read from server
			cout<<"Server: Others_Read fifo open failed "<<endl;
			exit (3) ;
		}
		if ( ( fd4 = open ( pipe_others_write, O_RDONLY|O_NONBLOCK )) < 0) {//open the pipe to read from others
			cout<<"Server: Others_Write fifo open failed "<<endl;
			exit (3) ;
		}
		for (;;){
			if(read(fd2,servermsg,SERMSGSZ+1)>0){	//read from boardclient
				tokenize(servermsg,tokens," ");		//brake the command in tokens
				if(tokens.fetchFirst()->fetchID()=="createchannel")
					if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))==NULL){ //if the channel id received does not exist create it
						channels.addNode(tokens.fetchFirst()->fetchNext()->fetchNext()->fetchID(),stoi(tokens.fetchFirst()->fetchNext()->fetchID()));
						cout<<"Server: Channel with ID "<<tokens.fetchFirst()->fetchNext()->fetchID()<<" was created."<<endl;
					}
					else
						cout<<"Server: Channel ID already exists"<<endl;
				else														//this is the case that server received getmessages command
					if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))!=NULL){ //check if the channel exist
						if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchMsgList()->fetchCounter()>0){ //if there are messages in the channel
							for(int i=1;i<channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchMsgList()->fetchCounter()+1;i++){	//send the messages of the channel
								if(write(fd1,"msg",SERMSGSZ+1)<0){ //before each message is sent the "msg" word is sent so the client can be aware
									cout<<"Server: Writing back to client failed. "<<strerror(errno)<<endl;
									exit(1);
								}
								if(write(fd1,channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchMsgList()->fetchMsg(i)->fetchID().c_str(),SERMSGSZ+1)<0){ //now the message is sent
									cout<<"Server: Writing back to client failed. "<<strerror(errno)<<endl;
									exit(1);
								}
							}
							channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchMsgList()->emptyList(); //empty the message list
						}
						else
							cout<<"Server: No messages"<<endl;
						if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchFileList()->fetchCounter()>0){ //if there are files in the channel
							StrListNode* tempfilename=channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchFileList()->fetchFirst();
							for(int i=0;i<channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchFileList()->fetchCounter();i++){
								char temp[PATHSZ];
								strncat(temp,argv[1],sizeof(temp)); 									//creating the path of the file to be sent to the client
								strncat(temp,tempfilename->fetchID().c_str(),sizeof(temp)); //and continue here
								int tempfd=open(temp,O_RDONLY|O_NONBLOCK);		//open the file of the server to send it to the client
								char newbuffer[SERMSGSZ];
								if(write(fd1,"receive",SERMSGSZ+1)>0)	//before each file, the word "receice" is sent
									if(write(fd1,tempfilename->fetchID().c_str(),SERMSGSZ+1)>0)	//then its name
										while(read(tempfd,newbuffer,SERMSGSZ+1)>0){	//and then the file itself
											if(write(fd1,newbuffer,SERMSGSZ+1)<0){
												cout<<"Server: Sending to client failed. "<<strerror(errno)<<endl;
												exit(1);
											}
										}
								remove(temp);	//after sending is complete, the temporary file of the server is deleted
								tempfilename=tempfilename->fetchNext();
								strncpy(temp,"",sizeof(temp));
							}
							channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchFileList()->emptyList();	//and the file list of the channel is emptied
						}
						else
							cout<<"Server: No files"<<endl;
					}
					else
						cout<<"Server: Channel does not exist"<<endl;
				tokens.emptyList();
			}
			if(read(fd4,bpostmsg,SERMSGSZ+1)>0){	//reading from the boardpost
				tokenize(bpostmsg,tokens,",");		//tokenize the command
				if(tokens.fetchFirst()->fetchID()=="list"){	//if it's list
					ListNode* temp=channels.fetchFirst();
					char tempstr[10]="";//to write the channelID temporarilly
					char bpostsendmsg[SERMSGSZ+1]="\n";	//the reply of the server
					for(int i=0;i<channels.fetchCounter();i++){ //for each channel in the list add to the reply :
						strncat(bpostsendmsg,temp->fetchName().c_str(),sizeof(temp->fetchName().c_str())); //its name
						strncat(bpostsendmsg," ",sizeof(" "));
						snprintf(tempstr,10,"%d",temp->fetchID());											//its ID
						strncat(bpostsendmsg,tempstr,sizeof(tempstr));
						strncat(bpostsendmsg,"\n",sizeof("\n"));											//and a newline
						temp=temp->fetchNext();
					}
					if(write(fd3,bpostsendmsg,SERMSGSZ+1)<0){
						cout<<"Server: Writing back to boardpost failed. "<<strerror(errno)<<endl;
						exit(1);
					}
					strncpy(bpostsendmsg,"\n",sizeof(bpostsendmsg));
					strncpy(tempstr,"",sizeof(tempstr));
				}
				else if(tokens.fetchFirst()->fetchID()=="write")											//if the command is write
					if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))!=NULL){		//check if the channel exists and add it to the list
						channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchMsgList()->addNode(tokens.fetchFirst()->fetchNext()->fetchNext()->fetchID());
						cout<<"Server: Wrote message"<<endl;
					}
					else
						cout<<"Server: Channel does not exist"<<endl;
				else if (tokens.fetchFirst()->fetchID()=="send"){			//if the command is send
					if(channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))!=NULL){	//check if the channel exist
						char content[SERMSGSZ]="";
						char namepath[PATHSZ]="";
						char nametemp[PATHSZ]="";
						char name[NAMESZ]="";
						StrList temptoks;
						strncat(namepath,argv[1],PATHSZ+1);
						strncat(nametemp,argv[1],PATHSZ+1);
						tokenize(tokens.fetchMsg(3)->fetchID().c_str(),temptoks,"/"); //break the path of the incoming file to take the name
						strncat(nametemp,temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str(),sizeof(temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str()));
						int tempfd=open(nametemp,O_WRONLY); //if in the board path there is a file with the same name
						if(tempfd>0){						//add a prefix to it
							close(tempfd);
							char tempstr[10];
							snprintf(tempstr,10,"%d",copies);
							strncat(namepath,tempstr,sizeof(tempstr));
							strncat(namepath,temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str(),sizeof(temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str()));
							strncat(name,tempstr,sizeof(tempstr));
							strncat(name,temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str(),sizeof(temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str()));
							copies++;
							strncpy(tempstr,"",sizeof(tempstr));
						}
						else{								//if not let it as it is
							strncpy(namepath,nametemp,sizeof(namepath));
							strncpy(name,temptoks.fetchMsg(temptoks.fetchCounter())->fetchID().c_str(),sizeof(name));
						}
						int newfile=creat(namepath,0744); //create the file temporarilly in the board bath until the client ask for it
						if (newfile<0)
							cout<<"Server: The temporary file was not created: "<<strerror(errno)<<endl;
						else while(read(fd4,content,SERMSGSZ+1)>0){
							if(write(newfile,content,SERMSGSZ+1)<0){
								cout<<"Server: Error at receiving others' file: "<<strerror(errno)<<endl;
								close(newfile);
							}
							strncpy(content,"",sizeof(content));
							strncpy(namepath,"",sizeof(name));
						}
						close(newfile);
						cout<<"Server: Received file"<<endl;
						channels.fetchNode(stoi(tokens.fetchFirst()->fetchNext()->fetchID()))->fetchFileList()->addNode(name); //then add its name to the filelist of the appropriate channel
						strncpy(name,"",sizeof(name));
						strncpy(namepath,"",sizeof(namepath));
						strncpy(nametemp,"",sizeof(nametemp));

					}
					else
						cout<<"Server: Channel does not exist"<<endl;
				}
				tokens.emptyList();
			}
		}
	}
	else{//client process
		int ccopies=0;
		char tempstr[SERMSGSZ];
		if ( ( fd1 = open ( pipe_client_read , O_RDONLY|O_NONBLOCK )) < 0) { //open the pipe to read from server
			cout<<"Client: Client_Read fifo open failed "<<endl;
			exit (3) ;
		}
		if ( ( fd2 = open ( pipe_client_write , O_WRONLY)) < 0) {//open the pipe to write to server
			cout<<"Client: Client_Write fifo open failed "<<endl;
			exit (3) ;
		}
		cin.getline(command,SERMSGSZ);
		while(1){
			tokenize(command,tokens," "); //break the command to tokens and for each command check its components are as many as they have to be and of the right type
			if(tokens.fetchFirst()->fetchID()=="createchannel"){
				if(tokens.fetchCounter()==3)
					if(stoi(tokens.fetchFirst()->fetchNext()->fetchID())>=0){
						res=write(fd2,command,SERMSGSZ+1); //send the command  to the server
						if(res==-1){
							cout<<"Client: Error at client write "<<strerror(errno)<<endl;
							exit(1);
						}
					}
					else cout<<"Client: Wrong channel ID. Please try again"<<endl;
				else cout<<"Client: Wrong number of arguments. Please try again"<<endl;
			}
			else if(tokens.fetchFirst()->fetchID()=="getmessages"){
				int newfd;
				if(tokens.fetchCounter()==2)
					if(stoi(tokens.fetchFirst()->fetchNext()->fetchID())>=0){
						res=write(fd2,command,SERMSGSZ+1); //send the command to the server
						if(res==-1){
							cout<<"Client: Error at client write"<<endl;
							exit(1);
						}
						sleep(1);
						if(read(fd1,servermsg,SERMSGSZ+1)>0)
							while((strcmp(servermsg,"msg")==0)||(strcmp(servermsg,"receive")==0)){ // receive files and messages as long as there are sent
								if(strcmp(servermsg,"receive")==0){	//if the server sent "receive" is going to be a file
									strncpy(servermsg,"",sizeof(servermsg));
									if(read(fd1,servermsg,SERMSGSZ+1)>0){	//receive its name
										cout<<"Client: Receiving file "<<servermsg<<endl;
										if(open(servermsg,O_RDONLY)>0){		//if in the path that board was executed there is a file with the same name
											snprintf(tempstr,SERMSGSZ,"%d",ccopies);		//add a prefix to it
											strncat(tempstr,servermsg,sizeof(servermsg));
										}
										else
											strncpy(tempstr,servermsg,sizeof(tempstr));
										newfd=creat(tempstr,0777);				//and create it
										strncpy(servermsg,"",sizeof(servermsg));
										strncpy(tempstr,"",sizeof(servermsg));
									}
									while(read(fd1,servermsg,SERMSGSZ+1)>0){		//then receive its content
										if((strcmp(servermsg,"msg")==0)||(strcmp(servermsg,"receive")==0)){
											if(read(fd1,servermsg,SERMSGSZ+1)<0)
												cout<<"Client: Reading from server failed: "<<strerror(errno)<<endl;
											break;
										}
										if(write(newfd,servermsg,SERMSGSZ+1)>0)
											strncpy(servermsg,"",sizeof(servermsg));
									}
									close(newfd);
									break;
								}
								else if(strcmp(servermsg,"msg")==0){//if the server sent "msg" is going to be a message
									strncpy(servermsg,"",sizeof(servermsg));
									cout<<"Client: Printing Message "<<endl;
									if(read(fd1,servermsg,SERMSGSZ+1)>0){ //read it and print it
										cout<<servermsg<<endl;
										strncpy(servermsg,"",sizeof(servermsg));
									}
									if(read(fd1,servermsg,SERMSGSZ+1)<0)
										cout<<"Client: Reading from server failed: "<<strerror(errno)<<endl;
								}

							}
					}
					else
						cout<<"Client: Wrong channel ID. Please try again"<<endl;
				else
					cout<<"Client: Wrong number of arguments. Please try again"<<endl;
			}
			else if(tokens.fetchFirst()->fetchID()== "exit"){
				cout<<"Client: I am terminating the client process with ID : " << ( long ) getpid ()<<endl;
				if(close(fd1)==-1){
						cout<<"Client: Error closing "<<fd1<<" "<<strerror(errno)<<endl;
						exit(1);
					}
					if(close(fd2)==-1){
						cout<<"Client: Error closing "<<fd2<<" "<<strerror(errno)<<endl;
						exit(1);
					}
				exit(0);
			}
			else if (tokens.fetchFirst()->fetchID()=="shutdown"){
				cout<<"Client: I am terminating the client process with ID : " << ( long ) getpid ()<<endl;
				if(kill(serverpid,9)==0){
					cout<<"Client: I am terminating the server process with ID : " << serverpid<<endl;
					if(close(fd1)==-1){
						cout<<"Client: Error closing "<<fd1<<" "<<strerror(errno)<<endl;
						exit(1);
					}
					if(close(fd2)==-1){
						cout<<"Client: Error closing "<<fd2<<" "<<strerror(errno)<<endl;
						exit(1);
					}
					if((path_ptr=opendir(argv[1]))==NULL) //opens the board directory
						cout<<strerror(errno)<<endl;
					else
						while ( (nextfile = readdir(path_ptr)) != NULL ){ //and delete every file in it (including fifos and temporary received files that client did not ask for them
							if((strcmp(nextfile->d_name,".")==0)||(strcmp(nextfile->d_name,"..")==0))
								continue; //skip the directories .. and .
							sprintf(filepath, "%s/%s", argv[1], nextfile->d_name);
							if(remove(filepath)==0)
								cout<<"Client: Deleted "<<filepath<<endl;
							else{
								cout<<"Client: Failed to delete "<<filepath<<" "<<strerror(errno)<<endl;
								exit(1);
							}
						}
					closedir(path_ptr);
					if(rmdir(argv[1])==0) //and finally delete the path of the board
						cout<<"Client: Deleted path"<<endl;
					else{
						cout<<"Client: Failed to delete path"<<endl;
						exit(1);
					}
					exit(0);
				}
				else
					cout<<"Client: Failed to kill server"<<endl;
			}
			else
				cout<<"Client: Wrong command. Please try again"<<endl;
			tokens.emptyList();
			cin.getline(command,SERMSGSZ);
		}
	}
}
