#include "transactions.h"

using namespace std;

Transaction::Transaction(const double amount,Account* sender,Account* receiver){
	Amount=amount;
	Sender=sender;
	Receiver=receiver;
}

TransactionListNode::TransactionListNode(const double amount,Account* sender,Account* receiver){
	periexomeno=new Transaction(amount,sender,receiver);
	Next=NULL;
}

bool TransactionList::exist(Account* sender,Account* receiver){
	TransactionListNode* temp=First;
	for(int i=0;i<counter;i++){
		if((temp->fetchTransaction()->fetchSender()==sender)&&(temp->fetchTransaction()->fetchReceiver()==receiver))
			return true;
		else
			temp=temp->fetchNext();}
	return false;
}

void TransactionList::addNode(const double amount,Account* sender,Account* receiver){
	TransactionListNode* temp=First;
	if(counter==0){
		First=new TransactionListNode(amount,sender,receiver);
		counter++;
	}
	else if (counter==1){
		First->addNext(amount,sender,receiver);
		counter++;
	}
	else{
		for(int i=0;i<counter-1;i++)
			temp=temp->fetchNext();
		temp->addNext(amount,sender,receiver);
		counter++;
	}
}

Transaction* TransactionList::fetchTransaction(Account* sender,Account* receiver){
	TransactionListNode* temp=First;
	for(int i=0;i<counter;i++)
		if((temp->fetchTransaction()->fetchSender()==sender)&&(temp->fetchTransaction()->fetchReceiver()==receiver))
			return temp->fetchTransaction();
		else
			temp=temp->fetchNext();
	return NULL;
}

void TransactionList::deleteNode(Account* sender,Account* receiver){
	TransactionListNode *temp=First;
	if((First->fetchTransaction()->fetchSender()==sender)&&(First->fetchTransaction()->fetchReceiver()==receiver)){
		First=temp->fetchNext();
		delete temp;
		counter--;
		return;
	}
	else{
		for(int i=0;i<counter-1;i++)
			if((temp->fetchNext()->fetchTransaction()->fetchSender()==sender)&&(temp->fetchNext()->fetchTransaction()->fetchReceiver()==receiver)){
				temp->deleteNext();
				counter--;
				return;
			}
			else
				temp=temp->fetchNext();
		if((temp->fetchNext()->fetchTransaction()->fetchSender()==sender)&&(temp->fetchNext()->fetchTransaction()->fetchReceiver()==receiver)){
			temp->deleteNext();
			counter--;
		}
	}
}

void TransactionListNode::deleteNext(){
	TransactionListNode* temp=this->fetchNext();
	this->Next=temp->fetchNext();
	delete temp;
}

double TransactionList::SumUp(){
	double result=0;
	TransactionListNode* temp=First;
	for(int i=0;i<counter;i++){
		result+=temp->fetchTransaction()->fetchAmount();
		temp=temp->fetchNext();
	}
	return result;
}
