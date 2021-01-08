#include <iostream>

using namespace std;

class StrListNode{
	private:
		string periexomeno;
		StrListNode* Next;
	public:
		StrListNode(string);
		~StrListNode(){delete Next;};
		StrListNode* fetchNext(){return Next;};
		void addNext(string newobj){Next=new StrListNode(newobj);};
		string fetchID(){return periexomeno;};
};

class StrList{
	private:
		StrListNode* First;
		int counter;
	public:
		StrList(){First=NULL;counter=0;};
		~StrList(){delete First;};
		void addNode(string);
		string fetchNode(string);
		StrListNode* fetchFirst(){return First;};
		StrListNode* fetchMsg(const int);
		int fetchCounter(){return counter;};
		void emptyList(){delete First;First=NULL;counter=0;};
};

