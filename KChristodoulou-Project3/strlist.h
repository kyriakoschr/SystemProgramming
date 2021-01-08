#include <iostream>

using namespace std;

class StrListNode{
	private:
		string periexomeno;
		StrListNode* Next;
	public:
		StrListNode(string);
		~StrListNode();
		StrListNode* fetchNext(){return Next;};
		void addNext(string newobj){Next=new StrListNode(newobj);};
		string fetchID(){return periexomeno;};
		void deleteNext();
		void PrintReverse();
};

class StrList{
	private:
		StrListNode* First;
		int counter;
	public:
		StrList(){First=NULL;counter=0;};
		~StrList();
		void addNode(string);
		string fetchNode(string);
		void deleteNode(string);
		string fetchLastID();
		StrListNode* fetchFirst(){return First;};
		int fetchCounter(){return counter;};
		void emptyList(){delete First;First=NULL;counter=0;};
		void print();
};

