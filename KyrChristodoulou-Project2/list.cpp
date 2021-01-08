#include "list.h"

using namespace std;

void tokenize(const char* instr,StrList& tokens,const string& delimiters = " ")
{
    const string &str(instr);
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos  = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the list.
        tokens.addNode(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

ListNode::ListNode(string object,const int cntr){
	channelName=object;
	id=cntr;
	Next=NULL;
}

void Lista::addNode(string periexomeno,const int newid){
	ListNode* temp=First;
	if(counter==0){
		First=new ListNode(periexomeno,newid);
		counter++;
	}
	else if(exist(newid)==false){
		for(int i=0;i<counter-1;i++)
			temp=temp->fetchNext();
		temp->addNext(periexomeno,newid);
		counter++;
	}
}

bool Lista::exist(const int target){
	if(fetchNode(target)!=NULL)
		return true;
	else return false;
}

ListNode* Lista::fetchNode(const int target){
	ListNode* temp=First;
	for(int i=0;i<counter;i++)
		if(temp->fetchID()==target)
			return temp;
		else
			temp=temp->fetchNext();
	return NULL;
}
