#include <iostream>
#include "strlist.h"

using namespace std;

void tokenize(const char*,StrList&,const string&);

class ListNode{
	private:
		int id;
		string channelName;
		StrList MsgsList;
		StrList FileList;
		ListNode* Next;
	public:
		ListNode(string,const int);
		~ListNode(){delete Next;};
		ListNode* fetchNext(){return Next;};
		void addNext(string newobj,const int newid){Next=new ListNode(newobj,newid);};
		string fetchName(){return channelName;};
		int fetchID(){return id;};
		StrList* fetchMsgList(){return &MsgsList;};
		StrList* fetchFileList(){return &FileList;};
};

class Lista{
	private:
		ListNode* First;
		int counter;
	public:
		Lista(){First=NULL;counter=0;};
		~Lista(){delete First;};
		void addNode(string,const int);
		ListNode* fetchNode(const int);
		ListNode* fetchFirst(){return First;};
		int fetchCounter(){return counter;};
		bool exist(const int);
};
