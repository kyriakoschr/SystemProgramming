#include <iostream>
#include <fstream>
#include "hasht.h"
#include "strlist.h"
#include <string>

using namespace std;

int htsize;

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
	string cmd;
	StrList tokens;
	StrListNode* tempstr1,* tempstr2,* tempstr3;
	char* temp;
	string temps="-b";
	int flag=0;
	if(argc==5){
		flag=1;
		if(argv[1]==temps){
			htsize=stoi(argv[2]);
			temp=(argv[4]);
		}
		else if(argv[3]==temps){
			htsize=stoi(argv[4]);
			temp=(argv[2]);
		}
	}
	else if(argc==3)
		htsize=stoi(argv[2]);
	else {
		cout<<"Wrong number of inputs"<<endl;
		return -1;
	}
	HashTable myhashtable(htsize);
	if(flag==1){
		ifstream operations (temp);
		if(operations.is_open())
			while(getline(operations,cmd)){
				tokenize(cmd,tokens);
				if(tokens.fetchFirst()->fetchID()=="createnodes"){
					if(tokens.fetchCounter()>1){
						tempstr1=tokens.fetchFirst()->fetchNext();
						for(int i=1;i<tokens.fetchCounter();i++){
							if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
								myhashtable.createnondes(stoi(tempstr1->fetchID()));
							tempstr1=tempstr1->fetchNext();
						}
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="delnodes"){
					if (tokens.fetchCounter()>1){
						tempstr1=tokens.fetchFirst()->fetchNext();
						for(int i=1;i<tokens.fetchCounter();i++){
							if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
								myhashtable.delnodes(stoi(tempstr1->fetchID()));
							tempstr1=tempstr1->fetchNext();
						}
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="addtran"){
					if(tokens.fetchCounter()==4){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();tempstr3=tempstr2->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0)&&(stod(tempstr3->fetchID())))
							myhashtable.addtran(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()),stod(tempstr3->fetchID()));
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="deltran"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.deltran(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="lookup"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							if(tempstr1->fetchID()=="in")
								myhashtable.lookup(in,stoi(tempstr2->fetchID()));
							else if(tempstr1->fetchID()=="out")
								myhashtable.lookup(out,stoi(tempstr2->fetchID()));
							else if(tempstr1->fetchID()=="sum")
								myhashtable.lookup(sum,stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="triangle"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stod(tempstr2->fetchID())))
							myhashtable.triangle(stoi(tempstr1->fetchID()),stod(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="conn"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.conn(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="allcycles"){
					if(tokens.fetchCounter()==2){
						tempstr1=tokens.fetchFirst()->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
							myhashtable.allcycles(stoi(tempstr1->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="traceflow"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.traceflow(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="bye"){
					if(tokens.fetchCounter()==1)
						myhashtable.bye();
				}
				else if(tokens.fetchFirst()->fetchID()=="print"){
					if(tokens.fetchCounter()==1)
						myhashtable.print();
				}
				else if(tokens.fetchFirst()->fetchID()=="dump"){
					if(tokens.fetchCounter()==2)
						myhashtable.dump(tokens.fetchFirst()->fetchNext()->fetchID());
				}
				tokens.emptyList();
				tempstr1=NULL;tempstr2=NULL;tempstr3=NULL;
			}
		}
		string cmd2;
		getline(cin,cmd2);
		while(cmd2.find(EOF)==string::npos){
				tokenize(cmd2,tokens);
				if(tokens.fetchFirst()->fetchID()=="createnodes"){
					if(tokens.fetchCounter()>1){
						tempstr1=tokens.fetchFirst()->fetchNext();
						for(int i=1;i<tokens.fetchCounter();i++){
							if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
								myhashtable.createnondes(stoi(tempstr1->fetchID()));
							tempstr1=tempstr1->fetchNext();
						}
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="delnodes"){
					if (tokens.fetchCounter()>1){
						tempstr1=tokens.fetchFirst()->fetchNext();
						for(int i=1;i<tokens.fetchCounter();i++){
							if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
								myhashtable.delnodes(stoi(tempstr1->fetchID()));
							tempstr1=tempstr1->fetchNext();
						}
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="addtran"){
					if(tokens.fetchCounter()==4){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();tempstr3=tempstr2->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0)&&(stod(tempstr3->fetchID())))
							myhashtable.addtran(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()),stod(tempstr3->fetchID()));
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="deltran"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.deltran(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if (tokens.fetchFirst()->fetchID()=="lookup"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							if(tempstr1->fetchID()=="in")
								myhashtable.lookup(in,stoi(tempstr2->fetchID()));
							else if(tempstr1->fetchID()=="out")
								myhashtable.lookup(out,stoi(tempstr2->fetchID()));
							else if(tempstr1->fetchID()=="sum")
								myhashtable.lookup(sum,stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="triangle"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stod(tempstr2->fetchID())))
							myhashtable.triangle(stoi(tempstr1->fetchID()),stod(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="conn"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.conn(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="allcycles"){
					if(tokens.fetchCounter()==2){
						tempstr1=tokens.fetchFirst()->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0))
							myhashtable.allcycles(stoi(tempstr1->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="traceflow"){
					if(tokens.fetchCounter()==3){
						tempstr1=tokens.fetchFirst()->fetchNext();tempstr2=tempstr1->fetchNext();
						if((stoi(tempstr1->fetchID())<=9999999)&&(stoi(tempstr1->fetchID())>=0)&&(stoi(tempstr2->fetchID())<=9999999)&&(stoi(tempstr2->fetchID())>=0))
							myhashtable.traceflow(stoi(tempstr1->fetchID()),stoi(tempstr2->fetchID()));
					}
				}
				else if(tokens.fetchFirst()->fetchID()=="bye"){
					if(tokens.fetchCounter()==1)
						myhashtable.bye();
				}
				else if(tokens.fetchFirst()->fetchID()=="print"){
					if(tokens.fetchCounter()==1)
						myhashtable.print();
				}
				else if(tokens.fetchFirst()->fetchID()=="dump"){
					if(tokens.fetchCounter()==2)
						myhashtable.dump(tokens.fetchFirst()->fetchNext()->fetchID());
				}
				tokens.emptyList();
				tempstr1=NULL;tempstr2=NULL;tempstr3=NULL;
				getline(cin,cmd2);
			}
			return 0;
}
