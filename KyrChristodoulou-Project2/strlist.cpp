#include "strlist.h"

using namespace std;

StrListNode::StrListNode(string object){
	periexomeno=object;
	Next=NULL;
}

void StrList::addNode(string periexomeno){
	StrListNode* temp=First;
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

StrListNode* StrList::fetchMsg(const int target){
	StrListNode* temp=First;
	if (target>counter)
		return NULL;
	for(int i=1;i<target;i++)
		temp=temp->fetchNext();
	return temp;
}

string StrList::fetchNode(string target){
	StrListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchID()==target)
			return temp->fetchID();
		else
			temp=temp->fetchNext();
	return "";
}
