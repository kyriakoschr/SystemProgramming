OBJS = main.o accounts.o transactions.o hasht.o list.o strlist.o
SOURCE = main.cpp accounts.cpp transactions.cpp hasht.cpp list.cpp strlist.cpp
HEADER = accounts.h transactions.h hasht.h list.h strlist.h
OUT = elegxos
CC = g++
FLAGS = -g -c -std=c++0x
$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

main.o: main.cpp hasht.h strlist.h
	$(CC) $(FLAGS) main.cpp
accounts.o: accounts.cpp accounts.h transactions.h list.h
	$(CC) $(FLAGS) accounts.cpp
transactions.o: transactions.cpp transactions.h
	$(CC) $(FLAGS) transactions.cpp
hasht.o: hasht.cpp hasht.h accounts.h
	$(CC) $(FLAGS) hasht.cpp
list.o: list.cpp list.h 
	$(CC) $(FLAGS) list.cpp
strlist.o: strlist.cpp strlist.h
	$(CC) $(FLAGS) strlist.cpp

clean :
	rm -f $(OBJS) $(OUT)

count :
	wc $(SOURCE) $(HEADER)
