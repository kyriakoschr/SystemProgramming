#include <iostream>
#include "transactions.h"
#include "list.h"

using namespace std;

class TransactionNode;

class Account{
	private:
		int AccountNo;
		bool checked;
		TransactionList* OutcomeTransactions;
		TransactionList* IncomeTransactions;
	public:
		Account(const int);
		~Account();
		int fetchID(){ return AccountNo;};
		void AddOutTransaction(Account*,const double);
		void AddInTransaction(Account*,const double);
		void deleteOutTrans(Account*);
		void deleteInTrans(Account*);
		TransactionListNode* fetchFirstOutTr(){return OutcomeTransactions->fetchFirst();};
		double SumOut();
		double SumIn();
		bool deletable();
		void print();
		void cycled(Account*,Lista&,Lista&);
		void connected(int&,Account*,Account*,Lista&,Lista&);
		void trace(double&,const int,int&,Lista&,Lista&);
		void trianconn(const double,Lista&,Lista&);
		void check(){checked=true;};
		void uncheck(){checked=false;};
		int receivers(){return OutcomeTransactions->fetchCounter();};
		bool checkOrNot(){return checked;};
};

class AccountListNode{
	private:
		Account* periexomeno;
		AccountListNode* Next;
	public:
		AccountListNode(const int);
		~AccountListNode();
		AccountListNode* fetchNext(){return Next;};
		void addNext(const int newobj){Next=new AccountListNode(newobj);};
		Account* fetchAccount(){return periexomeno;};
		void deleteNext();
};

class AccountList{
	private:
		AccountListNode* First;
		int counter;
	public:
		AccountList(){First=NULL;counter=0;};
		void addNode(const int);
		bool isEmpty(){if (counter==0) return true;else return false;};
		Account* fetchAccount(const int);
		AccountListNode* fetchFirst(){return First;};
		void deleteNode(const int);
		int fetchCounter(){return counter;};
};
