#include <iostream>

using namespace std;

class Account;

class Transaction{
	private:
		double Amount;
		Account* Sender;
		Account* Receiver;
	public:
		Transaction(const double,Account*,Account*);
		Account* fetchSender(){return Sender;};
		Account* fetchReceiver(){return Receiver;};
		double fetchAmount(){return Amount;};
		void changeAmount(const double newAm){Amount=Amount+newAm;};
};

class TransactionListNode{
	private:
		Transaction* periexomeno;
		TransactionListNode* Next;
	public:
		TransactionListNode(const double,Account*,Account*);
		~TransactionListNode(){delete Next;delete periexomeno;};
		TransactionListNode* fetchNext(){return Next;};
		int addNext(const double,Account* ,Account* );
		Transaction* fetchTransaction(){return periexomeno;};
};

class TransactionList{
	private:
		TransactionListNode* First;
		int counter;
	public:
		TransactionList(){First=NULL;counter=0;};
		~TransactionList(){delete First;};
		int addNode(const double,Account*,Account*);
		bool isEmpty(){ if(counter==0) return true; else return false;};
		Transaction* fetchTransaction(Account*,Account*);
		bool exist(Account*,Account*);
		double SumUp();
		int fetchCounter(){return counter;};
		TransactionListNode* fetchFirst(){return First;};
};
