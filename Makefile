CC=g++ -std=c++11
ZMQ=/home/utp/zmq
SFML=/usr/local

ZMQ_INCS=$(ZMQ)/include
ZMQ_LIBS=$(ZMQ)/lib
SFML_INCS=$(SFML)/include
SFML_LIBS=$(SFML)/lib

all:  dealer worker

dealer: dealer.cc
	$(CC) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o dealer dealer.cc -lczmq -lzmq

worker: worker.cc
	$(CC) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o worker worker.cc -lczmq -lzmq

clean:
	rm -f dealer
	rm -f worker 
