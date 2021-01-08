CC=g++
CFLAGS=-c -std=c++0x -g3
OBJS1=main.o accounts.o hasht.o transactions.o strlist.o
OBJS2=maincl.o strlist.o
LIBS ?= -lpthread

all: server client

server: $(OBJS1)
	$(CC) -g $(OBJS1) $(LIBS) -o $@

client: $(OBJS2)
	$(CC) -g $(OBJS2) -o $@

main.o: main.cpp hasht.h
	$(CC) $(CFLAGS) main.cpp

maincl.o: maincl.cpp strlist.h
	$(CC) $(CFLAGS) maincl.cpp

strlist.o: strlist.cpp strlist.h
	$(CC) $(CFLAGS) strlist.cpp

accounts.o: accounts.cpp transactions.h
	$(CC) $(CFLAGS) accounts.cpp

hasht.o: hasht.cpp accounts.h strlist.h
	$(CC) $(CFLAGS) hasht.cpp

transactions.o: transactions.cpp transactions.h
	$(CC) $(CFLAGS) transactions.cpp

clean :
	rm -f $(OBJS1) $(OBJS2) server client
