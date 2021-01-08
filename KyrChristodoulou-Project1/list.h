#include <iostream>

using namespace std;

class ListNode{
	private:
		int periexomeno;
		ListNode* Next;
	public:
		ListNode(int);
		~ListNode(){delete Next;};
		ListNode* fetchNext(){return Next;};
		void addNext(int newobj){Next=new ListNode(newobj);};
		int fetchID(){return periexomeno;};
		void deleteNext();
		void PrintReverse();
};

class Lista{
	private:
		ListNode* First;
		int counter;
	public:
		Lista(){First=NULL;counter=0;};
		~Lista(){delete First;};
		void addNode(int);
		int fetchNode(int);
		void deleteNode(int);
		int fetchLastID();
		ListNode* fetchFirst(){return First;};
		int fetchCounter(){return counter;};
		void emptyList(){delete First;First=NULL;counter=0;};
		void print();
		bool exist(const int);

};
