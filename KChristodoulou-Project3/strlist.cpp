#include "strlist.h"

using namespace std;

StrListNode::StrListNode(string object){
	periexomeno=object;
	Next=NULL;
}

StrList::~StrList(){
	delete First;
}

StrListNode::~StrListNode(){
	delete Next;
}

void StrList::addNode(string periexomeno){
	StrListNode* temp=First;
	if (fetchNode(periexomeno)!="NULL")
		return;
	if(counter==0){
		First=new StrListNode(periexomeno);
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

string StrList::fetchNode(string target){
	StrListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchID()==target)
			return temp->fetchID();
		else
			temp=temp->fetchNext();
	return "NULL";
}

void StrList::deleteNode(string target){
	StrListNode *temp=First;
	if(First->fetchID()==target){
		First=temp->fetchNext();
		delete temp;
		counter--;
		return;
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

void StrListNode::deleteNext(){
	StrListNode* temp=Next;
	Next=temp->fetchNext();
	delete temp;
}

void StrListNode::PrintReverse(){
	if(Next!=NULL)
		Next->PrintReverse();
	cout<<fetchID()<<",";
}

string StrList::fetchLastID(){
	StrListNode* temp=fetchFirst();
	for(int i=0;i<counter-1;i++)
		temp=temp->fetchNext();
	return temp->fetchID();
}

void StrList::print(){
	StrListNode* temp=First;
	cout<<"(";
	for (int i=0;i<counter;i++){
		cout<<temp->fetchID()<<",";
		temp=temp->fetchNext();
	}
}
