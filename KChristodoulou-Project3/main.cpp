#include <iostream>
#include <fstream>
#include "hasht.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MSGBUF 2048

using namespace std;

typedef struct {
	int q_size;
	int* data;
	int start ;
	int end ;
	int counter ;
} pool_t ;

int port;
HashTable myhash(hashtsize);
int worker_threads;
pthread_mutex_t mtx ;
pthread_mutex_t wrmtx ;
pthread_cond_t cond_nonempty ;
pthread_cond_t cond_nonfull ;
pool_t pool ;
pthread_cond_t read_cond , write_cond ;
int readers = 0;
bool writer = false ;

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

void initialize(pool_t * pool) {
	pool->start = 0;
	pool->end = -1;
	pool->counter = 0;
	pool->data=new int[pool->q_size];
}

void place(pool_t * pool, int data) {
	pthread_mutex_lock(&mtx);
	while (pool->counter >= pool->q_size) {
		printf(">> Found Buffer Full \n");
		pthread_cond_wait(&cond_nonfull, &mtx);
	}
	pthread_mutex_lock(&wrmtx);
	while(writer||readers>0)
		pthread_cond_wait(&write_cond, &wrmtx);
	writer=true;
	pthread_mutex_unlock(&wrmtx);
	pool->end = (pool->end + 1) % pool->q_size;
	pool->data[pool->end] = data;
	pool->counter++;
	pthread_mutex_lock(&wrmtx);
	writer=false;
	pthread_cond_broadcast(&read_cond);
	pthread_cond_signal(&write_cond);
	pthread_mutex_unlock(&wrmtx);
	pthread_cond_signal(&cond_nonempty);
	pthread_mutex_unlock(&mtx);
}

int obtain(pool_t * pool) {
	int data = 0;
	pthread_mutex_lock(&mtx);
	while (pool->counter <= 0) {
		printf(">> Found Buffer Empty \n");
		pthread_cond_wait(&cond_nonempty, &mtx);
	}
	pthread_mutex_lock(&wrmtx);
	while(writer)
		pthread_cond_wait(&read_cond, &wrmtx);
	readers++;
	pthread_mutex_unlock(&wrmtx);
	data = pool->data[pool->start];
	pool->start = (pool->start + 1) % pool->q_size;
	pool->counter--;
	pthread_mutex_lock(&wrmtx);
	readers--;
	if(readers==0)
		pthread_cond_broadcast(&write_cond);
	pthread_mutex_unlock(&wrmtx);
	pthread_cond_signal(&cond_nonfull);
	pthread_mutex_unlock(&mtx);

	return data;
}

void* producer(void* ptr){
	int	sock, newsock;
    struct sockaddr_in server, client;
    socklen_t clientlen;
    struct sockaddr *serverptr=(struct sockaddr*)&server;
    struct sockaddr *clientptr=(struct sockaddr*)&client;
    struct hostent *rem;
    if ((sock = socket(PF_INET,SOCK_STREAM,0)) < 0){
		perror("Socket: ");
		exit(EXIT_FAILURE);
	}
	server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    if (bind(sock, serverptr, sizeof(server)) < 0){
        perror("Bind: ");
        exit(EXIT_FAILURE);
	}
    if (listen(sock, 5) < 0){
		perror("Listen: ");
		exit(EXIT_FAILURE);
	}
    while(1){
    	if ((newsock = accept(sock, clientptr, &clientlen)) < 0){
			perror("Accept: ");
			continue;
		}
    	printf("Accepted connection\n");
    	place(&pool, newsock);
		printf("Master thread: %d\n", newsock);
		pthread_cond_signal(&cond_nonempty);
    }
}

void* consumer(void* ptr){
	while(1){
		int sock=obtain(&pool);
		printf("Worker Thread %ld obatined: %d\n",pthread_self(), sock);
		pthread_cond_signal(&cond_nonfull);
		do{
			char readbuf[MSGBUF];
			string result;
			string command;
			char readsize[MSGBUF];
			if(read(sock,readsize,MSGBUF))
				if(read(sock,readbuf,atoi(readsize))){
					command.clear();
					result.clear();
					command.append(readbuf);
				}
			if(command=="EXITING")
				break;
			cout<<command<<endl;
			StrList* tokens=new StrList;
			tokenize(command,*tokens," ");
			if(tokens->fetchFirst()->fetchID()=="add_account"){
				if(tokens->fetchCounter()==3){
					double amount=strtod(tokens->fetchFirst()->fetchNext()->fetchID().c_str(),NULL);
					if(amount){
						if(myhash.createnondes(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID(),amount)==1){
							result="Success. Account creation (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(")");
						}
						else{
							result="Error. Account creation failed (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(")");
						}
					}
					else{
						result="Error. Account creation failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(")");
					}
				}
				else if(tokens->fetchCounter()==4){
					int delay=strtol(tokens->fetchLastID().c_str(),NULL,10);
					double amount=strtod(tokens->fetchFirst()->fetchNext()->fetchID().c_str(),NULL);
					if(amount&&delay){
						if(myhash.createnondes(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID(),amount,delay)==1){
							result="Success. Account creation (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchLastID());
							result.append(")");
						}
						else{
							result="Error. Account creation failed (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(":");
							result.append(tokens->fetchLastID());
							result.append(")");
						}
					}
					else{
						result="Error. Account creation failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(":");
						result.append(tokens->fetchLastID());
						result.append(")");
					}
				}
				else
					result="Error. Add Account arguments are wrong.";
			}
			else if(tokens->fetchFirst()->fetchID()=="add_transfer"){
				if(tokens->fetchCounter()==4){
					double amount=strtod(tokens->fetchFirst()->fetchNext()->fetchID().c_str(),NULL);
					string receiver=tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext()->fetchID();
					if(myhash.addtran(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID(),receiver,amount)==1){
						result="Success. Transfer addition (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(receiver);
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(")");
					}
					else{
						result="Error. Transfer addition failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(receiver);
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(")");
					}
				}
				else if(tokens->fetchCounter()==5){
					int delay=strtol(tokens->fetchLastID().c_str(),NULL,10);
					double amount=strtod(tokens->fetchFirst()->fetchNext()->fetchID().c_str(),NULL);
					string receiver=tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext()->fetchID();
					if(myhash.addtran(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID(),receiver,amount,delay)==1){
						result="Success. Transfer addition (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(receiver);
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext()->fetchNext()->fetchID());
						result.append(")");
					}
					else{
						result="Error. Transfer addition failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(receiver);
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(":");
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext()->fetchNext()->fetchID());
						result.append(")");
					}
				}
				else
					result="Error. Add Transfer arguments are wrong.";
			}
			else if(tokens->fetchFirst()->fetchID()=="add_multi_transfer"){
				string sender=tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID();
				double amount=strtod(tokens->fetchFirst()->fetchNext()->fetchID().c_str(),NULL);
				if(!amount)
					result="Error. Add Mutlti-Transfer arguments are wrong.";
				string laststring=tokens->fetchLastID();
				int delay=strtol(laststring.c_str(),NULL,10);
				if(delay){
					StrList* temprecs=new StrList;
					StrListNode* temp=tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext();
					for(int i=0;i<tokens->fetchCounter()-4;i++){
						temprecs->addNode(temp->fetchID());
						temp=temp->fetchNext();
					}
					if(myhash.addmultitran(sender,*temprecs,amount,delay)==1){
						result="Success. Multi-Transfer addition (";
						result.append(sender);
						result.append(":");
						result.append(to_string(amount));
						result.append(":");
						result.append(laststring);
						result.append(")");
					}
					else{
						result="Error. Multi-Transfer addition failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(to_string(amount));
						result.append(":");
						result.append(laststring);
						result.append(")");
					}
				}
				else{
					StrList* temprecs=new StrList;
					StrListNode* temp=tokens->fetchFirst()->fetchNext()->fetchNext()->fetchNext();
					for(int i=0;i<tokens->fetchCounter()-3;i++){
						temprecs->addNode(temp->fetchID());
						temp=temp->fetchNext();
					}
					if(myhash.addmultitran(sender,*temprecs,amount)==1){
						result="Success. Multi-Transfer addition (";
						result.append(sender);
						result.append(":");
						result.append(to_string(amount));
						result.append(")");
					}
					else{
						result="Error. Multi-Transfer addition failed (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchNext()->fetchID());
						result.append(":");
						result.append(to_string(amount));
						result.append(")");
					}
				}
			}
			else if(tokens->fetchFirst()->fetchID()=="print_balance"){
				if(tokens->fetchCounter()==2){
					double resultd=myhash.print_balance(tokens->fetchFirst()->fetchNext()->fetchID());
					if(resultd >-1){
						result="Success. Balance (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(":");
						result.append(to_string(resultd));
						result.append(")");
					}
					else{
						result="Error. Balance (";
						result.append(tokens->fetchFirst()->fetchNext()->fetchID());
						result.append(")");
					}
				}
				else if(tokens->fetchCounter()==3){
					string laststring=tokens->fetchLastID();
					int delay=strtol(laststring.c_str(),NULL,10);
					double resultd=myhash.print_balance(tokens->fetchFirst()->fetchNext()->fetchID());
					if(delay){
						if(resultd >-1){
							result="Success. Balance (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(":");
							result.append(to_string(resultd));
							result.append(":");
							result.append(to_string(delay));
							result.append(")");
						}
						else{
							result="Error. Balance (";
							result.append(tokens->fetchFirst()->fetchNext()->fetchID());
							result.append(laststring);
							result.append(")");
						}
					}
					else
						result="Error. Print_Balance arguments are wrong.";
				}
				else
					result="Error. Print_Balance arguments are wrong.";
			}
			else if(tokens->fetchFirst()->fetchID()=="print_multi_balance"){
				string *table;
				string delaystr=tokens->fetchLastID();
				int delay=strtol(delaystr.c_str(),NULL,10);
				StrList* temprecs=new StrList;
				StrListNode* temp=tokens->fetchFirst()->fetchNext();
				if(delay){
					table=new string[tokens->fetchCounter()-2];
					for(int i=0;i<tokens->fetchCounter()-2;i++){
						temprecs->addNode(temp->fetchID());
						table[i]=temp->fetchID();
						temp=temp->fetchNext();
					}
				}
				else{
					table=new string[tokens->fetchCounter()-1];
					for(int i=0;i<tokens->fetchCounter()-1;i++){
						temprecs->addNode(temp->fetchID());
						table[i]=temp->fetchID();
						temp=temp->fetchNext();
					}
				}
				double* results=myhash.print_multi_balance(*temprecs);
				if(delay){
					if(results==NULL){
						result="Error. Multi-Balance (";
						result.append(table[0]);
						for(int i=1;i<temprecs->fetchCounter();i++){
							result.append(":");
							result.append(table[i]);
						}
						result.append(":");
						result.append(delaystr);
						result.append(")");
					}
					else{
						char buffer[32];
						snprintf(buffer, sizeof(buffer), "%g", results[0]);
						result="Success. Multi-Balance (";
						result.append(table[0]);
						result.append("/");
						result.append(buffer);
						for(int i=1;i<temprecs->fetchCounter();i++){
							char buffer[32];
							snprintf(buffer, sizeof(buffer), "%g", results[i]);
							result.append(":");
							result.append(table[i]);
							result.append("/");
							result.append(buffer);
						}
						result.append(":");
						result.append(delaystr);
						result.append(")");
					}
				}
				else{
					if(results==NULL){
						result="Error. Multi-Balance (";
						result.append(table[0]);
						for(int i=1;i<temprecs->fetchCounter();i++){
							result.append(":");
							result.append(table[i]);
						}
						result.append(")");
					}
					else{
						char buffer[32];
						snprintf(buffer, sizeof(buffer), "%g", results[0]);
						result="Success. Multi-Balance (";
						result.append(table[0]);
						result.append("/");
						result.append(buffer);
						for(int i=1;i<temprecs->fetchCounter();i++){
							char buffer[32];
							snprintf(buffer, sizeof(buffer), "%g", results[i]);
							result.append(":");
							result.append(table[i]);
							result.append("/");
							result.append(buffer);
						}
						result.append(")");
					}
				}
			}
			else
				result="Error. Unknown command";
			write(sock,to_string(result.length()+1).c_str(),MSGBUF);
			write(sock,result.c_str(),result.length()+1);
		}while(1);
		cout<<"Closing socket"<<endl;
		close(sock);
	}
}

int main(int argc, char* argv[]){
	if(argc!=7){
		cout<<"Wrong number of arguments"<<endl;
		return 0;
    }
    if (strcmp(argv[1],"-p")==0)
		port=atoi(argv[2]);
	else if(strcmp(argv[1],"-s")==0)
		worker_threads=atoi(argv[2]);
	else if(strcmp(argv[1],"-q")==0)
		pool.q_size=atoi(argv[2]);
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
	if (strcmp(argv[3],"-p")==0)
		port=atoi(argv[4]);
	else if(strcmp(argv[3],"-s")==0)
		worker_threads=atoi(argv[4]);
	else if(strcmp(argv[3],"-q")==0)
		pool.q_size=atoi(argv[4]);
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
    if (strcmp(argv[5],"-p")==0)
		port=atoi(argv[6]);
	else if(strcmp(argv[5],"-s")==0)
		worker_threads=atoi(argv[6]);
	else if(strcmp(argv[5],"-q")==0)
		pool.q_size=atoi(argv[6]);
	else{
		cout<<"Unknown argument"<<endl;
		return 0;
	}
	pthread_t master ;
	pthread_t workers[worker_threads];
	initialize (&pool);
	pthread_mutex_init(&mtx,0);
	pthread_cond_init(&cond_nonempty,0) ;
	pthread_cond_init(&cond_nonfull,0) ;
	pthread_mutex_init(&wrmtx,0);
	pthread_cond_init(&write_cond,0) ;
	pthread_cond_init(&read_cond,0) ;
	pthread_create(&master,0,producer,0) ;
	for(int i=0;i<worker_threads;i++)
		pthread_create(&workers[i],0,consumer,0) ;
	for(int i=0;i<worker_threads;i++)
		pthread_join(workers[i],0);
	pthread_join(master,0);
	return 0;
}
