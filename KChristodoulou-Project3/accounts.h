#include <iostream>
#include "transactions.h"

using namespace std;

class TransactionNode;

class Account{
	private:
		string AccountNo;
		double Balance;
		TransactionList* IncomeTransactions;
	public:
		Account(const string,const double);
		~Account();
		string fetchID(){ return AccountNo;};
		int AddInTransaction(Account*,const double);
		double Bal(){return Balance;};
		void SendMoney(const double amount){Balance-=amount;};
		double SumIn();
		void print();
};

class AccountListNode{
	private:
		Account* periexomeno;
		AccountListNode* Next;
	public:
		AccountListNode(const string,const double);
		~AccountListNode();
		AccountListNode* fetchNext(){return Next;};
		int addNext(const string ,const double);
		Account* fetchAccount(){return periexomeno;};
};

class AccountList{
	private:
		AccountListNode* First;
		int counter;
	public:
		AccountList(){First=NULL;counter=0;};
		int addNode(const string,const double);
		bool isEmpty(){if (counter==0) return true;else return false;};
		Account* fetchAccount(const string);
		AccountListNode* fetchFirst(){return First;};
		int fetchCounter(){return counter;};
};
