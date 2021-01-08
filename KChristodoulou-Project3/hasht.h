#include <iostream>
#include "accounts.h"
#include <fstream>
#include <stdlib.h>
#include <pthread.h>
#include "strlist.h"
#include <unistd.h>
#define in 1
#define out 2
#define sum 3
#define hashtsize 100
#define num_of_locks 20

class HashTableCell{
	private:
		AccountList lista;
	public:
		int addNode(const string,const double);
		Account* fetchAccount(const string);
		int fetchCounter();
		AccountListNode* fetchFirst();
		void print();
};

class HashTable{
	private:
		HashTableCell** HTable;
	public:
		HashTable(const int);
		~HashTable();
		int createnondes(const string,const double,const int delay = 0);
		int addtran(const string,const string,const double, const int delay = 0);
		int addmultitran(const string ,StrList, const double,const int delay = 0 );
		double print_balance(const string,const int delay = 0 );
		double* print_multi_balance( StrList,const int delay = 0 );
};
