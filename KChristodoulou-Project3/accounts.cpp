#include "accounts.h"

using namespace std;

Account::Account(const string accNO,const double amount){
	AccountNo=accNO;
	Balance=amount;
	IncomeTransactions=new TransactionList;
}

AccountListNode::AccountListNode(const string object,const double amount){
	periexomeno=new Account(object,amount);
	Next=NULL;
}

int AccountListNode::addNext(const string newobj,const double amount){
	Next=new AccountListNode(newobj,amount);
	if(Next->fetchAccount()->fetchID()==newobj)
		return 1;
	return 0;
}

int AccountList::addNode(const string periexomeno,const double amount){
	AccountListNode* temp=First;
	if(counter==0){
		First=new AccountListNode(periexomeno,amount);
		if(First->fetchAccount()->fetchID()==periexomeno){
			counter++;
			return 1;
		}
		else return 0;
	}
	else if (counter==1){
		if(First->addNext(periexomeno,amount)==1){
			counter++;
			return 1;
		}
		return 0;
	}
	else{
		for(int i=0;i<counter-1;i++)
			temp=temp->fetchNext();
		if(temp->addNext(periexomeno,amount)==1){
			counter++;
			return 1;
		}
		return 0;
	}
}

Account* AccountList::fetchAccount(const string target){
	AccountListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchAccount()->fetchID()==target)
			return temp->fetchAccount();
		else
			temp=temp->fetchNext();
	return NULL;
}

int Account::AddInTransaction(Account* sender,const double amount){
	if(IncomeTransactions->isEmpty()==false){
		if(IncomeTransactions->exist(sender,this)==true){
			IncomeTransactions->fetchTransaction(sender,this)->changeAmount(amount);
			this->Balance+=amount;
			return 1;
		}
		else{
			if(IncomeTransactions->addNode(amount,sender,this)==1){
				this->Balance+=amount;
				return 1;
			}
			return 0;
		}
	}
	else
		if(IncomeTransactions->addNode(amount,sender,this)==1){
			this->Balance+=amount;
			return 1;
		}
		return 0;
}

double Account::SumIn(){
	return IncomeTransactions->SumUp();
}

void Account::print(){
	TransactionListNode *temp=IncomeTransactions->fetchFirst();
	cout<<"vertex("<<this->fetchID()<<", Balance "<<this->Bal()<<") = ";
	if(IncomeTransactions->isEmpty()==true){
		cout<<"nothing"<<endl;
		return;
	}
	for(int i=0;i<IncomeTransactions->fetchCounter()-1;i++){
		cout<<"("<<temp->fetchTransaction()->fetchReceiver()->fetchID()<<","<<temp->fetchTransaction()->fetchAmount()<<"),";
		temp=temp->fetchNext();
	}
	cout<<"("<<temp->fetchTransaction()->fetchReceiver()->fetchID()<<","<<temp->fetchTransaction()->fetchAmount()<<")"<<endl;
}

AccountListNode::~AccountListNode(){
	delete Next;
	delete periexomeno;
}

Account::~Account(){
	delete IncomeTransactions;
}
