#include <iostream>
#include "accounts.h"
#include <fstream>
#define in 1
#define out 2
#define sum 3

extern int htsize;

class HashTableCell{
	private:
		AccountList lista;
	public:
		void addNode(const int );
		Account* fetchAccount(const int);
		void emptyList();
		int fetchCounter();
		AccountListNode* fetchFirst();
		void print();
		void DeleteNode(const int);
};

class HashTable{
	private:
		HashTableCell** HTable;
	public:
		HashTable(const int);
		~HashTable();
		void createnondes(const int);
		void delnodes(const int);
		void addtran(const int,const int,const double);
		void deltran(const int,const int);
		void lookup(const int,const int);
		void conn(const int,const int);
		void dump(string);
		void bye();
		void triangle(const int,const double);
		void allcycles(const int);
		void traceflow(const int,const int);
		void print();
};
