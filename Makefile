CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = standaard.o schema.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Schema

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

schema.o: schema.cc standaard.h constantes.h schema.h
	$(CC) $(CompileParms)  schema.cc

main.o: main.cc standaard.h constantes.h schema.h
	$(CC) $(CompileParms)  main.cc

