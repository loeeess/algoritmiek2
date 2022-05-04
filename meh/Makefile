CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = standaard.o standaard2.o schema.o schema2.o testmain.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o TestSchema

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

standaard2.o: standaard2.cc standaard2.h
	$(CC) $(CompileParms)  standaard2.cc

schema.o: schema.cc standaard.h constantes.h schema.h
	$(CC) $(CompileParms)  schema.cc

schema2.o: standaard2.h constantes2.h schema2.h
	$(CC) $(CompileParms)  schema2.cc

testmain.o: testmain.cc constantes.h constantes2.h schema.h schema2.h
	$(CC) $(CompileParms)  testmain.cc

