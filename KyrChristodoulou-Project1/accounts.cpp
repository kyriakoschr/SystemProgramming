#include "accounts.h"

using namespace std;

Account::Account(const int accNO){
	AccountNo=accNO;
	checked=false;
	OutcomeTransactions=new TransactionList;
	IncomeTransactions=new TransactionList;
}

AccountListNode::AccountListNode(const int object){
	periexomeno=new Account(object);
	Next=NULL;
}

void AccountList::addNode(const int periexomeno){
	AccountListNode* temp=First;
	if(counter==0){
		First=new AccountListNode(periexomeno);
		counter++;
	}
	else if (counter==1){
		First->addNext(periexomeno);
		counter++;
	}
	else{
		for(int i=0;i<counter-1;i++)
			temp=temp->fetchNext();
		temp->addNext(periexomeno);
		counter++;
	}
}

Account* AccountList::fetchAccount(const int target){
	AccountListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchAccount()->fetchID()==target)
			return temp->fetchAccount();
		else
			temp=temp->fetchNext();
	return NULL;
}

void AccountList::deleteNode(const int target){
	AccountListNode *temp=First;
	if(First->fetchAccount()->fetchID()==target){
		First=temp->fetchNext();
		delete temp;
		counter--;
		return;
	}
	else{
		for(int i=0;i<counter-1;i++)
			if(temp->fetchNext()->fetchAccount()->fetchID()==target){
				temp->deleteNext();
				counter--;
				return;
			}
			else
				temp=temp->fetchNext();
		if(temp->fetchNext()->fetchAccount()->fetchID()==target){
			temp->deleteNext();
			counter--;
		}
	}
}

void AccountListNode::deleteNext(){
	AccountListNode* temp=Next;
	Next=temp->fetchNext();
	delete temp;
}

void Account::AddOutTransaction(Account* receiver,const double amount){
	if(OutcomeTransactions->isEmpty()==false){
		if(OutcomeTransactions->exist(this,receiver)==true)
			OutcomeTransactions->fetchTransaction(this,receiver)->changeAmount(amount);
		else
			OutcomeTransactions->addNode(amount,this,receiver);
	}
	else
		OutcomeTransactions->addNode(amount,this,receiver);
}

void Account::AddInTransaction(Account* sender,const double amount){
	if(IncomeTransactions->isEmpty()==false){
		if(IncomeTransactions->exist(sender,this)==true)
			IncomeTransactions->fetchTransaction(sender,this)->changeAmount(amount);
		else
			IncomeTransactions->addNode(amount,sender,this);
	}
	else
		IncomeTransactions->addNode(amount,sender,this);
}

void Account::deleteOutTrans(Account* receiver){
	OutcomeTransactions->deleteNode(this,receiver);
}

void Account::deleteInTrans(Account* sender){
	IncomeTransactions->deleteNode(sender,this);
}

double Account::SumOut(){
	return OutcomeTransactions->SumUp();
}

double Account::SumIn(){
	return IncomeTransactions->SumUp();
}

bool Account::deletable(){
	if((IncomeTransactions->isEmpty())&&(OutcomeTransactions->isEmpty()))
		return true;
	else return false;
}

void Account::connected(int& flag,Account* source,Account* target,Lista& CheckList,Lista& PrintList){
	CheckList.addNode(this->fetchID());
	PrintList.addNode(this->fetchID());
	if(this->fetchID()==target->fetchID()){
		PrintList.addNode(target->fetchID());
		cout<<"success: conn("<<source->fetchID()<<","<<target->fetchID()<<") = (";
		PrintList.fetchFirst()->PrintReverse();
		cout<<")"<<endl;
		flag=1;
		return;
	}
	else{
		TransactionListNode* temp=this->OutcomeTransactions->fetchFirst();
		for(int i=0;i<this->OutcomeTransactions->fetchCounter()-1;i++){
			if(CheckList.exist(temp->fetchTransaction()->fetchReceiver()->fetchID())==true){
				temp=temp->fetchNext();}
			else{
				temp->fetchTransaction()->fetchReceiver()->connected(flag,source,target,CheckList,PrintList);
				if(flag==1)
					return;
				else temp=temp->fetchNext();
			}
		}
	}
}

void Account::trianconn(const double k,Lista& CheckList,Lista& PrintList){
	PrintList.addNode(this->fetchID());
	CheckList.addNode(this->fetchID());
	int first=PrintList.fetchFirst()->fetchID();
	TransactionListNode* temp=OutcomeTransactions->fetchFirst();
	for(int i=0;i<OutcomeTransactions->fetchCounter();i++){
		if(PrintList.fetchCounter()>3)
			return;
		else if(temp->fetchTransaction()->fetchAmount()<k)
			temp=temp->fetchNext();
		else{
			if(CheckList.exist(temp->fetchTransaction()->fetchReceiver()->fetchID())==false)
				temp->fetchTransaction()->fetchReceiver()->trianconn(k,CheckList,PrintList);
			else if((PrintList.fetchCounter()==3)&&(first==temp->fetchTransaction()->fetchReceiver()->fetchID())){
				cout<<"success: triangle ("<<first<<","<<k<<") ="<<endl;
				cout<<"("<<first<<",";
				PrintList.fetchFirst()->PrintReverse();cout<<")"<<endl;
			}
		temp=temp->fetchNext();
		}
	}
}
void Account::trace(double& total,const int l,int& tempcntr,Lista& checklist,Lista& printlist){
	printlist.addNode(this->fetchID());
	checklist.addNode(this->fetchID());
	if(l==tempcntr){
		cout<<"success: traceflow ("<<printlist.fetchFirst()->fetchID()<<","<<l<<") ="<<endl;
		printlist.print();
		cout<<total<<")"<<endl;
		return;
	}
	TransactionListNode* temp=OutcomeTransactions->fetchFirst();
	for(int i=0;i<OutcomeTransactions->fetchCounter()-1;i++){
		if(checklist.exist(temp->fetchTransaction()->fetchReceiver()->fetchID())==true)
			temp=temp->fetchNext();
		else{
			tempcntr++;
			total+=temp->fetchTransaction()->fetchAmount();
			temp->fetchTransaction()->fetchReceiver()->trace(total,l,tempcntr,checklist,printlist);
		}
	}
}

void Account::cycled(Account* target,Lista& CheckList,Lista& PrintList){
	PrintList.addNode(this->fetchID());
	CheckList.addNode(this->fetchID());
	TransactionListNode* temp=OutcomeTransactions->fetchFirst();
	for(int i=0;i<OutcomeTransactions->fetchCounter()-1;i++){
		if (CheckList.exist(temp->fetchTransaction()->fetchReceiver()->fetchID())==false)
			temp->fetchTransaction()->fetchReceiver()->cycled(target,CheckList,PrintList);
		else if((PrintList.fetchCounter()>3)&&(this==target)){
			cout<<"success: cycles "<<target->fetchID()<<" ="<<endl;
			PrintList.print();
			cout<<")"<<endl;
			return;
		}
	}
}

void Account::print(){
	TransactionListNode *temp=OutcomeTransactions->fetchFirst();
	cout<<"vertex("<<this->fetchID()<<") = ";
	if(OutcomeTransactions->isEmpty()==true){
		cout<<"nothing"<<endl;
		return;
	}
	for(int i=0;i<OutcomeTransactions->fetchCounter()-1;i++){
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
	delete OutcomeTransactions;
	delete IncomeTransactions;
}
