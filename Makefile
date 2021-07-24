CC=gcc
CFLAG=-g
TARGET=test.exe

OBJS=gluethread/glthread.o\
	graph.o\
	topology.o

test.exe:testapp.c $(OBJS)
	$(CC) $(CFLAG) testapp.o $(OBJS) -o test.exe

testapp.o:testapp.c
	$(CC) $(CFLAG) -c testapp.c -o testapp.o 

gluethread/glthread.o: gluethread/glthread.c
	$(CC) $(CFLAG) -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o 

graph.o:graph.c graph.h 
	$(CC) $(CFLAG) -c graph.c -o graph.o 

topology.o:topology.c
	$(CC) $(CFLAG) -c topology.c -o topology.o 

clean:
	rm *.o
	rm glthread/glthread.o
	rm *.exe 