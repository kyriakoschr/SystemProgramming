#include "list.h"

using namespace std;

ListNode::ListNode(int object){
	periexomeno=object;
	Next=NULL;
}

void Lista::addNode(int periexomeno){
	ListNode* temp=First;
	if (fetchNode(periexomeno)!=NULL)
		return;
	if(counter==0){
		First=new ListNode(periexomeno);
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

bool Lista::exist(const int target){
	if(fetchNode(target)!=NULL)
		return true;
	else return false;
}

int Lista::fetchNode(int target){
	ListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchID()==target)
			return temp->fetchID();
		else
			temp=temp->fetchNext();
	return NULL;
}

void Lista::deleteNode(int target){
	ListNode* temp=First;
	if(First->fetchID()==target){
		First=First->fetchNext();
		counter--;
		delete temp;
	}
	else{
		for(int i=0;i<counter-1;i++)
			if(temp->fetchNext()->fetchID()==target){
				temp->deleteNext();
				counter--;
				return;
			}
			else
				temp=temp->fetchNext();
		if(temp->fetchNext()->fetchID()==target){
			temp->deleteNext();
			counter--;
		}
	}
}

void ListNode::deleteNext(){
	ListNode* temp=this->Next;
	this->Next=temp->fetchNext();
	delete temp;
}

void ListNode::PrintReverse(){
	if(Next!=NULL)
		Next->PrintReverse();
	cout<<fetchID()<<",";
}

int Lista::fetchLastID(){
	ListNode* temp=fetchFirst();
	for(int i=0;i<counter-1;i++)
		temp=temp->fetchNext();
	return temp->fetchID();
}

void Lista::print(){
	ListNode* temp=First;
	cout<<"(";
	for (int i=0;i<counter;i++){
		cout<<temp->fetchID()<<",";
		temp=temp->fetchNext();
	}
}
