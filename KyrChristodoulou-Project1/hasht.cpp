#include "hasht.h"

using namespace std;

Account* HashTableCell::fetchAccount(const int target){
	if(lista.isEmpty()==false)
		return lista.fetchAccount(target);
	else
		return NULL;
}
int HashTableCell::fetchCounter(){
	return lista.fetchCounter();
}

void HashTableCell::addNode(const int newAcc){
	if(lista.isEmpty()==false)
		if(lista.fetchAccount(newAcc)!=NULL){
			cout<<"failure: Node already exists"<<endl;
			return;
		}
	lista.addNode(newAcc);
}

void HashTableCell::DeleteNode(const int target){
	if(lista.isEmpty()==false)
		if(lista.fetchAccount(target)!=NULL)
			lista.deleteNode(target);
}

HashTable::HashTable(const int HTSize){
	HTable=new HashTableCell*[HTSize];
	for(int i=0;i<htsize;i++)
		HTable[i]=new HashTableCell();
}

void HashTable::createnondes(const int newN){
	HTable[newN%htsize]->addNode(newN);
	cout<<"Success: created "<<newN<<endl;
}

void HashTable::delnodes(const int targ){
	Account* temp=HTable[targ%htsize]->fetchAccount(targ);
	if(temp!=NULL)
		if(temp->deletable()==true){
			HTable[targ%htsize]->DeleteNode(targ);
			cout<<"Success: deleted "<<targ<<endl;
		}
		else
			cout<<"failure: Can't delete "<<targ<<endl;
	else
		cout<<"failure: "<<targ<<" does not exist"<<endl;
}

void HashTable::addtran(const int from,const int to,const double amount){
	Account* tempfrom=HTable[from%htsize]->fetchAccount(from);
	Account* tempto=HTable[to%htsize]->fetchAccount(to);
	if((tempfrom!=NULL)&&(tempto!=NULL)){cout<<tempfrom->fetchID()<<endl;tempfrom->print();
		tempfrom->AddOutTransaction(tempto,amount);
		tempto->AddInTransaction(tempfrom,amount);
		cout<<"success: added transaction "<<from<<" "<<to<<" with amount "<<amount<<endl;
		tempfrom->print();
	}
	else
		cout<<"failure: missing Nodes"<<endl;
}

void HashTable::deltran(const int from,const int to){
	Account* tempfrom=HTable[from%htsize]->fetchAccount(from);
	Account* tempto=HTable[to%htsize]->fetchAccount(to);
	if((tempfrom!=NULL)&&(tempto!=NULL)){
		tempfrom->deleteOutTrans(tempto);
		tempto->deleteInTrans(tempfrom);
		cout<<"success: deleted transaction "<<from<<" "<<to<<endl;
	}
	else
		cout<<"failure: missing Nodes"<<endl;
}

void HashTable::lookup(const int flag,const int node){
	double result=0;
	Account* temp=HTable[node%htsize]->fetchAccount(node);
	if (temp==NULL){
		cout<<"failure: missing Node"<<endl;
		return ;
	}
	else
		if (flag==1)
			cout<<"success: in("<<node<<") = "<<temp->SumIn()<<endl;
		else if (flag==2)
			cout<<"success: out("<<node<<") = "<<temp->SumOut()<<endl;
		else if (flag==3)
			cout<<"success: sum("<<node<<") = "<<temp->SumIn()-temp->SumOut()<<endl;
}

void HashTable::conn(const int n1,const int n2){
	int result=0;
	Account* temp1=HTable[n1%htsize]->fetchAccount(n1);
	Account* temp2=HTable[n2%htsize]->fetchAccount(n2);
	Lista CheckList;
	Lista PrintList;
	if((temp1==NULL)||(temp1==NULL)){
		cout<<"failure: missing nodes"<<endl;
		return;
	}
	temp1->connected(result,temp1,temp2,CheckList,PrintList);
	if(result!=1)
		cout<<"conn("<<n1<<","<<n2<<") not found"<<endl;
}

void HashTable::triangle(const int n,const double k){
	Account* temp1=HTable[n%htsize]->fetchAccount(n);
	Lista CheckList;
	Lista PrintList;
	if(temp1==NULL){
		cout<<"failure: missing node"<<endl;
		return ;
	}
	temp1->trianconn(k,CheckList,PrintList);
}

void HashTable::allcycles(const int n){
	Account* temp1=HTable[n%htsize]->fetchAccount(n);
	Lista CheckList;
	Lista PrintList;
	if(temp1==NULL){
		cout<<"failure: missing node"<<endl;
		return;
	}
	temp1->cycled(temp1,CheckList,PrintList);
}

void HashTable::traceflow(const int n,const int l){
	int tempcntr=0;
	double total=0;
	Lista PrintList;
	Lista CheckList;
	Account* temp1=HTable[n%htsize]->fetchAccount(n);
	if(temp1==NULL){
		cout<<"failure: missing node"<<endl;
		return;
	}
	temp1->trace(total,l,tempcntr,CheckList,PrintList);
}

void HashTable::bye(){
	for(int i=0;i<htsize;i++)
		HTable[i]->emptyList();
	cout<<"success: cleaned memory"<<endl;

}

void HashTable::dump(string filename){
	ofstream myfile (filename);
	AccountListNode* temp;
	TransactionListNode *temptr;
	if(myfile.is_open()){
		for(int i=0;i<htsize;i++)
				if(HTable[i]->fetchCounter()>0){
					temp=HTable[i]->fetchFirst();
					for(int j=0;j<HTable[i]->fetchCounter();j++){
						myfile <<"createnodes ";
						myfile << temp->fetchAccount()->fetchID();
						myfile << "\n";
						temp=temp->fetchNext();
					}
				}
		for(int i=0;i<htsize;i++)
			if(HTable[i]->fetchCounter()>0){
				temp=HTable[i]->fetchFirst();
				for(int j=0;j<HTable[i]->fetchCounter();j++){
					temptr=temp->fetchAccount()->fetchFirstOutTr();
					for(int k=0;k<temp->fetchAccount()->receivers();k++){
						myfile <<"addtran ";
						myfile << temp->fetchAccount()->fetchID();
						myfile << " ";
						myfile << temptr->fetchTransaction()->fetchReceiver()->fetchID();
						myfile << " ";
						myfile << temptr->fetchTransaction()->fetchAmount();
						myfile << "\n";
						temptr=temptr->fetchNext();
					}
				}

			}
	}
}

AccountListNode* HashTableCell::fetchFirst(){
	return lista.fetchFirst();
}

void HashTableCell::emptyList(){
	delete lista.fetchFirst();
}

HashTable::~HashTable(){
	for(int i=0;i<htsize;i++)
		delete HTable[i];
	delete[] HTable;
}

void HashTable::print(){
	for(int i=0;i<htsize;i++){
		if(HTable[i]->fetchCounter()>0)
			HTable[i]->print();
	}
}

void HashTableCell::print(){
	AccountListNode* temp=lista.fetchFirst();
	for (int i=0;i<lista.fetchCounter();i++){
		temp->fetchAccount()->print();
		temp=temp->fetchNext();
	}
}
