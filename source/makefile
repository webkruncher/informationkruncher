



INCS+=../include/infosock.h
INCS+=../include/tools.h
INCS+=../include/oformat.h
INCS+=../include/spinlock.h

INCLUDES+=-I projects

all: ../infokruncher.81

../infokruncher.443: infokruncher.cpp $(INCS) 
	g++ -DUSE_SSL=1 -DSERVICE_PORT=443 -fpermissive -pthread infokruncher.cpp ${INCLUDES} -o ../infokruncher.443 -I../include -lcrypto -lssl

../infokruncher.80: infokruncher.cpp $(INCS) 
	g++ -DUSE_SSL=0 -DSERVICE_PORT=80 -fpermissive -pthread infokruncher.cpp ${INCLUDES} -o ../infokruncher.80 -I../include -lcrypto -lssl

../infokruncher.81: infokruncher.cpp $(INCS) 
	g++ -DUSE_SSL=0 -DSERVICE_PORT=81 -fpermissive -pthread infokruncher.cpp ${INCLUDES} -o ../infokruncher.81 -I../include -lcrypto -lssl
