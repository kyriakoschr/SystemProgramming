#include "hasht.h"

using namespace std;

pthread_mutex_t hshtblocks[num_of_locks];

int hashf(const string s,const int M){
	int intLength = s.length() / 4;
	long Sum = 0;
	for (int j = 0; j < intLength; j++) {
		string c=s.substr(j * 4, (j * 4) + 4);
		long mult = 1;
		for (int k = 0; k < c.length(); k++) {
			Sum += (c.c_str())[k] * mult;
			mult *= 256;
		}
	}
	string c2 = s.substr(intLength * 4);
	long mult = 1;
	for (int k = 0; k < c2.length(); k++) {
		Sum += (c2.c_str())[k] * mult;
		mult *= 256;
	}
	return(abs(Sum) % M);
}

Account* HashTableCell::fetchAccount(const string target){
	if(lista.isEmpty()==false)
		return lista.fetchAccount(target);
	return NULL;
}

int HashTableCell::addNode(const string newAcc,const double amount){
	if(lista.isEmpty()==false)
		if(lista.fetchAccount(newAcc)!=NULL)
			return 0;
	if(lista.addNode(newAcc,amount)!=1)
		return 0;
	else
		return 1;
}

HashTable::HashTable(const int HTS){
	HTable=new HashTableCell*[HTS];
	for(int i=0;i<HTS;i++)
		HTable[i]=new HashTableCell();
	for(int i=0;i<num_of_locks;i++){
		pthread_mutex_init(&hshtblocks[i],NULL);
	}
}

int HashTable::createnondes(const string newN,const double amount,const int delay){
	int targetbucket=hashf(newN,hashtsize);
	int targetlock=targetbucket/num_of_locks;
	pthread_mutex_lock (&hshtblocks[targetlock]);
	if(HTable[targetbucket]->addNode(newN,amount)!=1){
		usleep(delay);
		pthread_mutex_unlock (&hshtblocks[targetlock]);
		return 0;
	}
	usleep(delay);
	pthread_mutex_unlock (&hshtblocks[targetlock]);
	return 1;
}

bool exist(int* locksarray,int counter,int newlock){
	for(int i=0;i<counter;i++){
		if(locksarray[i]==newlock)
			return true;
	}
	return false;
}

void sort(int* locksarray,int counter){
	int temp=0;
	for(int i=0; i<counter; i++)
		for(int j=i+1; j<counter; j++)
			if(locksarray[i]>locksarray[j]){
				temp=locksarray[i];
				locksarray[i]=locksarray[j];
				locksarray[j]=temp;
			}
}

int HashTable::addmultitran(const string from,StrList to, const double amount,const int delay){
	Account* source;
	Account* dest;
	int targetbucket[(to.fetchCounter())];
	int targetlock[(1+to.fetchCounter())];
	int sourcebucket=hashf(from,hashtsize);
	int counter=0;
	StrListNode* temp=to.fetchFirst();
	for(int i=0;i<to.fetchCounter()+1;i++)
		targetlock[i]=-1;				//arxikopiw tis klidwnies me -1
	for(int i=0;i<to.fetchCounter();i++){
		targetbucket[i]=hashf(temp->fetchID(),hashtsize);	//vazw ston pinaka to bucket tou kathe paralipti
		if(exist(targetlock,counter,targetbucket[i]/num_of_locks)==false){	//elegxo an iparxei idi klidwnia tou en logw bucket sto pinaka targetlock
			targetlock[counter]=targetbucket[i]/num_of_locks;		//an den iparxei tin apothikevw
			counter++;
		}
		temp=temp->fetchNext();
	}
	if(exist(targetlock,counter,sourcebucket/num_of_locks)==false){	//kanw ton elegxo gia tin klidwnia tou apostolea
		targetlock[counter]=sourcebucket/num_of_locks;
		counter++;
	}
	sort(targetlock,counter);					//taksinomw tiw klidwnies se afxousa seira
	for(int i=0;i<counter;i++)
		if(targetlock[i]!=-1)
			pthread_mutex_lock (&hshtblocks[targetlock[i]]);	//tis kleidwnw
	source=HTable[sourcebucket]->fetchAccount(from);		//pernw ton apostolea
	if(source==NULL){										//kai elegxw an iparxei
		usleep(delay);
		for(int i=0;i<counter;i++)
			if(targetlock[i]!=-1)							//an den iparxei termatizei i sinartisi, epistrefei 0 afou kanei unlock oles tis klidonies
				pthread_mutex_unlock (&hshtblocks[targetlock[i]]);
		return 0;
	}
	if((to.fetchCounter()*amount)>source->Bal()){		//elegxw an to ipoloipo toy apostolea arkei gia oles tis sinallages
		usleep(delay);
		for(int i=0;i<counter;i++)
			if(targetlock[i]!=-1)
				pthread_mutex_unlock (&hshtblocks[targetlock[i]]);
		return 0;
	}
	temp=to.fetchFirst();
	for(int i=0;i<to.fetchCounter();i++){				//kai an iparxoun OLOI oi paraliptes
		dest=HTable[targetbucket[i]]->fetchAccount(temp->fetchID());
		if(dest==NULL){
			usleep(delay);
			for(int j=0;j<counter;j++)
				if(targetlock[j]!=-1)
					pthread_mutex_unlock (&hshtblocks[targetlock[j]]);
			return 0;
		}
		temp=temp->fetchNext();
	}
	temp=to.fetchFirst();
	for(int i=0;i<to.fetchCounter();i++){				//gia kathe paralipti prosthetw ti sinallagi stis iserxomenes toy kai aferw apo ton apostolea
		dest=HTable[targetbucket[i]]->fetchAccount(temp->fetchID());
		if(dest->AddInTransaction(source,amount)==1){
			source->SendMoney(amount);
			temp=temp->fetchNext();
		}
		else{												//an mia sinallagi apotixei ksekinw apo ton arxi mexri ekei pou eftase
			temp=to.fetchFirst();							//kai kanw to antitheto
			dest=HTable[targetbucket[i]]->fetchAccount(temp->fetchID());
			for(int j=0;j<i;j++){
				if(dest->AddInTransaction(source,amount*(-1))==1){
					source->SendMoney(amount*(-1));
					temp=temp->fetchNext();
				}
			}
			usleep(delay);
			for(int j=0;j<counter;j++)				//kanw unlock kai epistrefei 0
				if(targetlock[j]!=-1)
					pthread_mutex_unlock (&hshtblocks[targetlock[j]]);
			return 0;
		}
	}
	usleep(delay);
	for(int j=0;j<counter;j++) //se periptwsi apolitis epitixias kanei unlock kai epistrefei 1
		if(targetlock[j]!=-1)
			pthread_mutex_unlock (&hshtblocks[targetlock[j]]);
	return 1;
}

int HashTable::addtran(const string from,const string to,const double amount,const int delay){
	int targetbucketfrom=hashf(from,hashtsize);
	int targetbucketto=hashf(to,hashtsize);
	int targetlockfrom=targetbucketfrom/num_of_locks;
	int targetlockto=targetbucketto/num_of_locks;
	if(targetlockfrom!=targetlockto)			// klidwnw tis klidwnies me afxousa seira
		if(targetlockfrom<targetlockto){
			pthread_mutex_lock (&hshtblocks[targetlockfrom]);
			pthread_mutex_lock (&hshtblocks[targetlockto]);
		}
		else{
			pthread_mutex_lock (&hshtblocks[targetlockto]);
			pthread_mutex_lock (&hshtblocks[targetlockfrom]);
		}
	else
		pthread_mutex_lock (&hshtblocks[targetlockfrom]);
	Account* tempfrom=HTable[targetbucketfrom]->fetchAccount(from);
	Account* tempto=HTable[targetbucketto]->fetchAccount(to);
	if((tempfrom==NULL)||(tempto==NULL)){
		usleep(delay);
		if(targetlockfrom!=targetlockto)
			if(targetlockfrom<targetlockto){
				pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
				pthread_mutex_unlock (&hshtblocks[targetlockto]);
			}
			else{
				pthread_mutex_unlock (&hshtblocks[targetlockto]);
				pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
			}
		else
			pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
		return 0;
	}
	if(tempfrom->Bal()>=amount){	//an iparxei diathesimo ipolipo kanw ti metafora
		if(tempto->AddInTransaction(tempfrom,amount)!=1){ //an den petixei kseklidwnw
			usleep(delay);
			if(targetlockfrom!=targetlockto)
				if(targetlockfrom<targetlockto){
					pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
					pthread_mutex_unlock (&hshtblocks[targetlockto]);
				}
				else{
					pthread_mutex_unlock (&hshtblocks[targetlockto]);
					pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
				}
			else
				pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
			return 0;
		}
	}
	else{									//an den iparxei ipolipo kai pali kseklidwnw
		usleep(delay);
		if(targetlockfrom!=targetlockto)
			if(targetlockfrom<targetlockto){
				pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
				pthread_mutex_unlock (&hshtblocks[targetlockto]);
			}
			else{
				pthread_mutex_unlock (&hshtblocks[targetlockto]);
				pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
			}
		else
			pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
		return 0;
	}
	tempfrom->SendMoney(amount);//aferw ta lefta apo ton apostolea
	usleep(delay);
	if(targetlockfrom!=targetlockto)	//kai kseklidwnw
		if(targetlockfrom<targetlockto){
			pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
			pthread_mutex_unlock (&hshtblocks[targetlockto]);
		}
		else{
			pthread_mutex_unlock (&hshtblocks[targetlockto]);
			pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
		}
	else
		pthread_mutex_unlock (&hshtblocks[targetlockfrom]);
	return 1;
}

AccountListNode* HashTableCell::fetchFirst(){
	return lista.fetchFirst();
}

HashTable::~HashTable(){
	for(int i=0;i<hashtsize;i++)
		delete HTable[i];
	delete[] HTable;
}

double HashTable::print_balance(const string AccName,const int delay){
	int targetbucket=hashf(AccName,hashtsize);
	int targetlock=targetbucket/num_of_locks;
	double result;
	pthread_mutex_lock (&hshtblocks[targetlock]);
	Account* tempAcc=HTable[targetbucket]->fetchAccount(AccName);
	if(tempAcc==NULL){
		usleep(delay);
		pthread_mutex_unlock (&hshtblocks[targetlock]);
		return -1;
	}
	result=tempAcc->Bal();
	usleep(delay);
	pthread_mutex_unlock (&hshtblocks[targetlock]);
	return result;
}

double* HashTable::print_multi_balance( StrList names,const int delay){
	double* results=new double[names.fetchCounter()];
	Account* dest;
	int targetbucket[names.fetchCounter()];
	int targetlock[names.fetchCounter()];
	for(int i=0;i<names.fetchCounter();i++)
		targetlock[i]=-1;
	int counter=0;
	StrListNode* temp=names.fetchFirst();
	for(int i=0;i<names.fetchCounter();i++){
		targetbucket[i]=hashf(temp->fetchID(),hashtsize);	//vazw ston pinaka to bucket tou kathe logariasmoy
		if(exist(targetlock,counter,targetbucket[i]/num_of_locks)==false){	//elegxo an iparxei idi klidwnia tou en logw bucket sto pinaka targetlock
			targetlock[counter]=targetbucket[i]/num_of_locks;		//an den iparxei tin apothikevw
			counter++;
		}
		temp=temp->fetchNext();
	}
	sort(targetlock,counter);					//taksinomw tiw klidwnies se afxousa seira
	for(int i=0;i<counter;i++)
		if(targetlock[i]!=-1)
			pthread_mutex_lock (&hshtblocks[targetlock[i]]);	//tis kleidwnw
	temp=names.fetchFirst();
	for(int i=0;i<names.fetchCounter();i++){								//pernaw ston pinaka apotelesmatwn to ipoloipo tou ekastote logariasmou
		dest=HTable[targetbucket[i]]->fetchAccount(temp->fetchID());
		if(dest==NULL){
			usleep(delay);
			for(int j=0;j<counter;j++)
				if(targetlock[j]!=-1)
					pthread_mutex_unlock (&hshtblocks[targetlock[j]]);	//tis kanw unlock
			return NULL;
		}
		else{
			results[i]=dest->Bal();
			if(names.fetchCounter()!=i+1){
				temp=temp->fetchNext();
				dest=HTable[targetbucket[i]]->fetchAccount(temp->fetchID());
			}
		}
	}
	usleep(delay);
	for(int i=0;i<counter;i++)
		if(targetlock[i]!=-1)
			pthread_mutex_unlock (&hshtblocks[targetlock[i]]);	//tis kanw unlock
	return results;
}
