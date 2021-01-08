CC=g++
CFLAGS=-c -g -std=c++0x
OBJS1=board.o list.o strlist.o
OBJS2=boardpost.o list.o strlist.o

all: board boardpost

board: $(OBJS1)
	$(CC) -g $(OBJS1) -o $@

boardpost: $(OBJS2)
	$(CC) -g $(OBJS2) -o $@

board.o: board.cpp list.h
	$(CC) $(CFLAGS) board.cpp

boardpost.o: boardpost.cpp list.h
	$(CC) $(CFLAGS) boardpost.cpp

strlist.o: strlist.cpp strlist.h
	$(CC) $(CFLAGS) strlist.cpp

list.o: list.cpp list.h strlist.h
	$(CC) $(CFLAGS) list.cpp

clean :
	rm -f $(OBJS1) $(OBJS2) board boardpost
