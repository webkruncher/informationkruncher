



INCS+=../include/infosock.h
INCS+=../include/tools.h
INCS+=../include/oformat.h
INCS+=../include/spinlock.h

INCLUDES+=-I projects -I ../../contractkruncher -I ../../libdb/build_unix
LIBS= -L../../libdb/build_unix -L../../libdb/build_unix/.libs/ -ldb_cxx -lpthread

all: ../infokruncher.443 ../infokruncher.80 

../infokruncher.443: infokruncher.cpp $(INCS) ../../contractkruncher/ContractKruncher.a 
	g++ -DUSE_SSL=1 -DSERVICE_PORT=443 -fpermissive -pthread infokruncher.cpp $(LIBS) ${INCLUDES} -o ../infokruncher.443 -I../include -lcrypto -lssl ../../contractkruncher/ContractKruncher.a

../infokruncher.80: infokruncher.cpp $(INCS) ../../contractkruncher/ContractKruncher.a
	g++ -DUSE_SSL=0 -DSERVICE_PORT=80 -fpermissive -pthread infokruncher.cpp $(LIBS) ${INCLUDES} -o ../infokruncher.80 -I../include -lcrypto -lssl ../../contractkruncher/ContractKruncher.a

