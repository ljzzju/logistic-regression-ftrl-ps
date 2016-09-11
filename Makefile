#!/bin/bash
CPP = g++
CPP_tag = -std=c++11
DEPS_PATH = ./repo/ps-lite/deps
INCLUDE = -I./repo/ps-lite/src -I./repo/ps-lite/deps/include -I./repo/dmlc-core/include

LDFLAGS = $(addprefix $(DEPS_PATH)/lib/, libglog.a libprotobuf.a libgflags.a libzmq.a libcityhash.a liblz4.a)


all: train dump_model

train: main.o ./repo/ps-lite/build/libps.a $(LDFLAGS) -lpthread
	$(CPP) $(CPP_tag) -o $@ $^ $(INCLUDE)

main.o: src/main.cpp ./repo/ps-lite/build/libps.a
	$(CPP) $(CPP_tag) -c src/main.cpp $(INCLUDE)

dump_model: dump.o ./repo/dmlc-core/libdmlc.a
	$(CPP) $(CPP_tag) -o $@ $^ $(INCLUDE)

dump.o: src/dump.cc ./repo/dmlc-core/libdmlc.a ./repo/ps-lite/deps/lib/libglog.a 
	$(CPP) $(CPP_tag) -c src/dump.cc $(INCLUDE)
	

clean:
	rm -f *~ train
	rm -f *.o
